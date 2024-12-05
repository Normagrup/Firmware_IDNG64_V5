//#include "stm32f0xx.h"
#include "config.h"
#include "ethFramming.h"
#include "ethTransceiver.h"
#include "systick.h"
#include "daliUtils.h"
#include "w5100/w5100.h"
#include "iED.h"

unsigned char macAdrress[6];

volatile unsigned int ethTransceiverInicializadoCnt;
volatile unsigned char ethTransceiverEstado;
volatile unsigned char ethTransceiverTiempoSinRecibirCnt;
volatile unsigned char ethTransceiverIntentosFallidosCnt;
volatile unsigned char sendKeepAliveYa;
volatile unsigned char ethTransceiverKeepAliveWhenConnectedCnt;

extern unsigned int portDest;

extern unsigned char _configIpAddress[4];

unsigned char _sendI1Abierto;
unsigned char _sendI1Cerrado;
unsigned char _sendI2Abierto;
unsigned char _sendI2Cerrado;

unsigned char _broadcastAddress[4];

void sendI1Abierto (void)
{
	_sendI1Abierto=0x01;
}

void sendI1Cerrado (void)
{
	_sendI1Cerrado=0x01;
}

void sendI2Abierto (void)
{
	_sendI2Abierto=0x01;
}

void sendI2Cerrado (void)
{
	_sendI2Cerrado=0x01;
}


void ethTransceiverReset (void)
{
	ethTransceiverIntentosFallidosCnt=0;
	ethTransceiverEstado=0x00;
}

void ethTransceiverLAN_test (void)
{
	ethTransceiverIntentosFallidosCnt=0;
	ethTransceiverEstado=0x03;
}


void sendKeepAlive (void)
{
	sendKeepAliveYa=0x01;
}


void ethTransceiverTiempoSinRecibirCntRst (void)
{
	ethTransceiverTiempoSinRecibirCnt=0x00;	
}



void ethTransceiverRTCC_Tick (void)
{
		if (ethTransceiverEstado==0x05)
		{
			sendKeepAlive();			
			if (ethTransceiverTiempoSinRecibirCnt<2)
			{
				ethTransceiverTiempoSinRecibirCnt++;
			}			
			else
			{
				ethTransceiverTiempoSinRecibirCnt=0x00;
				ethTransceiverLAN_test();	
			}
		}			
}

unsigned char ethTransceiverConTestActiveGet (void)
{
	unsigned char gatewayAddress[4];
	configGatewayAddressGet(&gatewayAddress[0]);	
	if ((gatewayAddress[0]==0x00) && (gatewayAddress[1]==0x00) && (gatewayAddress[2]==0x00) && (gatewayAddress[3]==0x00))
	{
		return 0x00;		
	}
	else
	{
		return 0x01;
	}	
}

	
void ethTransceiverTick (void)
{
	if (ethTransceiverInicializadoCnt>0)
	{
		ethTransceiverInicializadoCnt--;
	}
	
	if (ethTransceiverKeepAliveWhenConnectedCnt>0)
	{
		ethTransceiverKeepAliveWhenConnectedCnt--;
	}
	
	
}


void macInit (void)
{	
	boardI2CRead(&macAdrress[0], 0xFA, 6);
}













void ethTransceiverDoTask (void)
{
	unsigned char gatewayAddress[4];
	configGatewayAddressGet(&gatewayAddress[0]);
	switch (ethTransceiverEstado)
	{
		case 0x00:
		{		
			boardW5100ResetON();
			ethTransceiverInicializadoCnt=100;
			ethTransceiverEstado=0x01;
			break;
		}
		
		case 0x01:
		{
			if (ethTransceiverInicializadoCnt==0)
			{
				ethTransceiverInicializadoCnt=100;
				ethTransceiverEstado=0x02;
			}
			break;
		}

		case 0x02:
		{
			if (ethTransceiverInicializadoCnt==0)
			{
				ethTransceiverW5100Init();							//Init transceiver
				ethTransceiverEstado=0x03;			
				ethTransceiverInicializadoCnt=200;				
			}
			break;
		}	

		case 0x03:
		{
			if (ethTransceiverConTestActiveGet()==0x01)
			{		
				if (ethTransceiverInicializadoCnt==0)
				{
					ethICMPPingSend(&gatewayAddress[0]);
					ethTransceiverEstado=0x04;	
				}				
			}
			else
			{
				ethTransceiverEstado=0x05;
				sendKeepAliveYa=1;
				ethTransceiverKeepAliveWhenConnectedCnt=10;		
			}
			break;
		}		
		
		case 0x04:
		{
			if (ethTransceiverConTestActiveGet()==0x01)
			{
				if ((ethICMPPongReceivedGet()==0x01) || (ethICMPPongTimeoutGet()==0x01))
				{
					if ((ethICMPPongReceivedGet()==0x01) || (ethTransceiverIntentosFallidosCnt>10))
					{
						ethTransceiverEstado=0x05;	
						sendKeepAliveYa=1;
						ethTransceiverKeepAliveWhenConnectedCnt=10;

					}				
					else
					{
						ethTransceiverIntentosFallidosCnt++;
						ethTransceiverEstado=0x00;	
						botoneraFailLedBlink();
						
					}					
				}			
			}
			else
			{
				ethTransceiverEstado=0x05;	
				sendKeepAliveYa=1;
				ethTransceiverKeepAliveWhenConnectedCnt=10;				
			}
			break;
		}		
		
		case 0x05:
		{
			webServerDoTask();			
			if ((sendKeepAliveYa==0x01)  && (ethTransceiverKeepAliveWhenConnectedCnt==0x00))
			{
					unsigned char keepAlive1[7];	
					keepAlive1[0]=0xFF;
					keepAlive1[1]=macAdrress[0];
					keepAlive1[2]=macAdrress[1];
					keepAlive1[3]=macAdrress[2];
					keepAlive1[4]=macAdrress[3];
					keepAlive1[5]=macAdrress[4];
					keepAlive1[6]=macAdrress[5];
					ethSendFrame(0,&keepAlive1[0],7,&_broadcastAddress[0], portDest);						
					sendKeepAliveYa=0x00;
			}			
				

			break;
		}				
	}
}


























void ethTransceiverInit (void)
{
	ethTransceiverIntentosFallidosCnt=0;	
	ethTransceiverEstado=0x00;
}

void ethTransceiverW5100Init (void)
{
	unsigned char ipAddress[4];
	unsigned char submaskAddress[4];
	unsigned char gatewayAddress[4];
	configIpAddressGet(&ipAddress[0]);
	configSubmaskAddressGet(&submaskAddress[0]);
	configGatewayAddressGet(&gatewayAddress[0]);
	boardW5100ResetOFF();
	sysTickDelay(10);
	iinchip_init();	
	setGAR(&gatewayAddress[0]);
	setSHAR(&macAdrress[0]);
	setSUBR(&submaskAddress[0]);
	applySUBR();
	setSIPR(&ipAddress[0]);		
	sysinit(10,01);
	sysTickDelay(10);	
	socket(0,Sn_MR_UDP,2234,0);	
	sysTickDelay(10);
	_broadcastAddress[0]=ipAddress[0] | (~submaskAddress[0]);
	_broadcastAddress[1]=ipAddress[1] |	(~submaskAddress[1]);
	_broadcastAddress[2]=ipAddress[2] |	(~submaskAddress[2]);
	_broadcastAddress[3]=ipAddress[3] |	(~submaskAddress[3]);
	
}












