	#include "dali.h"
	#include "config.h"

#define NOT_SENDING         0x00
#define SENDING             0x01
#define SENT                0x02
#define TRUE                0x01
#define FALSE               0x00
#define NOT_STARTED         0x00
#define IN_PROGRESS         0x01
#define DONE                0x02
#define BOOL                unsigned char	
	
#define STAGE_0             0x00
#define STAGE_1             0x01
#define STAGE_2             0x02
#define STAGE_3             0x03
#define STAGE_4             0x04
	
extern unsigned char _dAQCGLDireccionesOcupadasMatriz[64];
extern unsigned char _dAQCGLDireccionesOcupadasCnt;
extern unsigned char _dAQDTLTiposDispositivoMatriz[64];
extern unsigned char _dAQDTLTiposDispositivoValid[8];
extern unsigned char _dAQCGLStatus;
extern unsigned char _dAQDTLStatus;
unsigned char 			 _dAQGroupsLowStatus;
unsigned char 			 _dAQGroupsHighStatus;
unsigned char				 _dALSStatus;
unsigned char				 _dALSStage;



	
void dALSDoTask(void)
{
		unsigned char applyDT[8];	
		if (_dALSStatus==IN_PROGRESS)
		{
			if (_dALSStage==STAGE_0)
			{	
				if (_dAQCGLStatus==NOT_STARTED) 		dAQCGLStart();
				if (_dAQCGLStatus==DONE) 
				{			
					configDaliLoopInstalledSet(&_dAQCGLDireccionesOcupadasMatriz[0],0x00);
					_dALSStage=STAGE_1;
				}		
			}						
			if (_dALSStage==STAGE_1)
			{	
				if (_dAQDTLStatus==NOT_STARTED)			dAQDTLStart(&_dAQCGLDireccionesOcupadasMatriz[0]);
				if (_dAQDTLStatus==DONE)
				{					
					configDaliLoopDeviceTypeSet (&_dAQCGLDireccionesOcupadasMatriz[0], &_dAQDTLTiposDispositivoMatriz[0], 0x01);			
					_dALSStage=STAGE_2;			
				}						
			}														
			
			if (_dALSStage==STAGE_2)
			{	
				if (_dAQGroupsLowStatus==NOT_STARTED)				dAQGroupsLowSend();
				if (_dAQGroupsLowStatus==DONE)
				{					
					_dALSStage=STAGE_3;			
				}						
			}	
			
			if (_dALSStage==STAGE_3)
			{	
				if (_dAQGroupsHighStatus==NOT_STARTED)			dAQGroupsHighSend();
				if (_dAQGroupsHighStatus==DONE)
				{					
					_dALSStage=STAGE_4;			
				}						
			}				
			
			if (_dALSStage==STAGE_4)
			{	
				daliAccessHandlerAddrUnlock (A_HANDLER_ADDR_LINE_SCANNING);
				daliAddressingNotify(0x01,0x02,0x01,0xFF,0xFF,_dAQCGLDireccionesOcupadasCnt);	
				_dALSStatus=DONE;
			}
		}
}
	
	
	
BOOL dALSStart(void)
{
	unsigned char retVal;	
	if (_dALSStatus!=IN_PROGRESS)
	{
		if (daliAccessHandlerAddrLock (A_HANDLER_ADDR_LINE_SCANNING)==0x01)
		{			
			dAQCGLDoneFlagClear();
			dAQDTLDoneFlagClear();
			
			_dAQGroupsLowStatus=NOT_STARTED;
			_dAQGroupsHighStatus=NOT_STARTED;
			
			_dALSStatus=IN_PROGRESS;
			_dALSStage=STAGE_0;
			retVal=TRUE;
		}
		else
		{
			retVal=FALSE;
			_dALSStatus=DONE;
		}
	}
	else
	{
		retVal=FALSE;
		_dALSStatus=DONE;
	}	
	return retVal;	
}

unsigned int groupticker=20000;
void groupsTick()
{
	if(groupticker>0) {groupticker--;}
}
unsigned char daliGroupSend = 0;
void groupsDoTask()
{
	if(groupticker==0)
	{  
		//dALSStart();
		polling_TmpGroupDisable();
		unsigned char apply[8];
		daliLoopActualLevelApplyGet (&apply[0]);
		if (daliGroupSend==0)
		{
			if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x07, 0xFF, &apply[0], 0x2E, 0xC0)==0x01)
			{
					_dAQGroupsLowStatus=SENDING;
			}
			groupticker=300;
			daliGroupSend=1;
		}
		else
		{
			if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x07, 0xFF, &apply[0], 0x2E, 0xC1)==0x01)
			{
					_dAQGroupsHighStatus=SENDING;
			}
			groupticker=20000;
			daliGroupSend=0;
			polling_EndTmpDisable();
		}
	}
}
	
void dALSDoneFlagClear (void)
{
	_dALSStatus=NOT_STARTED;	
}