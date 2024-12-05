		#include "dali.h"
				
#define DAQCGL_TIMEOUT			15
#define NOK         				0x00
#define OK         					0x01
#define NA         					0xFF		
#define ERR_TIMEOUT					0x02			
#define TRUE                0x01
#define FALSE               0x00
#define NOT_SENDING         0x00
#define SENDING             0x01
#define SENT                0x02
#define NOT_STARTED         0x00
#define IN_PROGRESS         0x01
#define DONE                0x02

		#define STAGE_0             0x00
		#define STAGE_1             0x01



#define BOOL                unsigned char						

// External.
extern unsigned char 		_dAQueryDTSent;
// Internal.

unsigned char 					_dAQDTLStatus;	
unsigned char						_dAQDTLTiposDispositivoMatriz[64];
unsigned char						_dAQDTLTiposDispositivoValid[8];

unsigned char						*_applyDT;
unsigned char _dAQDTLCurrentAddress;

void dAQDTLDoneFlagClear (void)
{
	_dAQDTLStatus=NOT_STARTED;
}		

BOOL dAQDTLStart (unsigned char *applyDT)
{
	BOOL retVal;		
	unsigned char n;
	if (_dAQDTLStatus!=IN_PROGRESS)
	{				
		if (daliAccessHandlerAddrLock (A_HANDLER_ADDR_QUERY_DEVICE_TYPE)==0x01)
		{								
			for (n=0;n<64;n++)
			{
				_dAQDTLTiposDispositivoMatriz[n]=0xFF;
			}
			_dAQueryDTSent=NOT_SENDING;				// Borrar todas las variables relacionadas con el envío de comandos.				
			_dAQDTLCurrentAddress=0;
			_dAQDTLStatus=IN_PROGRESS;		
			retVal=TRUE;
		}
		else
		{
			retVal=FALSE;
			_dAQDTLStatus=DONE;
		}
	}
	else
	{
		retVal=FALSE;
		_dAQDTLStatus=DONE;
	}
	return retVal;
}						




void dAQDTLDoTask(void)
{
	unsigned char apply[8];	
	if (_dAQDTLStatus==IN_PROGRESS) 
	{
		if (_dAQDTLCurrentAddress<63)
		{
			if (daliLoopInstalledGet(_dAQDTLCurrentAddress)==1)
			{
				if (_dAQueryDTSent==NOT_SENDING)			
				{		
					daliAddressingNotify(0x01,0x01,0xFF,0xFF,0x03,0xFF);
					dAQueryDTSend(_dAQDTLCurrentAddress);					
				}
				else if (_dAQueryDTSent==SENT)			
				{
					_dAQueryDTSent=NOT_SENDING;
					_dAQDTLCurrentAddress++;
				}				
			}			
			else
			{
				_dAQDTLCurrentAddress++;
			}
		}	
		else
		{
			daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_QUERY_DEVICE_TYPE);
			_dAQDTLStatus=DONE;
		}
	}
	
	
	
	
	
	
	/*
	
	
	if (_dAQDTLStatus==IN_PROGRESS) 
	{
			if (_dAQueryDTLoopSent==NOT_SENDING)			
			{				
				daliAddressingNotify(0x01,0x01,0xFF,0xFF,0x03,0xFF);
				daliLoopInstalledArrayByteGet (&apply[0]);				
				dAQueryDTLoopSend(&apply[0]);					
			}
			if (_dAQueryDTLoopSent==SENT)						
			{
				daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_QUERY_DEVICE_TYPE);
				_dAQDTLStatus=DONE;
			}						
	}			
	*/
}
