#include "systick.h"
#include "wdt.h"
#include "daliUtils.h"
#include "ethTransceiver.h"
#include "historico.h"

volatile unsigned int _sysTickTimingDelay;
unsigned short int _sysTickPrescaler;

void sysTickTick (void)
{
	
	if (_sysTickPrescaler>0)
	{
		_sysTickPrescaler--;
	}
	else
	{
		_sysTickPrescaler=4;
	
	if (_sysTickTimingDelay!= 0x00)
  { 
    _sysTickTimingDelay--;
  }
	
	ethTransceiverTick();
	ethICMPTick();
	daliUtilsTick ();
	historicoTick();
	botoneraTick();
	groupsTick();
	sensoresSysTick ();
	}
}






void sysTickDelay (volatile unsigned int nTime)
{
	_sysTickTimingDelay = nTime;
  while(_sysTickTimingDelay != 0)
	{
		wdtDoTask();
	}		
}
