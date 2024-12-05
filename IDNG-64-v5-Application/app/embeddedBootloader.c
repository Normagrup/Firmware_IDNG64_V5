#include "embeddedBootloader.h"
#include "m25p.h"

unsigned int _embeddedBootloaderCurrentPageCnt;

unsigned char _embeddedBootloaderWriteStart;
unsigned char _embeddedBootloaderWrite;
unsigned char _embeddedBootloaderReadStart;
unsigned char _embeddedBootloaderRead;


unsigned char _eBootloaderTransceiver_Pending;
unsigned int _eBootloaderTransceiver_CurrentPageCnt;
unsigned char _eBootloaderTransceiver_WriteStart;
unsigned char _eBootloaderTransceiver_Write;
unsigned char _eBootloaderTransceiver_ReadStart;
unsigned char _eBootloaderTransceiver_Read;







void embeddedBootloaderWrite (unsigned char *buffer)
{
		unsigned char status;			
		m25pWriteEnable();
		m25pWriteBytes(0x05,_embeddedBootloaderCurrentPageCnt,0,&buffer[0],256);	
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);	
		if (_embeddedBootloaderCurrentPageCnt<256)
		{
			_embeddedBootloaderCurrentPageCnt++;
		}
		_embeddedBootloaderWrite=0x01;
}

void embeddedBootloaderWriteStart (void)
{
		unsigned char status;		
		m25pWriteEnable();
		m25pSectorErase (5,0,0);
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);		
		_embeddedBootloaderCurrentPageCnt=0;
		
		_embeddedBootloaderWriteStart=0x01;
		_embeddedBootloaderWrite=0x00;
		_embeddedBootloaderReadStart=0x00;
		_embeddedBootloaderRead=0x00;
}

void embeddedBootloaderReadStart (void)
{
	_embeddedBootloaderCurrentPageCnt=0;
	_embeddedBootloaderReadStart=0x01;
}

void embeddedBootloaderRead (unsigned char *buffer)
{
	m25pReadBytes(0x05,_embeddedBootloaderCurrentPageCnt,0,&buffer[0],256);		
	if (_embeddedBootloaderCurrentPageCnt<256)
	{
		_embeddedBootloaderCurrentPageCnt++;
	}
	
	_embeddedBootloaderRead=0x01;
}

unsigned char embeddedBootloaderGo (void)
{
	unsigned char answer;
	// TODO
	/*
	if ((_embeddedBootloaderWriteStart==0x01) && (_embeddedBootloaderWrite==0x01) && (_embeddedBootloaderReadStart==0x01) && (_embeddedBootloaderRead==0x01))
	{
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xAAAAAAAA);	
		answer=1;
	}
	else
	{
		answer=0;
	}
	*/
	return answer;
}


void embeddedBootloaderInit (void)
{
	// TODO
	/*
	if (RTC_ReadBackupRegister(RTC_BKP_DR0)==0xAAAAAAAA)
	{
		RTC_WriteBackupRegister(RTC_BKP_DR0,0);		
	}	
	
	if (RTC_ReadBackupRegister(RTC_BKP_DR1)==0xAAAAAAAA)
	{
		RTC_WriteBackupRegister(RTC_BKP_DR1,0);	
		_eBootloaderTransceiver_Pending=1;	
	}	
	*/
	
	
	
	
}





void eBootloaderTransceiver_Write (unsigned char *buffer)
{
		unsigned char status;			
		m25pWriteEnable();
		m25pWriteBytes(0x01,_eBootloaderTransceiver_CurrentPageCnt,0,&buffer[0],256);	
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);	
		if (_eBootloaderTransceiver_CurrentPageCnt<256)
		{
			_eBootloaderTransceiver_CurrentPageCnt++;
		}
		_eBootloaderTransceiver_Write=0x01;
}

void eBootloaderTransceiver_WriteStart (void)
{	
		unsigned char status;		
		m25pWriteEnable();
		m25pSectorErase (1,0,0);
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);		
		_eBootloaderTransceiver_CurrentPageCnt=0;
		_eBootloaderTransceiver_WriteStart=0x01;
		_eBootloaderTransceiver_Write=0;
		_eBootloaderTransceiver_ReadStart=0;
		_eBootloaderTransceiver_Read=0;
}

void eBootloaderTransceiver_Read (unsigned char *buffer)
{
	m25pReadBytes(0x01,_eBootloaderTransceiver_CurrentPageCnt,0,&buffer[0],256);		
	if (_eBootloaderTransceiver_CurrentPageCnt<256)
	{
		_eBootloaderTransceiver_CurrentPageCnt++;
	}
	
	_eBootloaderTransceiver_Read=0x01;
}

void eBootloaderTransceiver_ReadStart (unsigned char *buffer)
{
	_eBootloaderTransceiver_CurrentPageCnt=0;
	_eBootloaderTransceiver_ReadStart=0x01;
}




void eBootloaderTransceiver_Check (void)
{
	unsigned char byteCnt;
	unsigned char pageCnt;	
	unsigned short int suma1;
	unsigned char buffer1[128];
	unsigned short int suma2;
	unsigned char buffer2[128];
	if (_eBootloaderTransceiver_Pending==1)
	{				
		boardAVRBootON();		
		boardLedRojoON();
		boardLedVerdeON();
		sysTickDelay (4000);			//Con 900 funciona.		
		boardLedRojoOFF();
		boardLedVerdeOFF();
		
		for (pageCnt=0;pageCnt<112;pageCnt++)
		{
			suma1=0;			
			suma2=0;			
			m25pReadBytes(0x01,pageCnt,0,&buffer1[0],128);		
			m25pReadBytes(0x01,pageCnt,128,&buffer2[0],128);							
			boardLedRojoON();
			boardLedVerdeON();		
			for (byteCnt=0;byteCnt<128;byteCnt++)
			{
				spi1Write(0xFB, buffer1[byteCnt]);
				suma1=suma1+buffer1[byteCnt];
			}		
			spi1Write(0xFB, (2*pageCnt));
			spi1Write(0xFB, ((unsigned char)(suma1%256)));
			sysTickDelay (12);	//Con 2 ya funciona.			
			boardLedRojoOFF();
			boardLedVerdeOFF();			
			for (byteCnt=0;byteCnt<128;byteCnt++)
			{
				spi1Write(0xFB, buffer2[byteCnt]);
				suma2=suma2+buffer2[byteCnt];
			}		
			spi1Write(0xFB, ((2*pageCnt)+1));
			spi1Write(0xFB, ((unsigned char)(suma2%256)));
			sysTickDelay (12); //Con 2 ya funciona.
		}		
		_eBootloaderTransceiver_Pending=0;	
		boardAVRBootOFF();		
	}	
	else
	{
		boardAVRBootOFF();		
	}
}







unsigned char eBootloaderTransceiver_Go (void)
{
	unsigned char answer;
	// TODO
	/*
	if ((_eBootloaderTransceiver_WriteStart==0x01) && (_eBootloaderTransceiver_Write==0x01) && (_eBootloaderTransceiver_ReadStart==0x01) && (_eBootloaderTransceiver_Read==0x01))
	{
		RTC_WriteBackupRegister(RTC_BKP_DR1,0xAAAAAAAA);	
		answer=1;
	}
	else
	{
		answer=0;
	}	
	*/
	
	return answer;
}











