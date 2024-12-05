/*
 * (c)COPYRIGHT
 * ALL RIGHT RESERVED
 *
 * FileName : w5100.c
 * Revision History :
 * ----------	-------		------------------------------------------------
 * 	Date			version	  	Description
 * ----------	-------  	------------------------------------------------
 * 01/25/2007	1.1			Bug is Fixed in the Indirect Mode 
 *							: Memory mapping error	
 * ----------	-------		------------------------------------------------
 * 01/08/2008	1.2			Modification of unsigned char Command Part
 *							: Check if the appropriately performed after writing Sn_CR
 *
 *							Modification of SPI Part
 *							: SPI code changed by adding 'spi.h'.
 *							: Change control type for SPI port from byte to bit.
 * ----------	-------		------------------------------------------------
 * 01/15/2008	1.3			Bug is Fixed in the pppinit() fuction.
 *							: do not clear interrupt value, so fixed.
 *
 *		                   			Modification of ISR
 *                   				: Do not exit ISR, if there is interrupt.
 * ----------	-------		------------------------------------------------
 * 03/21/2008	1.4			Modification of SetMR() function
 *                   				: Use IINCHIP_WRITE() function in Direct or SPI mode.
 * ----------	-------		------------------------------------------------
 * 03/21/2008	1.5			Bug is Fixed in the pppinit() fuction.
 *							: do not clear receive buffer, so fixed. +200903[bj] clear receive buffer
 * ----------	-------		------------------------------------------------
 * 03/13/2012	1.6			Added clearSUBR(), applySUBR() and modified setSUBR() functions
 *							      because of the ARP errata.
 * ----------	-------		------------------------------------------------
 * 10/01/2013	1.6.2		Removed clearSUBR(), applySUBR() and modified setSUBR() functions
 *							Currently ARP Errata is testing.
 *							: It is the same with Ver 1.5
 * ----------	-------		------------------------------------------------
 * 10/11/2013	1.7			Added clearSUBR(), applySUBR() and modified setSUBR() functions
 *							because of the ARP errata.
 *											
 * ----------	-------		------------------------------------------------
 * 10/28/2013	1.8			modify clearSUBR()functions because of the ARP errata.
 *							but it was only solved the Erratum3.
 *											
 * ----------	-------		------------------------------------------------
 */
#include <stdio.h>
#include <string.h>

  
#include "types.h"
#include "socket.h"
#include "w5100.h"




#ifdef __DEF_IINCHIP_PPP__
   #include "md5.h"
#endif


#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_SPI_MODE__)
#include "spi.h"		//+2007113[jhpark]
#endif

static unsigned char I_STATUS[MAX_SOCK_NUM];
static unsigned int SMASK[MAX_SOCK_NUM];           /**< Variable for Tx buffer MASK in each channel */
static unsigned int RMASK[MAX_SOCK_NUM];           /**< Variable for Rx buffer MASK in each channel */
static unsigned int SSIZE[MAX_SOCK_NUM];           /**< Max Tx buffer size by each channel */
static unsigned int RSIZE[MAX_SOCK_NUM];           /**< Max Rx buffer size by each channel */
static unsigned int SBUFBASEADDRESS[MAX_SOCK_NUM]; /**< Tx buffer base address by each channel */
static unsigned int RBUFBASEADDRESS[MAX_SOCK_NUM]; /**< Rx buffer base address by each channel */
static unsigned char SUBN_VAR[4];
static unsigned char IP_VAR[4];


unsigned char getISR(unsigned char s)
{
	return I_STATUS[s];
}

void putISR(unsigned char s, unsigned char val)
{
   I_STATUS[s] = val;
}

unsigned int getIINCHIP_RxMAX(unsigned char s)
{
   return RSIZE[s];
}

unsigned int getIINCHIP_TxMAX(unsigned char s)
{
   return SSIZE[s];
}

unsigned int getIINCHIP_RxMASK(unsigned char s)
{
   return RMASK[s];
}

unsigned int getIINCHIP_TxMASK(unsigned char s)
{
   return SMASK[s];
}

unsigned int getIINCHIP_RxBASE(unsigned char s)
{
   return RBUFBASEADDRESS[s];
}
unsigned int getIINCHIP_TxBASE(unsigned char s)
{
   return SBUFBASEADDRESS[s];
}

 /**
@brief	This function writes the data into W5100 registers.
*/
unsigned char IINCHIP_WRITE(unsigned int addr,unsigned char data)
{
// DIRECT MODE I/F
#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_DIRECT_MODE__)
	IINCHIP_ISR_DISABLE();
	*((volatile unsigned char*)(addr)) = data;
	IINCHIP_ISR_ENABLE();
#elif(__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)	/* INDIRECT MODE I/F */
	   IINCHIP_ISR_DISABLE();
	   *((volatile unsigned char*)IDM_AR0) = (unsigned char)((addr & 0xFF00) >> 8);
	   *((volatile unsigned char*)IDM_AR1) = (unsigned char)(addr & 0x00FF);
	   *((volatile unsigned char*)IDM_DR)  = data;
	   IINCHIP_ISR_ENABLE();
#elif (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_SPI_MODE__)
	   IINCHIP_ISR_DISABLE();
	//IINCHIP_SpiInit();
	
	//SPI MODE I/F
	
	
	
	spiIINCHIP_CSoff_w5100();                             // CS=0, SPI start
	
	

	spiIINCHIP_SpiSendData(0xF0);
	spiIINCHIP_SpiSendData((addr & 0xFF00) >> 8);
	spiIINCHIP_SpiSendData(addr & 0x00FF);
	spiIINCHIP_SpiSendData(data);

	spiIINCHIP_CSon_w5100();    

	   IINCHIP_ISR_ENABLE();   
#else
	#error "unknown bus type"
#endif
	return 1;
}


/**
@brief	This function reads the value from W5100 registers.
*/
unsigned char IINCHIP_READ(unsigned int addr)
{
	unsigned char data;

// DIRECT MODE I/F

#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_DIRECT_MODE__)
	IINCHIP_ISR_DISABLE();
	data = *((volatile unsigned char*)(addr));
	IINCHIP_ISR_ENABLE();
#elif(__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)   
	   IINCHIP_ISR_DISABLE();
	   *((volatile unsigned char*)IDM_AR0) = (unsigned char)((addr & 0xFF00) >> 8);
	   *((volatile unsigned char*)IDM_AR1) = (unsigned char)(addr & 0x00FF);
	   data = *((volatile unsigned char*)IDM_DR);
	   IINCHIP_ISR_ENABLE();
   
#elif (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_SPI_MODE__)
	   IINCHIP_ISR_DISABLE();
	//IINCHIP_SpiInit();	
	spiIINCHIP_CSoff_w5100();                             // CS=0, SPI start

	spiIINCHIP_SpiSendData(0x0F);
	spiIINCHIP_SpiSendData((addr & 0xFF00) >> 8);
	spiIINCHIP_SpiSendData(addr & 0x00FF);
	
	
	//spiIINCHIP_SpiSendData(0);
	data = spiIINCHIP_SpiRecvData();

	spiIINCHIP_CSon_w5100();                          	// SPI end
	   IINCHIP_ISR_ENABLE();
#else
	#error "unknown bus type"
#endif
	return data;
}


/**
@brief	This function writes into W5100 memory(Buffer)
*/ 
unsigned int wiz_write_buf(unsigned int addr,unsigned char* buf,unsigned int len)
{
#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_DIRECT_MODE__)
	IINCHIP_ISR_DISABLE();
   	memcpy((unsigned char *)addr, buf, len);
	IINCHIP_ISR_ENABLE();
#elif (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)
	   unsigned int idx = 0;
	   IINCHIP_ISR_DISABLE();
	   *((volatile unsigned char*)IDM_AR0) = (unsigned char)((addr & 0xFF00) >> 8);
	   *((volatile unsigned char*)IDM_AR1) = (unsigned char)(addr & 0x00FF);
	   for (idx = 0; idx < len ; idx++) *((volatile unsigned char*)IDM_DR) = buf[idx];
	   IINCHIP_ISR_ENABLE();
#elif (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_SPI_MODE__)
	unsigned int idx = 0;
	
	   IINCHIP_ISR_DISABLE();
	//IINCHIP_SpiInit();
      
	//SPI MODE I/F
	for(idx=0;idx<len;idx++)
	   {
		spiIINCHIP_CSoff_w5100();                             // CS=0, SPI start 

		spiIINCHIP_SpiSendData(0xF0);
		spiIINCHIP_SpiSendData(((addr+idx) & 0xFF00) >> 8);
		spiIINCHIP_SpiSendData((addr+idx) & 0x00FF);
		spiIINCHIP_SpiSendData(buf[idx]);

		spiIINCHIP_CSon_w5100();                             // CS=0, SPI end 
	   }

	   IINCHIP_ISR_ENABLE();	   
#else
	#error "unknown bus type"
#endif
	return len;
}


/**
@brief	This function reads into W5100 memory(Buffer)
*/ 
unsigned int wiz_read_buf(unsigned int addr, unsigned char* buf,unsigned int len)
{
#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_DIRECT_MODE__)
	IINCHIP_ISR_DISABLE();
	memcpy(buf, (unsigned char *)addr, len);
	IINCHIP_ISR_ENABLE();
#elif(__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)
	   unsigned int idx = 0;
	   IINCHIP_ISR_DISABLE();
	   *((volatile unsigned char*)IDM_AR0) = (unsigned char)((addr & 0xFF00) >> 8);
	   *((volatile unsigned char*)IDM_AR1) = (unsigned char)(addr & 0x00FF);
	   for (idx = 0; idx < len ; idx++) buf[idx] = *((volatile unsigned char*)IDM_DR);
	   IINCHIP_ISR_ENABLE();
#elif (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_SPI_MODE__)
	unsigned int idx = 0;
	   IINCHIP_ISR_DISABLE();

	//IINCHIP_SpiInit();
   
	for (idx=0; idx<len; idx++)
      	   {
		spiIINCHIP_CSoff_w5100();                             // CS=0, SPI start 

		spiIINCHIP_SpiSendData(0x0F);
		spiIINCHIP_SpiSendData(((addr+idx) & 0xFF00) >> 8);
		spiIINCHIP_SpiSendData((addr+idx) & 0x00FF);


		//spiIINCHIP_SpiSendData(0);
		buf[idx] = spiIINCHIP_SpiRecvData();

		spiIINCHIP_CSon_w5100();                             // CS=0, SPI end 	   
	   }

	   IINCHIP_ISR_ENABLE();
#else
	#error "unknown bus type"
#endif
	return len;
}


/**
@brief	This function is for resetting of the iinchip. Initializes the iinchip to work in whether DIRECT or INDIRECT mode
*/ 
void iinchip_init(void)
{	
	setMR( MR_RST );
#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)
	setMR( MR_IND | MR_AI );
#ifdef __DEF_IINCHIP_DBG__	
	printf("MR value is %d \r\n",IINCHIP_READ(MR));
#endif	
#endif
}


/**
@brief	This function set the transmit & receive buffer size as per the channels is used

Note for TMSR and RMSR bits are as follows\n
bit 1-0 : memory size of channel #0 \n
bit 3-2 : memory size of channel #1 \n
bit 5-4 : memory size of channel #2 \n
bit 7-6 : memory size of channel #3 \n\n
Maximum memory size for Tx, Rx in the W5100 is 8K Bytes,\n
In the range of 8KBytes, the memory size could be allocated dynamically by each channel.\n
Be attentive to sum of memory size shouldn't exceed 8Kbytes\n
and to data transmission and receiption from non-allocated channel may cause some problems.\n
If the 8KBytes memory is already  assigned to centain channel, \n
other 3 channels couldn't be used, for there's no available memory.\n
If two 4KBytes memory are assigned to two each channels, \n
other 2 channels couldn't be used, for there's no available memory.\n
*/ 
void sysinit(
	unsigned char tx_size, 	/**< tx_size Tx memory size (00 - 1KByte, 01- 2KBtye, 10 - 4KByte, 11 - 8KByte) */
	unsigned char rx_size		/**< rx_size Rx memory size (00 - 1KByte, 01- 2KBtye, 10 - 4KByte, 11 - 8KByte) */
	)
{
	int16 i;
	int16 ssum,rsum;

#ifdef __DEF_IINCHIP_DBG__
	printf("sysinit()\r\n");
#endif

	ssum = 0;
	rsum = 0;
	
	IINCHIP_WRITE(TMSR,tx_size); /* Set Tx memory size for each channel */
	IINCHIP_WRITE(RMSR,rx_size);	 /* Set Rx memory size for each channel */

	SBUFBASEADDRESS[0] = (unsigned int)(__DEF_IINCHIP_MAP_TXBUF__);		/* Set base address of Tx memory for channel #0 */
	RBUFBASEADDRESS[0] = (unsigned int)(__DEF_IINCHIP_MAP_RXBUF__);		/* Set base address of Rx memory for channel #0 */

#ifdef __DEF_IINCHIP_DBG__
	printf("Channel : SEND MEM SIZE : RECV MEM SIZE\r\n");
#endif

   for (i = 0 ; i < MAX_SOCK_NUM; i++)       // Set the size, masking and base address of Tx & Rx memory by each channel
	{
		SSIZE[i] = (int16)(0);
		RSIZE[i] = (int16)(0);
		if (ssum < 8192)
		{
         switch((tx_size >> i*2) & 0x03)  // Set Tx memory size
			{
			case 0:
				SSIZE[i] = (int16)(1024);
				SMASK[i] = (unsigned int)(0x03FF);
				break;
			case 1:
				SSIZE[i] = (int16)(2048);
				SMASK[i] = (unsigned int)(0x07FF);
				break;
			case 2:
				SSIZE[i] = (int16)(4096);
				SMASK[i] = (unsigned int)(0x0FFF);
				break;
			case 3:
				SSIZE[i] = (int16)(8192);
				SMASK[i] = (unsigned int)(0x1FFF);
				break;
			}
		}
		if (rsum < 8192)
		{
         switch((rx_size >> i*2) & 0x03)     // Set Rx memory size
			{
			case 0:
				RSIZE[i] = (int16)(1024);
				RMASK[i] = (unsigned int)(0x03FF);
				break;
			case 1:
				RSIZE[i] = (int16)(2048);
				RMASK[i] = (unsigned int)(0x07FF);
				break;
			case 2:
				RSIZE[i] = (int16)(4096);
				RMASK[i] = (unsigned int)(0x0FFF);
				break;
			case 3:
				RSIZE[i] = (int16)(8192);
				RMASK[i] = (unsigned int)(0x1FFF);
				break;
			}
		}
		ssum += SSIZE[i];
		rsum += RSIZE[i];

      if (i != 0)             // Sets base address of Tx and Rx memory for channel #1,#2,#3
		{
			SBUFBASEADDRESS[i] = SBUFBASEADDRESS[i-1] + SSIZE[i-1];
			RBUFBASEADDRESS[i] = RBUFBASEADDRESS[i-1] + RSIZE[i-1];
		}
#ifdef __DEF_IINCHIP_DBG__
		printf("%d : %.4x : %.4x : %.4x : %.4x\r\n", i, (unsigned int)SBUFBASEADDRESS[i], (unsigned int)RBUFBASEADDRESS[i], SSIZE[i], RSIZE[i]);
#endif
	}
}


void setMR(unsigned char val)
{

#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)
	*((volatile unsigned char*)(MR)) = val;
#else
	/* 	DIRECT ACCESS	*/
	IINCHIP_WRITE(MR,val);
#endif	
}


/**
@brief	This function sets up gateway IP address.
*/ 
void setGAR(
	unsigned char * addr	/**< a pointer to a 4 -byte array responsible to set the Gateway IP address. */
	)
{
	IINCHIP_WRITE((GAR0 + 0),addr[0]);
	IINCHIP_WRITE((GAR0 + 1),addr[1]);
	IINCHIP_WRITE((GAR0 + 2),addr[2]);
	IINCHIP_WRITE((GAR0 + 3),addr[3]);
}
void getGWIP(unsigned char * addr)
{
	addr[0] = IINCHIP_READ((GAR0 + 0));
	addr[1] = IINCHIP_READ((GAR0 + 1));
	addr[2] = IINCHIP_READ((GAR0 + 2));
	addr[3] = IINCHIP_READ((GAR0 + 3));
}


/**
@brief	It sets up SubnetMask address
*/ 
void setSUBR(
	unsigned char * addr	/**< a pointer to a 4 -byte array responsible to set the SubnetMask address */
	)
{
  SUBN_VAR[0] = addr[0];
	SUBN_VAR[1] = addr[1];
	SUBN_VAR[2] = addr[2];
	SUBN_VAR[3] = addr[3];
}


/**
@brief	It sets up SubnetMask address
*/ 
void applySUBR(
	void	/**< a pointer to a 4 -byte array responsible to set the SubnetMask address */
	)
{
	IINCHIP_WRITE((SUBR0 + 0), SUBN_VAR[0]);
	IINCHIP_WRITE((SUBR0 + 1), SUBN_VAR[1]);
	IINCHIP_WRITE((SUBR0 + 2), SUBN_VAR[2]);
	IINCHIP_WRITE((SUBR0 + 3), SUBN_VAR[3]);
}


/**
@brief	It sets up SubnetMask address
*/ 
void clearSUBR(
	void	/**< a pointer to a 4 -byte array responsible to set the SubnetMask address */
	)
{
	
	getSIPR(&IP_VAR[0]);
	
	if((IP_VAR[0]==0 && IP_VAR[1]==0 &&IP_VAR[2]==0 &&IP_VAR[3]==0))
	{
		IINCHIP_WRITE((SUBR0 + 0), 0);
		IINCHIP_WRITE((SUBR0 + 1), 0);
		IINCHIP_WRITE((SUBR0 + 2), 0);
		IINCHIP_WRITE((SUBR0 + 3), 0);
	}

}


/**
@brief	This function sets up MAC address.
*/ 
void setSHAR(
	unsigned char * addr	/**< a pointer to a 6 -byte array responsible to set the MAC address. */
	)
{
	IINCHIP_WRITE((SHAR0 + 0),addr[0]);
	IINCHIP_WRITE((SHAR0 + 1),addr[1]);
	IINCHIP_WRITE((SHAR0 + 2),addr[2]);
	IINCHIP_WRITE((SHAR0 + 3),addr[3]);
	IINCHIP_WRITE((SHAR0 + 4),addr[4]);
	IINCHIP_WRITE((SHAR0 + 5),addr[5]);
}


/**
@brief	This function sets up Source IP address.
*/
void setSIPR(
	unsigned char * addr	/**< a pointer to a 4 -byte array responsible to set the Source IP address. */
	)
{
	IINCHIP_WRITE((SIPR0 + 0),addr[0]);
	IINCHIP_WRITE((SIPR0 + 1),addr[1]);
	IINCHIP_WRITE((SIPR0 + 2),addr[2]);
	IINCHIP_WRITE((SIPR0 + 3),addr[3]);
}


/**
@brief	This function gets Interrupt register in common register.
 */
unsigned char getIR( void )
{
   return IINCHIP_READ(IR);
}



/**
@brief	This function sets up Retransmission time.

If there is no response from the peer or delay in response then retransmission 
will be there as per RTR (Retry Time-value Register)setting
*/
void setRTR(unsigned int timeout)
{
	IINCHIP_WRITE(RTR0,(unsigned char)((timeout & 0xff00) >> 8));
	IINCHIP_WRITE((RTR0 + 1),(unsigned char)(timeout & 0x00ff));
}


/**
@brief	This function set the number of Retransmission.

If there is no response from the peer or delay in response then recorded time 
as per RTR & RCR register seeting then time out will occur.
*/
void setRCR(unsigned char retry)
{
	IINCHIP_WRITE(RCR,retry);
}


/**
@brief	This function set the interrupt mask Enable/Disable appropriate Interrupt. ('1' : interrupt enable)

If any bit in IMR is set as '0' then there is not interrupt signal though the bit is
set in IR register.
*/
void setIMR(unsigned char mask)
{
	IINCHIP_WRITE(IMR,mask); // must be setted 0x10.
}


/**
@brief	These below functions are used to get the Gateway, SubnetMask
		and Source Hardware Address (MAC Address) and Source IP address
*/
void getGAR(unsigned char * addr)
{
	addr[0] = IINCHIP_READ(GAR0);
	addr[1] = IINCHIP_READ(GAR0+1);
	addr[2] = IINCHIP_READ(GAR0+2);
	addr[3] = IINCHIP_READ(GAR0+3);
}
void getSUBR(unsigned char * addr)
{
	addr[0] = SUBN_VAR[0];
	addr[1] = SUBN_VAR[1];
	addr[2] = SUBN_VAR[2];
	addr[3] = SUBN_VAR[3];
}
void getSHAR(unsigned char * addr)
{
	addr[0] = IINCHIP_READ(SHAR0);
	addr[1] = IINCHIP_READ(SHAR0+1);
	addr[2] = IINCHIP_READ(SHAR0+2);
	addr[3] = IINCHIP_READ(SHAR0+3);
	addr[4] = IINCHIP_READ(SHAR0+4);
	addr[5] = IINCHIP_READ(SHAR0+5);
}
void getSIPR(unsigned char * addr)
{
	addr[0] = IINCHIP_READ(SIPR0);
	addr[1] = IINCHIP_READ(SIPR0+1);
	addr[2] = IINCHIP_READ(SIPR0+2);
	addr[3] = IINCHIP_READ(SIPR0+3);
}


/**
@brief	These below functions are used to get the Destination Hardware Address (MAC Address), Destination IP address and Destination Port.
*/
void getSn_DHAR(unsigned char s, unsigned char * addr)
{
	addr[0] = IINCHIP_READ(Sn_DHAR0(s));
	addr[1] = IINCHIP_READ(Sn_DHAR0(s)+1);
	addr[2] = IINCHIP_READ(Sn_DHAR0(s)+2);
	addr[3] = IINCHIP_READ(Sn_DHAR0(s)+3);
	addr[4] = IINCHIP_READ(Sn_DHAR0(s)+4);
	addr[5] = IINCHIP_READ(Sn_DHAR0(s)+5);
}
void setSn_DHAR(unsigned char s, unsigned char * addr)
{
	IINCHIP_WRITE((Sn_DHAR0(s) + 0),addr[0]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 1),addr[1]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 2),addr[2]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 3),addr[3]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 4),addr[4]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 5),addr[5]);
}
void getSn_DIPR(unsigned char s, unsigned char * addr)
{
	addr[0] = IINCHIP_READ(Sn_DIPR0(s));
	addr[1] = IINCHIP_READ(Sn_DIPR0(s)+1);
	addr[2] = IINCHIP_READ(Sn_DIPR0(s)+2);
	addr[3] = IINCHIP_READ(Sn_DIPR0(s)+3);
}
void setSn_DIPR(unsigned char s, unsigned char * addr)
{
	IINCHIP_WRITE((Sn_DIPR0(s) + 0),addr[0]);
	IINCHIP_WRITE((Sn_DIPR0(s) + 1),addr[1]);
	IINCHIP_WRITE((Sn_DIPR0(s) + 2),addr[2]);
	IINCHIP_WRITE((Sn_DIPR0(s) + 3),addr[3]);
}
void getSn_DPORT(unsigned char s, unsigned char * addr)
{
	addr[0] = IINCHIP_READ(Sn_DPORT0(s));
	addr[1] = IINCHIP_READ(Sn_DPORT0(s)+1);
}
void setSn_DPORT(unsigned char s, unsigned char * addr)
{
	IINCHIP_WRITE((Sn_DPORT0(s) + 0),addr[0]);
	IINCHIP_WRITE((Sn_DPORT0(s) + 1),addr[1]);
}


/**
@brief	This sets the maximum segment size of TCP in Active Mode), while in Passive Mode this is set by peer
*/
void setSn_MSS(unsigned char s, unsigned int Sn_MSSR0)
{
	IINCHIP_WRITE(Sn_MSSR0(s),(unsigned char)((Sn_MSSR0 & 0xff00) >> 8));
	IINCHIP_WRITE((Sn_MSSR0(s) + 1),(unsigned char)(Sn_MSSR0 & 0x00ff));
}

void setSn_TTL(unsigned char s, unsigned char ttl)
{
   IINCHIP_WRITE(Sn_TTL(s), ttl);
}


/**
@brief	These below function is used to setup the Protocol Field of IP Header when
		executing the IP Layer RAW mode.
*/
void setSn_PROTO(unsigned char s, unsigned char proto)
{
	IINCHIP_WRITE(Sn_PROTO(s),proto);
}


/**
@brief	get unsigned char interrupt status

These below functions are used to read the Interrupt & Soket Status register
*/
unsigned char getSn_IR(unsigned char s)
{
   return IINCHIP_READ(Sn_IR(s));
}

void setSn_IR(unsigned char s, unsigned char ir)
{
	IINCHIP_WRITE(Sn_IR(s),ir);
}

/**
@brief	 get unsigned char status
*/
unsigned char getSn_SR(unsigned char s)
{
   return IINCHIP_READ(Sn_SR(s));
}


/**
@brief	get unsigned char TX free buf size

This gives free buffer size of transmit buffer. This is the data size that user can transmit.
User shuold check this value first and control the size of transmitting data
*/
unsigned int getSn_TX_FSR(unsigned char s)
{
	unsigned int val=0,val1=0;
	do
	{
		val1 = IINCHIP_READ(Sn_TX_FSR0(s));
		val1 = (val1 << 8) + IINCHIP_READ(Sn_TX_FSR0(s) + 1);
    if (val1 != 0)
		{
   			val = IINCHIP_READ(Sn_TX_FSR0(s));
   			val = (val << 8) + IINCHIP_READ(Sn_TX_FSR0(s) + 1);
		}
	} while (val != val1);
  return val;
}


/**
@brief	 get unsigned char RX recv buf size

This gives size of received data in receive buffer. 
*/
unsigned int getSn_RX_RSR(unsigned char s)
{
	unsigned int val=0,val1=0;
	do
	{
		val1 = IINCHIP_READ(Sn_RX_RSR0(s));
		val1 = (val1 << 8) + IINCHIP_READ(Sn_RX_RSR0(s) + 1);
      if(val1 != 0)
		{
   			val = IINCHIP_READ(Sn_RX_RSR0(s));
   			val = (val << 8) + IINCHIP_READ(Sn_RX_RSR0(s) + 1);
		}
	} while (val != val1);
   return val;
}


/**
@brief	 This function is being called by send() and sendto() function also. 

This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
register. User should read upper byte first and lower byte later to get proper value.
*/
void send_data_processing(unsigned char s, unsigned char *data, unsigned int len)
{
	unsigned int ptr;
	ptr = IINCHIP_READ(Sn_TX_WR0(s));
	ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_TX_WR0(s) + 1);
	write_data(s, data, (unsigned char *)(ptr), len);
	ptr += len;
	IINCHIP_WRITE(Sn_TX_WR0(s),(unsigned char)((ptr & 0xff00) >> 8));
	IINCHIP_WRITE((Sn_TX_WR0(s) + 1),(unsigned char)(ptr & 0x00ff));
}


/**
@brief	This function is being called by recv() also.

This function read the Rx read pointer register
and after copy the data from receive buffer update the Rx write pointer register.
User should read upper byte first and lower byte later to get proper value.
*/
void recv_data_processing(unsigned char s, unsigned char *data, unsigned int len)
{
	unsigned int ptr;
	ptr = IINCHIP_READ(Sn_RX_RD0(s));
	ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RD0(s) + 1);
#ifdef __DEF_IINCHIP_DBG__
	printf("ISR_RX: rd_ptr : %.4x\r\n", ptr);
#endif
	read_data(s, (unsigned char *)ptr, data, len); // read data
	ptr += len;
	IINCHIP_WRITE(Sn_RX_RD0(s),(unsigned char)((ptr & 0xff00) >> 8));
	IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(unsigned char)(ptr & 0x00ff));
}


/**
@brief	for copy the data form application buffer to Transmite buffer of the chip.

This function is being used for copy the data form application buffer to Transmite
buffer of the chip. It calculate the actual physical address where one has to write
the data in transmite buffer. Here also take care of the condition while it exceed
the Tx memory uper-bound of unsigned char.
*/
void write_data(unsigned char s, volatile unsigned char * src, volatile unsigned char * dst, unsigned int len)
{
	unsigned int size;
	unsigned int dst_mask;
	unsigned char * dst_ptr;

	dst_mask = (unsigned int)dst & getIINCHIP_TxMASK(s);
	dst_ptr = (unsigned char *)(getIINCHIP_TxBASE(s) + dst_mask);
	
	if (dst_mask + len > getIINCHIP_TxMAX(s)) 
	{
		size = getIINCHIP_TxMAX(s) - dst_mask;
		wiz_write_buf((unsigned int)dst_ptr, (unsigned char*)src, size);
		src += size;
		size = len - size;
		dst_ptr = (unsigned char *)(getIINCHIP_TxBASE(s));
		wiz_write_buf((unsigned int)dst_ptr, (unsigned char*)src, size);
	} 
	else
	{
		wiz_write_buf((unsigned int)dst_ptr, (unsigned char*)src, len);
	}
}


/**
@brief	This function is being used for copy the data form Receive buffer of the chip to application buffer.

It calculate the actual physical address where one has to read
the data from Receive buffer. Here also take care of the condition while it exceed
the Rx memory uper-bound of unsigned char.
*/
void read_data(unsigned char s, volatile unsigned char * src, volatile unsigned char * dst, unsigned int len)
{
	unsigned int size;
	unsigned int src_mask;
	unsigned char * src_ptr;

	src_mask = (unsigned int)src & getIINCHIP_RxMASK(s);
	src_ptr = (unsigned char *)(getIINCHIP_RxBASE(s) + src_mask);
	
	if( (src_mask + len) > getIINCHIP_RxMAX(s) ) 
	{
		size = getIINCHIP_RxMAX(s) - src_mask;
		wiz_read_buf((unsigned int)src_ptr, (unsigned char*)dst,size);
		dst += size;
		size = len - size;
		src_ptr = (unsigned char *)(getIINCHIP_RxBASE(s));
		wiz_read_buf((unsigned int)src_ptr, (unsigned char*) dst,size);
	} 
	else
	{
		wiz_read_buf((unsigned int)src_ptr, (unsigned char*) dst,len);
	}
}


#ifdef __DEF_IINCHIP_PPP__
#define PPP_OPTION_BUF_LEN 64

unsigned char pppinit_in(unsigned char * id, unsigned char idlen, unsigned char * passwd, unsigned char passwdlen);


/**
@brief	make PPPoE connection
@return	1 => success to connect, 2 => Auth fail, 3 => timeout, 4 => Auth type not support

*/
unsigned char pppinit(unsigned char * id, unsigned char idlen, unsigned char * passwd, unsigned char passwdlen)
{
	unsigned char ret;
	unsigned char isr;
	
	// PHASE0. W5100 PPPoE(ADSL) setup
	// enable pppoe mode
	printf("-- PHASE 0. W5100 PPPoE(ADSL) setup process --\r\n");
	printf("\r\n");
	IINCHIP_WRITE(MR,IINCHIP_READ(MR) | MR_PPPOE);

	// open unsigned char in pppoe mode
	isr = IINCHIP_READ(Sn_IR(0));// first clear isr(0), W5100 at present time
	IINCHIP_WRITE(Sn_IR(0),isr);
	
	IINCHIP_WRITE(PTIMER,200); // 5sec timeout
	IINCHIP_WRITE(PMAGIC,0x01); // magic number
	IINCHIP_WRITE(Sn_MR(0),Sn_MR_PPPOE);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_OPEN);
	
	/* +20071122[chungs]:wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	
	ret = pppinit_in(id, idlen, passwd, passwdlen);

	// close ppp connection unsigned char
	/* +200801 (hwkim) */
	close(0);
	/* ------- */
	
	return ret;
}


unsigned char pppinit_in(unsigned char * id, unsigned char idlen, unsigned char * passwd, unsigned char passwdlen)
{
	unsigned char i = 0;
	unsigned char loop_idx = 0;
	unsigned char isr = 0;
	unsigned char buf[PPP_OPTION_BUF_LEN];
	unsigned int len;
	unsigned char str[PPP_OPTION_BUF_LEN];
	unsigned char str_idx,dst_idx;

   // PHASE1. PPPoE Discovery
	// start to connect pppoe connection
	printf("-- PHASE 1. PPPoE Discovery process --");
	printf(" ok\r\n");
	printf("\r\n");
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCON);
	/* +20071122[chungs]:wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	
	wait_10ms(100);

	loop_idx = 0;
	//check whether PPPoE discovery end or not
	while (!(IINCHIP_READ(Sn_IR(0)) & Sn_IR_PNEXT))
	{
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
			printf("timeout before LCP\r\n"); 
			return 3;
		}
		wait_10ms(100);
	}

   /* +200801[bj] clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/

   // PHASE2. LCP process
	printf("-- PHASE 2. LCP process --");
		
	// send LCP Request
	{
		// Magic number option
		// option format (type value + length value + data)
	   // write magic number value
		buf[0] = 0x05; // type value
		buf[1] = 0x06; // length value
		buf[2] = 0x01; buf[3] = 0x01; buf[4] = 0x01; buf[5]= 0x01; // data
		// for MRU option, 1492 0x05d4  
		// buf[6] = 0x01; buf[7] = 0x04; buf[8] = 0x05; buf[9] = 0xD4;
	}
	send_data_processing(0, buf, 0x06);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); // send request 
	/* +20071122[chungs]:wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
		
	wait_10ms(100);

	while (!((isr = IINCHIP_READ(Sn_IR(0))) & Sn_IR_PNEXT))
	{
		if (isr & Sn_IR_PRECV) // Not support option
		{
   /* +200801[bj] clear interrupt value*/
         IINCHIP_WRITE(Sn_IR(0), Sn_IR_PRECV);
   /*---*/
			len = getSn_RX_RSR(0);
			if ( len > 0 )
			{
				recv_data_processing(0, str, len);
				IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
				/* +20071122[chungs]:wait to process the command... */
				while( IINCHIP_READ(Sn_CR(0)) ) 
					;
				/* ------- */
				
				// for debug
				//printf("LCP proc len = %d\r\n", len); for (i = 0; i < len; i++) printf ("%02x ", str[i]); printf("\r\n");
				// get option length
				len = str[4]; len = ((len & 0x00ff) << 8) + str[5];
				len += 2;
				str_idx = 6; dst_idx = 0; // ppp header is 6 byte, so starts at 6.
				do 
				{
					if ((str[str_idx] == 0x01) || (str[str_idx] == 0x02) || (str[str_idx] == 0x03) || (str[str_idx] == 0x05))
					{
						// skip as length of support option. str_idx+1 is option's length.
						str_idx += str[str_idx+1];
					}
					else
					{
						// not support option , REJECT
						memcpy((unsigned char *)(buf+dst_idx), (unsigned char *)(str+str_idx), str[str_idx+1]);
						dst_idx += str[str_idx+1]; str_idx += str[str_idx+1];
					}
				} while (str_idx != len);
	   			// for debug
	   			//printf("LCP dst proc\r\n"); for (i = 0; i < dst_idx; i++) printf ("%02x ", buf[i]); printf("\r\n");
	   
	   			// send LCP REJECT packet
	   			send_data_processing(0, buf, dst_idx);
	   			IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCJ);
				/* +20071122[chungs]:wait to process the command... */
				while( IINCHIP_READ(Sn_CR(0)) ) 
					;
				/* ------- */
  			}
		}
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
			printf("timeout after LCP\r\n");
			return 3;
		}
		wait_10ms(100);
	}
	printf(" ok\r\n");
	printf("\r\n");

   /* +200801[bj] clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);

   /* +200903[bj] clear receive buffer */
	len = getSn_RX_RSR(0);
	if ( len > 0 )
	{
		recv_data_processing(0, str, len);
		//printf("dummy proc len = %d\r\n", len); for (i = 0; i < len; i++) printf ("%02x ", str[i]); printf("\r\n");
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
		while( IINCHIP_READ(Sn_CR(0)) ) 
			;
	}
   /*---*/

	printf("-- PHASE 3. PPPoE(ADSL) Authentication mode --\r\n");
	printf("Authentication protocol : %.2x %.2x, ", IINCHIP_READ(PATR0), IINCHIP_READ(PATR0+1));

	loop_idx = 0;
	if (IINCHIP_READ(PATR0) == 0xc0 && IINCHIP_READ(PATR0+1) == 0x23)
	{
		printf("PAP\r\n"); // in case of adsl normally supports PAP.
		// send authentication data
		// copy (idlen + id + passwdlen + passwd)
		buf[loop_idx] = idlen; loop_idx++;
		memcpy((unsigned char *)(buf+loop_idx), (unsigned char *)(id), idlen); loop_idx += idlen;
		buf[loop_idx] = passwdlen; loop_idx++;
		memcpy((unsigned char *)(buf+loop_idx), (unsigned char *)(passwd), passwdlen); loop_idx += passwdlen;
		send_data_processing(0, buf, loop_idx);
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR);
		/* +20071122[chungs]:wait to process the command... */
		while( IINCHIP_READ(Sn_CR(0)) ) 
			;
		/* ------- */
		wait_10ms(100);
	}	
	else if (IINCHIP_READ(PATR0) == 0xc2 && IINCHIP_READ(PATR0+1) == 0x23)
	{
		unsigned char chal_len;
		md5_ctx context;
		unsigned char  digest[16];

		len = getSn_RX_RSR(0);
		if ( len > 0 )
		{
			recv_data_processing(0, str, len);
			IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
			/* +20071122[chungs]:wait to process the command... */
			while( IINCHIP_READ(Sn_CR(0)) ) 
				;
			/* ------- */
#ifdef __DEF_IINCHIP_DBG__
			printf("recv CHAP\r\n");
			{
				int16 i;
				
				for (i = 0; i < 32; i++) 
					printf ("%02x ", str[i]);
			}
			printf("\r\n");
#endif
// str is C2 23 xx CHAL_ID xx xx CHAP_LEN CHAP_DATA
// index  0  1  2  3       4  5  6        7 ...

			memset(buf,0x00,64);
			buf[loop_idx] = str[3]; loop_idx++; // chal_id
			memcpy((unsigned char *)(buf+loop_idx), (unsigned char *)(passwd), passwdlen); loop_idx += passwdlen; //passwd
			chal_len = str[6]; // chal_id
			memcpy((unsigned char *)(buf+loop_idx), (unsigned char *)(str+7), chal_len); loop_idx += chal_len; //challenge
			buf[loop_idx] = 0x80;
#ifdef __DEF_IINCHIP_DBG__
			printf("CHAP proc d1\r\n");
			{
				int16 i;
				for (i = 0; i < 64; i++) 
					printf ("%02x ", buf[i]);
			}
			printf("\r\n");
#endif

			md5_init(&context);
			md5_update(&context, buf, loop_idx);
			md5_final(digest, &context);

#ifdef __DEF_IINCHIP_DBG__
			printf("CHAP proc d1\r\n");
			{
				int16 i;				
				for (i = 0; i < 16; i++) 
					printf ("%02x", digest[i]);
			}
			printf("\r\n");
#endif
			loop_idx = 0;
			buf[loop_idx] = 16; loop_idx++; // hash_len
			memcpy((unsigned char *)(buf+loop_idx), (unsigned char *)(digest), 16); loop_idx += 16; // hashed value
			memcpy((unsigned char *)(buf+loop_idx), (unsigned char *)(id), idlen); loop_idx += idlen; // id
			send_data_processing(0, buf, loop_idx);
			IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR);
			/* +20071122[chungs]:wait to process the command... */
			while( IINCHIP_READ(Sn_CR(0)) ) 
				;
			/* ------- */
			wait_10ms(100);
		}
	}
	else
	{
		printf("Not support\r\n");
#ifdef __DEF_IINCHIP_DBG__
		printf("Not support PPP Auth type: %.2x%.2x\r\n",IINCHIP_READ(PATR0), IINCHIP_READ(PATR0+1));
#endif
		return 4;
	}
	printf("\r\n");

	printf("-- Waiting for PPPoE server's admission --");
	loop_idx = 0;
	while (!((isr = IINCHIP_READ(Sn_IR(0))) & Sn_IR_PNEXT))
	{
		if (isr & Sn_IR_PFAIL)
		{
   /* +200801[bj] clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/
			printf("failed\r\nReinput id, password..\r\n");
			return 2;
		}
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
   /* +200801[bj] clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/
			printf("timeout after PAP\r\n");
			return 3;
		}
		wait_10ms(100);
	}
   /* +200801[bj] clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);

   /* +200903[bj] clear receive buffer */
	len = getSn_RX_RSR(0);
	if ( len > 0 )
	{
		recv_data_processing(0, str, len);
		//printf("dummy proc len = %d\r\n", len); for (i = 0; i < len; i++) printf ("%02x ", str[i]); printf("\r\n");
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
		while( IINCHIP_READ(Sn_CR(0)) ) 
			;
	}
   /*---*/

	printf("ok\r\n");
	printf("\r\n");
	printf("-- PHASE 4. IPCP process --");
	// IP Address
	buf[0] = 0x03; buf[1] = 0x06; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00;
	send_data_processing(0, buf, 6);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR);
	/* +20071122[chungs]:wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	wait_10ms(100);

	loop_idx = 0;
	while (1)
	{
		if (IINCHIP_READ(Sn_IR(0)) & Sn_IR_PRECV)
		{
   /* +200801[bj] clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/
			len = getSn_RX_RSR(0);
			if ( len > 0 )
			{
				recv_data_processing(0, str, len);
				IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
				/* +20071122[chungs]:wait to process the command... */
				while( IINCHIP_READ(Sn_CR(0)) ) 
					;
				/* ------- */
	   			//for debug
	   			//printf("IPCP proc len = %d\r\n", len); for (i = 0; i < len; i++) printf ("%02x ", str[i]); printf("\r\n");
	   			str_idx = 6; dst_idx = 0;
	   			if (str[2] == 0x03) // in case of NAK
	   			{
	   				do 
	   				{
	   					if (str[str_idx] == 0x03) // request only ip information
	   					{
	   						memcpy((unsigned char *)(buf+dst_idx), (unsigned char *)(str+str_idx), str[str_idx+1]);
	   						dst_idx += str[str_idx+1]; str_idx += str[str_idx+1];
	   					}
	   					else
	   					{
	   						// skip byte
	   						str_idx += str[str_idx+1];
	   					}
	   					// for debug
	   					//printf("s: %d, d: %d, l: %d", str_idx, dst_idx, len);
	   				} while (str_idx != len);
	   				send_data_processing(0, buf, dst_idx);
	   				IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); // send ipcp request
	   				/* +20071122[chungs]:wait to process the command... */
					while( IINCHIP_READ(Sn_CR(0)) ) 
						;
					/* ------- */
	   				wait_10ms(100);
	   				break;
	   			}
			}
		}
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
			printf("timeout after IPCP\r\n");
			return 3;
		}
		wait_10ms(100);
		send_data_processing(0, buf, 6);
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); //ipcp re-request
		/* +20071122[chungs]:wait to process the command... */
		while( IINCHIP_READ(Sn_CR(0)) ) 
			;
		/* ------- */
	}

	loop_idx = 0;
	while (!(IINCHIP_READ(Sn_IR(0)) & Sn_IR_PNEXT))
	{
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
			printf("timeout after IPCP NAK\r\n");
			return 3;
		}
		wait_10ms(100);
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); // send ipcp request
		/* +20071122[chungs]:wait to process the command... */
		while( IINCHIP_READ(Sn_CR(0)) ) 
			;
		/* ------- */
	}
   /* +200801[bj] clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/
	printf("ok\r\n");
	printf("\r\n");
	return 1;
	// after this function, User must save the pppoe server's mac address and pppoe session id in current connection
}


/**
@brief	terminate PPPoE connection
*/
unsigned char pppterm(unsigned char * mac, unsigned char * sessionid)
{
	unsigned int i;
	unsigned char isr;
#ifdef __DEF_IINCHIP_DBG__
	printf("pppterm()\r\n");
#endif
	/* Set PPPoE bit in MR(Common Mode Register) : enable unsigned char0 pppoe */
	IINCHIP_WRITE(MR,IINCHIP_READ(MR) | MR_PPPOE);
	
	// write pppoe server's mac address and session id 
	// must be setted these value.
	for (i = 0; i < 6; i++) IINCHIP_WRITE((Sn_DHAR0(0)+i),mac[i]);
	for (i = 0; i < 2; i++) IINCHIP_WRITE((Sn_DPORT0(0)+i),sessionid[i]);
	isr = IINCHIP_READ(Sn_IR(0));
	IINCHIP_WRITE(Sn_IR(0),isr);
	
	//open unsigned char in pppoe mode
	IINCHIP_WRITE(Sn_MR(0),Sn_MR_PPPOE);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_OPEN);
	/* +20071122[chungs]:wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	wait_1us(1);
	// close pppoe connection
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PDISCON);
	/* +20071122[chungs]:wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	wait_10ms(100);
	// close unsigned char
	/* +200801 (hwkim) */
	close(0);
	/* ------- */
	

#ifdef __DEF_IINCHIP_DBG__
	printf("pppterm() end ..\r\n");
#endif

	return 1;
}
#endif
