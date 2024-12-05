//#include "stm32f0xx.h"
#include "stm32g0xx_hal.h"
#include "config.h"
#include "w5100/socket.h"
#include "w5100/w5100.h"
#include "ethTransceiver.h"
#include "systick.h"




#define BUF_LEN 32
#define PING_REQUEST 8
#define PING_REPLY 0
#define CODE_ZERO 0

#define SOCKET_ERROR 1
#define TIMEOUT_ERROR 2
#define SUCCESS 3
#define REPLY_ERROR 4
#define PING_DEBUG










typedef struct pingmsg
{
  uint8_t  Type; 		// 0 - Ping Reply, 8 - Ping Request
  uint8_t  Code;		// Always 0
  int16_t  CheckSum;	// Check sum
  int16_t  ID;	            // Identification
  int16_t  SeqNum; 	// Sequence Number
  int8_t	 Data[BUF_LEN];// Ping Data  : 1452 = IP RAW MTU - sizeof(Type+Code+CheckSum+ID+SeqNum)
} PINGMSGR;


PINGMSGR PingRequest;	 // Variable for Ping Request
PINGMSGR PingReply;	     // Variable for Ping Reply
static uint16_t RandomID = 0x1234; 
static uint16_t RandomSeqNum = 0x4321;
uint8_t ping_reply_received = 0; 
uint8_t req=0;
uint8_t rep=0;


unsigned char ethICMPPingSent;
unsigned char ethICMPPongReceived;
unsigned char ethICMPPongTimeout;
unsigned int ethICMPPongTimeoutCnt;

void ethICMPTick(void)
{
	if (ethICMPPingSent==0x01)
	{
		if (ethICMPPongTimeoutCnt>0)
		{
			ethICMPPongTimeoutCnt--;
		}

	}
}	
	



uint16_t checksum(uint8_t * data_buf, uint16_t len)
{
  uint16_t sum, tsum, i, j;
  uint32_t lsum;

  j = len >> 1;
  lsum = 0;
  tsum = 0;
  for (i = 0; i < j; i++)
    {
      tsum = data_buf[i * 2];
      tsum = tsum << 8;
      tsum += data_buf[i * 2 + 1];
      lsum += tsum;
    }
   if (len % 2)
    {
      tsum = data_buf[i * 2];
      lsum += (tsum << 8);
    }
    sum = (uint16_t)lsum;
    sum = ~(sum + (lsum >> 16));
  return sum;

}


uint16_t htons( uint16_t hostshort)
{
	
#if 1
  //#ifdef LITTLE_ENDIAN
	uint16_t netshort=0;
	netshort = (hostshort & 0xFF) << 8;

	netshort |= ((hostshort >> 8)& 0xFF);
	return netshort;
#else
	return hostshort;
#endif
}






uint8_t _ping_request(uint8_t s, uint8_t *addr){
  uint16_t i;
//	/* make header of the ping-request  */
		PingRequest.Type = PING_REQUEST;										// Ping-Request
		PingRequest.Code = CODE_ZERO;	                   		// Always '0'
		PingRequest.ID = htons(RandomID++);	       // set ping-request's ID to random integer value
		PingRequest.SeqNum =htons(RandomSeqNum++);// set ping-request's sequence number to ramdom integer value
//	/* Fill in Data[]  as size of BIF_LEN (Default = 32)*/
		for(i = 0 ; i < BUF_LEN; i++)
		{	                                
			PingRequest.Data[i] = (i) % 8;		  //'0'~'8' number into ping-request's data 	
		}
	 /* Do checksum of Ping Request */
		PingRequest.CheckSum = 0;		               // value of checksum before calucating checksum of ping-request packet
		PingRequest.CheckSum = htons(checksum((uint8_t*)&PingRequest,40));  // Calculate checksum
//	/* sendto ping_request to destination */
		sendto(s,(uint8_t*)&PingRequest,40,&addr[0],3000);
	return 0;
} 


















unsigned char ethICMPPingSend (unsigned char *address)
{
	
	if (ethICMPPingSent==0x00)
	{
		setSn_PROTO(3,IPPROTO_ICMP);
		socket(3,Sn_MR_IPRAW,3000,0);				
		sysTickDelay(10);
		ethICMPPongReceived=0x00;
		ethICMPPongTimeout=0x00;
		ethICMPPongTimeoutCnt=50;
		_ping_request (3, &address[0]);	
		ethICMPPingSent=0x01;		
	}
	return 0x01;
}


unsigned char ethICMPPongReceivedGet (void)
{
	return ethICMPPongReceived;	
}


unsigned char ethICMPPongTimeoutGet (void)
{
	return ethICMPPongTimeout;	
}


void ethICMPDoTask (void)
{
	unsigned int _rsize;	
	
	if (ethICMPPingSent==0x01)
	{
		_rsize=getSn_RX_RSR(3);
	
		if (_rsize>0) 
		{
			ethICMPPingSent=0x00;
			close(3);		
			ethICMPPongReceived=0x01;			
		}	
		
		if 	(ethICMPPongTimeoutCnt==0x00)
		{
			ethICMPPingSent=0x00;
			close(3);		
			ethICMPPongTimeout=0x01;
		}		
		
		
	}	
}


