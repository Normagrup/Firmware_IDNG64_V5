#include "dali.h"

#define NOT_SENDING         0x00
#define SENDING             0x01
#define SENT                0x02
#define TRUE                0x01
#define FALSE               0x00
#define NOT_STARTED         0x00
#define IN_PROGRESS         0x01
#define DONE                0x02
#define BOOL                unsigned char	

// External.
extern unsigned char		_dADTRSent;		
extern unsigned char		_dAStoreDTRAsAddrSent;

// Internal.
unsigned char 					_dADSAStatus;			
unsigned char						_dADSATimeout;


BOOL dADSAStart (void)
{	
	BOOL retVal;
	if (_dADSAStatus!=IN_PROGRESS)
	{	
		if (daliAccessHandlerAddrLock (A_HANDLER_ADDR_DELETE_SHORT_ADDRESS)==0x01)
		{			
			_dADTRSent=NOT_SENDING;
			_dAStoreDTRAsAddrSent=NOT_SENDING;			
			_dADSAStatus=IN_PROGRESS;			
			retVal=TRUE;			
		}
		else
		{
			retVal=FALSE;
			_dADSAStatus=DONE;
		}
	}
	else
	{
		retVal=FALSE;
		_dADSAStatus=DONE;
	}
	return retVal;
}


void dADSADoTask (void)
{
	if (_dADSAStatus==IN_PROGRESS)
	{
		if (_dADTRSent==NOT_SENDING)	dADTRSend(0xFF);
		if (_dADTRSent==SENT)					dAStoreDTRAsAddrSend(0xFF);		
		if (_dAStoreDTRAsAddrSent==SENT)
		{		
			daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_DELETE_SHORT_ADDRESS);
			_dADSAStatus=DONE;
		}
	}	
}

void dADSADoneFlagClear (void)
{
	_dADSAStatus=NOT_STARTED;
}
