#include "daliCommands.h"
#include "dali.h"
#include "daliLoop.h"
#include "rtcc.h"
#include "m25p.h"
#include "test.h"
#include "config.h"
unsigned char testFtPendiente[16];
unsigned char testFtCnt[16];
unsigned char testDtPendiente[16];
unsigned char testDtCnt[16];
volatile unsigned char testOverflow;

void testDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)									
{
	unsigned char _n;
	if ((daliSentCommandHigh==0x2C) && (daliSentCommandLow==0xE3))
	{
		for (_n=0;_n<16;_n++)
		{
			if ((testFtPendiente[_n]==0x01) && (((_n<<1)|0x81)==daliSentDest))
			{
				testFtPendiente[_n]=0;
				polling_PollEmergencyStatus(0x00);
			}
		}
	}
	
	if ((daliSentCommandHigh==0x2C) && (daliSentCommandLow==0xE4))
	{
		for (_n=0;_n<16;_n++)
		{
			if ((testDtPendiente[_n]==0x01) && (((_n<<1)|0x81)==daliSentDest))
			{
				testDtPendiente[_n]=0;
				polling_PollEmergencyStatus(0x01);
			}
		}
	}
}







void testSet (unsigned char grupo, unsigned char activo, unsigned char *testTime)
{
	unsigned char testData[8];	
	if (grupo<16)
	{	
		testData[0]=activo;
		testData[1]=testTime[0];
		testData[2]=testTime[1];
		testData[3]=testTime[2];
		testData[4]=testTime[3];
		testData[5]=testTime[4];
		testData[6]=testTime[5];
		testData[7]=testTime[6];					
		configTestSet(grupo,&testData[0],0x01);	
	}	
}

unsigned char testGet (unsigned char grupo, unsigned char *testTime)
{
	unsigned char testData[8];	
	configTestGet(grupo,&testData[0]);
	if (testData[0]==0x01)
	{
		testTime[0]=testData[1];
		testTime[1]=testData[2];
		testTime[2]=testData[3];
		testTime[3]=testData[4];
		testTime[4]=testData[5];
		testTime[5]=testData[6];
		testTime[6]=testData[7];
		return 0x01;
	}	
	else
	{
		return 0x00;
	}
}


void testRTCC_Tick (void)
{
	unsigned char _rtccWeekday;
	unsigned char _rtccDay;
	unsigned char _rtccMonth;
	unsigned char _rtccHour;
	unsigned char _rtccMinutes;
	unsigned char _n;	
	unsigned char testData[8];	
	Date date;
	Time time;
	date=rtccDateGet();
	time=rtccTimeGet();		
	_rtccWeekday=date.weekday;
	_rtccDay=date.day;
	_rtccMonth=date.month;
	_rtccHour=time.hours;
	_rtccMinutes=time.minutes;
	for (_n=0;_n<16;_n++)
	{
		configTestGet(_n,&testData[0]);
		if (testData[0]==0x01)
		{
			if ((_rtccWeekday==testData[1]) && (_rtccHour==testData[2]) && (_rtccMinutes==testData[3]))
			{
				testFtPendiente[_n]=0x01;
				testFtCnt[_n]=10;
			}
			if ((_rtccMonth==testData[4]) && (_rtccDay==testData[5]) && (_rtccHour==testData[6]) && (_rtccMinutes==testData[7]))
			{
				testDtPendiente[_n]=0x01;
				testDtCnt[_n]=10;
			}			
		}	
	}
	testOverflow=0x01;
}




void testInit (void)
{
	unsigned char n;
	for (n=0;n<16;n++)	testFtPendiente[n]=0x00;
	for (n=0;n<16;n++)	testFtCnt[n]=0x00;
	for (n=0;n<16;n++)	testDtPendiente[n]=0x00;	
	for (n=0;n<16;n++)	testDtCnt[n]=0x00;
	testOverflow=0x00;
}
void testDoTask (void)
{
	unsigned char _n;
	unsigned char apply[8];
	
	if (testOverflow==0x01)
	{
	for (_n=0;_n<16;_n++)
	{
		if (testFtPendiente[_n]==0x01)
		{
			if (testFtCnt[_n]>0)
			{			
				if (daliSend(DALI_TEST, 0xFFFF, 0x01, ((_n<<1)|0x81), &apply[0], 0x2C, 0xE3)==0x01)
				{				
				}
				else
				{
				}	
				testFtCnt[_n]--;
			}
			else
			{
				// No se ha podido enviar.
				testFtPendiente[_n]=0;
			}	
		}		
		if (testDtPendiente[_n]==0x01)
		{
			if (testDtCnt[_n]>0)
			{			
				if (daliSend(DALI_TEST, 0xFFFF, 0x01, ((_n<<1)|0x81), &apply[0], 0x2C, 0xE4)==0x01)
				{				
				}
				else
				{
				}	
				testDtCnt[_n]--;
			}
			else
			{
				// No se ha podido enviar.
				testDtPendiente[_n]=0;
			}	
		}	
	}		
		testOverflow=0x00;
	}	
}

