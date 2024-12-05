		#include "dali.h"
		


		#define NOK         				0x00
		#define OK         					0x01
		#define NA         					0xFF		
		#define ERR_NE       				0x00
		#define ERR_TOO_MUCH				0x01		
		#define ERR_TIMEOUT					0x02			
		#define ALL_DEVICES         0x00
		#define NEW_DEVICES         0xFF
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
		#define STAGE_2             0x02
		#define STAGE_3             0x03
		#define STAGE_4             0x04
		#define STAGE_5             0x05
		#define STAGE_6             0x06
		#define STAGE_7             0x07
		#define STAGE_8             0x08
		#define STAGE_9             0x09
		#define STAGE_10            0x0A
		#define STAGE_11            0x0B
		#define STAGE_12            0x0C
		#define STAGE_13            0x0D
		#define STAGE_14            0x0E
		#define STAGE_15            0x0F

		#define BOOL                unsigned char

unsigned char									_dANIOpSRAReintentosCnt;
unsigned char									_dANIOpPSAReintentosCnt;		
unsigned char 								_dANIStatus;
unsigned char 								_dANIStage;
unsigned short int						_dANITimeout;
	
extern unsigned char 					_dASRAStatus;			
extern unsigned char					_dASRAResult;			
extern unsigned char 					_dASRAEncontradosCnt;
extern unsigned char 					_dAPSAStatus;	
extern unsigned char					_dADSAStatus;
extern unsigned char					_dAQCGLDireccionesOcupadasMatriz[64];
extern unsigned char					_newInitType;
extern unsigned char 					_ipA[4];

static void dANIGotoStage (unsigned char stage)
{		
	dADSADoneFlagClear();	
	dASRADoneFlagClear();		
	dAPSADoneFlagClear();	
	_dANIStage=stage;
}

void dANIRtcTick (void)
{
	if (_dANIStatus==IN_PROGRESS)
	{
		if (_dANITimeout>0)
		{
			_dANITimeout--;
		}
		else
		{
			daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_NEW_INITIALIZATION);
			//_dANIResult=NOK;
			//_dANIExitCode=ERR_TIMEOUT;											     
			_dANIStatus=DONE;
		}		
	}
}

BOOL dANIStart (void)
{
	unsigned char retVal;
	if (_dANIStatus!=IN_PROGRESS)
	{		
		if (daliAccessHandlerAddrLock (A_HANDLER_ADDR_NEW_INITIALIZATION)==0x01)
		{			
				_dANIStatus=IN_PROGRESS;
		
				_dANITimeout=840;
		
				_dANIOpSRAReintentosCnt=3;
				_dANIOpPSAReintentosCnt=3;
				dANIGotoStage (STAGE_0);
				retVal=TRUE;
		}
		else
		{
			_dANIStatus=DONE;			
			retVal=FALSE;
		}
	}
	else
	{
		_dANIStatus=DONE;			
		retVal=FALSE;		
	}
	return retVal;
}

void dANIStop (void)
{
}

void dANIDoTask (void)
{		
	unsigned char n;	
	if (_dANIStatus==IN_PROGRESS)
	{	
		if (_dANIStage==STAGE_0)
		{
			dANIGotoStage(STAGE_1);
		}		
		if (_dANIStage==STAGE_1)
		{
			if (_dADSAStatus==NOT_STARTED)				dADSAStart();
			if (_dADSAStatus==DONE)
			{
				dANIGotoStage(STAGE_2);
			}		
		}		
		if (_dANIStage==STAGE_2)
		{
			if (_dASRAStatus==NOT_SENDING)				dASRAStart(ALL_DEVICES,TRUE);	
			if (_dASRAStatus==DONE)
			{
				if (_dASRAResult==OK)
				{
					if (_dASRAEncontradosCnt>0)
					{						
						dANIGotoStage(STAGE_3);			
					}
					else
					{
						dANIGotoStage(STAGE_5);	
					}
				}
				else
				{					
					if (_dANIOpSRAReintentosCnt>0)
					{
						_dANIOpSRAReintentosCnt--;
						dANIGotoStage(STAGE_0);			
						daliAddressingDebug(0xAA,0xFF,0xFF,0xFF,_dANIOpSRAReintentosCnt);						
					}
					else
					{
						dANIGotoStage(STAGE_5);	
						daliAddressingDebug(0xAB,0xFF,0xFF,0xFF,0xFF);
					}				
				}				
			}	
		}
		if (_dANIStage==STAGE_3)
		{	
			if (_dAPSAStatus==NOT_SENDING) 				
			{
				for (n=0;n<64;n++)
				{
					_dAQCGLDireccionesOcupadasMatriz[n]=0;
				}				
				dAPSAStart(ALL_DEVICES, _dASRAEncontradosCnt);
			}
			if (_dAPSAStatus==DONE)
			{
				dANIGotoStage(STAGE_4);
			}		
		}		
		if (_dANIStage==STAGE_4)
		{	
			dANIGotoStage(STAGE_5);
		}		
		if (_dANIStage==STAGE_5)
		{			
			daliAddressingNotify(0x01,0x02,0x01,0xFF,0xFF,_dASRAEncontradosCnt);			
			
			daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_NEW_INITIALIZATION);

			if (_newInitType==0x01)		
			{
				sensorScanningStart (&_ipA[0]);
			}
			
			
			_dANIStatus=DONE;			
			
			
			
		}
	}	
}

