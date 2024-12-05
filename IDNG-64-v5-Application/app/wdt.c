#include "fallos.h"
unsigned char _wdtReboot;
unsigned char _wdtTickCnt;

void wdtReboot (void)
{
	if (_wdtReboot==0x00)
	{	
		boardWdtDoTask();
		_wdtReboot=0x01;
	}
}

void wdtTick (void)
{
	if (_wdtTickCnt>0)	_wdtTickCnt--;
}

void wdtReloadEnable (void)
{
	_wdtTickCnt=30;	
}
	
void wdtInit (void)
{		
	// TODO	
}

void wdtDoTask (void)
{		
	if (_wdtReboot==0)
	{	
		boardWdtDoTask();
	}
}

void wdtReload (void)
{
	if (_wdtTickCnt>0)	wdtDoTask ();
}


