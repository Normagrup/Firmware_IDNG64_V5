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
#define BOOL                unsigned char						

// External.
extern unsigned char 		_dAQueryCGLoopSent;
// Internal.
unsigned char 					_dAQCGLStatus;			
unsigned char						_dAQCGLDireccionesOcupadasMatriz[64];
unsigned char						_dAQCGLDireccionesOcupadasCnt;
unsigned char 					_dAQCGLStatus;		
unsigned char						_dAQCGLTimeout;
unsigned char						_dAQCGLResult;
unsigned char						_dAQCGLExitCode;

void dAQCGLDoneFlagClear (void)
{
	_dAQCGLStatus=NOT_STARTED;
}		
void dAQCGLRtcTick(void)
{
	if (_dAQCGLStatus==IN_PROGRESS)
	{
		if (_dAQCGLTimeout>0)
		{
			_dAQCGLTimeout--;
		}
		else
		{
			daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_QUERY_CONTROL_GEAR);			
			_dAQCGLResult=NOK;
			_dAQCGLExitCode=ERR_TIMEOUT;
			_dAQCGLStatus=DONE;
		}
	}
}

BOOL dAQCGLStart (void)
{
	BOOL retVal;		
	unsigned char n;
	if (_dAQCGLStatus!=IN_PROGRESS)
	{		
		if (daliAccessHandlerAddrLock (A_HANDLER_ADDR_QUERY_CONTROL_GEAR)==0x01)
		{								
			_dAQCGLDireccionesOcupadasCnt=0;
			for (n=0;n<64;n++)
			{
				_dAQCGLDireccionesOcupadasMatriz[n]=FALSE;
			}
			_dAQueryCGLoopSent=NOT_SENDING;				// Borrar todas las variables relacionadas con el envío de comandos.			
			_dAQCGLTimeout=DAQCGL_TIMEOUT;
			_dAQCGLStatus=IN_PROGRESS;	
			retVal=TRUE;
		}
		else
		{
			retVal=FALSE;
			_dAQCGLStatus=DONE;
		}
	}
	else
	{
		retVal=FALSE;							
		_dAQCGLStatus=DONE;
	}
	return retVal;
}						
						
void dAQCGLDoTask(void)
{
	if (_dAQCGLStatus==IN_PROGRESS) 
	{
		if (_dAQueryCGLoopSent==NOT_SENDING)				
		{
			daliAddressingNotify(0x01,0x01,0xFF,0xFF,0x01,0xFF);
			dAQueryCGLoopSend();					
		}
		if (_dAQueryCGLoopSent==SENT)						
		{
			daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_QUERY_CONTROL_GEAR);
			_dAQCGLStatus=DONE;
			_dAQCGLResult=OK;
			_dAQCGLExitCode=NA;
		}						
	}			
}
