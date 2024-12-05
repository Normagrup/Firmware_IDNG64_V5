/*

							 P O L L I N G . C 
							 
							 14/02/2019.
							 
							 DIEGO LUIS FERNÁNDEZ HACES @ NORMAGRUP TECHNOLOGY.

*/

#include "dali.h"
#include "polling.h"
#include "daliLoop.h"
#include "historicoTest.h"
#include "utils.h"
#include "daliSensores.h"
#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT

#define POLLING_QUERY_STATUS																				0
#define POLLING_QUERY_ACTUAL_LEVEL																	1
#define POLLING_QUERY_EMERGENCY_MODE																2
#define POLLING_QUERY_FAILURE_STATUS																3
#define POLLING_QUERY_EMERGENCY_STATUS															4
#define POLLING_QUERY_STATUS_SKIP_CNT_DEFAULT_VALUE									4
#define POLLING_QUERY_ACTUAL_LEVEL_SKIP_CNT_DEFAULT_VALUE						4
#define POLLING_QUERY_FAILURE_STATUS_SKIP_CNT_DEFAULT_VALUE					8
#define POLLING_QUERY_EMERGENCY_MODE_SKIP_CNT_DEFAULT_VALUE					0
#define POLLING_QUERY_EMERGENCY_STATUS_SKIP_CNT_DEFAULT_VALUE				16
#define POLLING_QUERY_STATUS_SKIP_CNT_FOCUS_VALUE										0
#define POLLING_QUERY_ACTUAL_LEVEL_SKIP_CNT_FOCUS_VALUE							0
#define POLLING_QUERY_FAILURE_STATUS_SKIP_CNT_FOCUS_VALUE						0
#define POLLING_QUERY_EMERGENCY_MODE_SKIP_CNT_FOCUS_VALUE						0
#define POLLING_QUERY_EMERGENCY_STATUS_SKIP_CNT_FOCUS1_VALUE				0
#define POLLING_QUERY_EMERGENCY_STATUS_SKIP_CNT_FOCUS2_VALUE				8
#define POLLING_TMP_DISABLE_TIMEOUT																	900
#define POLLING_POLL_QUERY_FAILURE_STATUS_CNT_VALUE									10
#define POLLING_POLL_QUERY_ACTUAL_LEVEL_CNT_VALUE										10
#define POLLING_POLL_QUERY_EMERGENCY_MODE_LEVEL_CNT_VALUE						10
#define POLLING_POLL_QUERY_EMERGENCY_STATUS_CNT_FT_VALUE						120
#define POLLING_POLL_QUERY_EMERGENCY_STATUS_CNT_DT_VALUE						21600
#define POLLING_BUSY_CNT_MAX 																				30
#define POLLING_BUSY_CLEAR_CNT_VALUE																0

unsigned char 			_polling_LastOpIncomplete;

unsigned char				_polling_BusyCnt;

unsigned char 			_polling_SkipCnt[5][2];
unsigned char 			_polling_LastOpIncompleteApplyBackup[8];
unsigned char 			_polling_LastOpIncompleteSentBackup[8];
unsigned short int	_polling_TmpDisableTimeoutCnt;
unsigned char 			_polling_Cnt;
unsigned char 			_polling_PollActualLevelCnt;
unsigned char 			_polling_PollEmergencyModeCnt;
unsigned short int	_polling_PollEmergencyStatusCnt;
//unsigned char 			_polling_PollFailureStatusCnt;
unsigned char 			_polling_NextCnt=0xFF;

static void 	polling_CntInc 							(void)
{	
	unsigned char ptr;		
	if (_polling_NextCnt==0xFF)
	{		
		if (_polling_Cnt==0x00)	_polling_SkipCnt[1][0]=0;				// Query Actual Level Después de Query Status.
		ptr=_polling_Cnt;	
		if (ptr<4)
		{
			ptr++;
		}
		else
		{
			ptr=0;
		}		
		while (_polling_SkipCnt[ptr][0]>0)
		{				
			_polling_SkipCnt[ptr][0]--;		
			if (ptr<4)
			{
				ptr++;
			}
			else
			{
				ptr=0;
			}		
		}	
		_polling_SkipCnt[ptr][0]=_polling_SkipCnt[ptr][1];			
		_polling_Cnt=ptr;	
	}
	else
	{
		_polling_Cnt=_polling_NextCnt;
		_polling_NextCnt=0xFF;
	}	
}
static void 	polling_IncompleteCheck 		(unsigned char *apply)
{
	unsigned char n;
	if (_polling_LastOpIncomplete==_polling_Cnt)
	{
		for (n=0;n<8;n++)	apply[n]=_polling_LastOpIncompleteSentBackup[n]&(~(_polling_LastOpIncompleteApplyBackup[n]));		
	}
	_polling_LastOpIncomplete=0xFF;
}
static void		polling_IncompleteSet				(unsigned char *sentApply, unsigned char *validArray, unsigned char operation)
{
	unsigned char n;
	if (_polling_LastOpIncomplete==0xFF)	
	{				
		for (n=0;n<8;n++)		_polling_LastOpIncompleteSentBackup[n]=sentApply[n];
		for (n=0;n<8;n++)		_polling_LastOpIncompleteApplyBackup[n]=validArray[n];
		_polling_LastOpIncomplete=operation;					
	}	
}
void 					polling_Tick 								(void)
{
	if (_polling_TmpDisableTimeoutCnt>0)			_polling_TmpDisableTimeoutCnt--;
	if (_polling_BusyCnt>0)										_polling_BusyCnt--;
	if (_polling_PollActualLevelCnt>0)				_polling_PollActualLevelCnt--;
	if (_polling_PollEmergencyModeCnt>0)			_polling_PollEmergencyModeCnt--;
	if (_polling_PollEmergencyStatusCnt>0)		_polling_PollEmergencyStatusCnt--;			
}
void 					polling_TmpDisable 					(void)
{
	daliStop();
	_polling_TmpDisableTimeoutCnt=POLLING_TMP_DISABLE_TIMEOUT;
}
void 					polling_TmpGroupDisable 					(void)
{
	//daliStop();
	_polling_TmpDisableTimeoutCnt=POLLING_TMP_DISABLE_TIMEOUT;
}
void 					polling_EndTmpDisable				(void)
{
	_polling_Cnt=0;
	_polling_TmpDisableTimeoutCnt=0;	
}
/*
void					polling_PollFailureStatus		(void)
{
	_polling_NextCnt=POLLING_QUERY_FAILURE_STATUS;
	_polling_PollFailureStatusCnt=POLLING_POLL_QUERY_FAILURE_STATUS_CNT_VALUE;
	_polling_SkipCnt[POLLING_QUERY_STATUS][0]=POLLING_QUERY_FAILURE_STATUS_SKIP_CNT_FOCUS_VALUE;
	_polling_SkipCnt[POLLING_QUERY_ACTUAL_LEVEL][1]=POLLING_QUERY_FAILURE_STATUS_SKIP_CNT_FOCUS_VALUE;	
}
*/
void 					polling_PollActualLevel 		(void)
{
	_polling_NextCnt=POLLING_QUERY_STATUS;
	_polling_PollActualLevelCnt=POLLING_POLL_QUERY_ACTUAL_LEVEL_CNT_VALUE;
	_polling_SkipCnt[POLLING_QUERY_STATUS][0]=POLLING_QUERY_STATUS_SKIP_CNT_FOCUS_VALUE;
	_polling_SkipCnt[POLLING_QUERY_ACTUAL_LEVEL][0]=POLLING_QUERY_ACTUAL_LEVEL_SKIP_CNT_FOCUS_VALUE;	
	_polling_SkipCnt[POLLING_QUERY_STATUS][1]=POLLING_QUERY_STATUS_SKIP_CNT_FOCUS_VALUE;
	_polling_SkipCnt[POLLING_QUERY_ACTUAL_LEVEL][1]=POLLING_QUERY_ACTUAL_LEVEL_SKIP_CNT_FOCUS_VALUE;	
}
void 					polling_PollEmergencyMode 	(void)
{
	_polling_NextCnt=POLLING_QUERY_EMERGENCY_MODE;
	_polling_PollEmergencyModeCnt=POLLING_POLL_QUERY_EMERGENCY_MODE_LEVEL_CNT_VALUE;
	_polling_SkipCnt[POLLING_QUERY_EMERGENCY_MODE][0]=POLLING_QUERY_EMERGENCY_MODE_SKIP_CNT_FOCUS_VALUE;	
	_polling_SkipCnt[POLLING_QUERY_EMERGENCY_MODE][1]=POLLING_QUERY_EMERGENCY_MODE_SKIP_CNT_FOCUS_VALUE;
}
void 					polling_PollEmergencyStatus (unsigned char test)
{
	_polling_NextCnt=POLLING_QUERY_EMERGENCY_STATUS;
	if (test==0x00)
	{
		_polling_PollEmergencyStatusCnt=POLLING_POLL_QUERY_EMERGENCY_STATUS_CNT_FT_VALUE;
		_polling_SkipCnt[POLLING_QUERY_EMERGENCY_STATUS][0]=POLLING_QUERY_EMERGENCY_STATUS_SKIP_CNT_FOCUS1_VALUE;
		_polling_SkipCnt[POLLING_QUERY_EMERGENCY_STATUS][1]=POLLING_QUERY_EMERGENCY_STATUS_SKIP_CNT_FOCUS1_VALUE;
	}
	else 
	{
		_polling_PollEmergencyStatusCnt=POLLING_POLL_QUERY_EMERGENCY_STATUS_CNT_DT_VALUE;
		_polling_SkipCnt[POLLING_QUERY_EMERGENCY_STATUS][0]=POLLING_QUERY_EMERGENCY_STATUS_SKIP_CNT_FOCUS2_VALUE;
		_polling_SkipCnt[POLLING_QUERY_EMERGENCY_STATUS][1]=POLLING_QUERY_EMERGENCY_STATUS_SKIP_CNT_FOCUS2_VALUE;
	}	
}	
unsigned char polling_StatusGet 					(void)
{
	if (_polling_TmpDisableTimeoutCnt>0)
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}	
}
void 					polling_DaliNotifyCb				(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray, unsigned char sensorAddress)
{	
	unsigned char apply[8];		
	unsigned char ultimo;
	if (daliReplyFrameType==0x80) 	// Solo procesa frames completos.
	{		
		//_polling_BusyCnt=0;
		daliLoopBatchFrameProcess(daliSentCommandHigh, daliSentCommandLow, daliSentApply, daliReplyValidArray, daliReplyAnswerArray);					
		if (_polling_LastOpIncomplete==0xFF)		polling_CntInc();		
	}			
	if (daliReplyFrameType==0x99) 	// Incompleto.
	{			
		unsigned char n;							
		if (daliSentCommandLow==0x90)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_STATUS);
		if (daliSentCommandLow==0xA0)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_ACTUAL_LEVEL);
		if (daliSentCommandLow==0xFA)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_EMERGENCY_MODE);
		if (daliSentCommandLow==0xFC)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_FAILURE_STATUS);
		if (daliSentCommandLow==0xFD)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_EMERGENCY_STATUS);
		for (n=0;n<64;n++)
		{
			if (utils_BitFromByteArrayGet (&daliReplyValidArray[0], n)==0x01)	ultimo=n;		
		}		
		for (n=ultimo;n<64;n++)
		{			
			utils_BitFromByteArrayClear(&daliSentApply[0],n);
		}	
		daliLoopBatchFrameProcess(daliSentCommandHigh, daliSentCommandLow, daliSentApply, daliReplyValidArray, daliReplyAnswerArray);						
		//_polling_BusyCnt=0;
	}			
	if ((daliSentFrameType==0x15) && (sensorAddress<16))
	{
		unsigned char n;							
		if (daliSentCommandLow==0x90)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_STATUS);
		if (daliSentCommandLow==0xA0)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_ACTUAL_LEVEL);
		if (daliSentCommandLow==0xFA)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_EMERGENCY_MODE);
		if (daliSentCommandLow==0xFC)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_FAILURE_STATUS);
		if (daliSentCommandLow==0xFD)		polling_IncompleteSet(&daliSentApply[0],&daliReplyValidArray[0],POLLING_QUERY_EMERGENCY_STATUS);
		for (n=0;n<64;n++)
		{
			if (utils_BitFromByteArrayGet (&daliReplyValidArray[0], n)==0x01)	ultimo=n;		
		}		
		for (n=ultimo;n<64;n++)
		{			
			utils_BitFromByteArrayClear(&daliSentApply[0],n);
		}	
		daliLoopBatchFrameProcess(daliSentCommandHigh, daliSentCommandLow, daliSentApply, daliReplyValidArray, daliReplyAnswerArray);						
		//_polling_BusyCnt=5;		
		daliSensoresQuiereComunicar (sensorAddress);			
	}	
	_polling_BusyCnt=0;	
}
void 					polling_BusyClear 					(void)
{
	if (_polling_BusyCnt>POLLING_BUSY_CLEAR_CNT_VALUE)	_polling_BusyCnt=POLLING_BUSY_CLEAR_CNT_VALUE;	
}
void 					polling_DoTask 							(void)
{		
	unsigned char apply[8];	
	unsigned char cmd;	
	if (_polling_PollActualLevelCnt==0)					_polling_SkipCnt[POLLING_QUERY_STATUS][1]=POLLING_QUERY_STATUS_SKIP_CNT_DEFAULT_VALUE;
	if (_polling_PollActualLevelCnt==0)					_polling_SkipCnt[POLLING_QUERY_ACTUAL_LEVEL][1]=POLLING_QUERY_ACTUAL_LEVEL_SKIP_CNT_DEFAULT_VALUE;
	if (_polling_PollEmergencyModeCnt==0)				_polling_SkipCnt[POLLING_QUERY_EMERGENCY_MODE][1]=POLLING_QUERY_EMERGENCY_MODE_SKIP_CNT_DEFAULT_VALUE;
	//if (_polling_PollFailureStatusCnt==0)				_polling_SkipCnt[POLLING_QUERY_FAILURE_STATUS][1]=POLLING_QUERY_FAILURE_STATUS_SKIP_CNT_DEFAULT_VALUE;
	if (_polling_PollEmergencyStatusCnt==0)			_polling_SkipCnt[POLLING_QUERY_EMERGENCY_STATUS][1]=POLLING_QUERY_EMERGENCY_STATUS_SKIP_CNT_DEFAULT_VALUE;
	if (interfacesInstaladosCntGet()==0)				cmd=0x07;
	if (interfacesInstaladosCntGet()>0)					cmd=0x15;		
	
	if ((_polling_TmpDisableTimeoutCnt==0x00) && (_polling_BusyCnt==0x00)	&& (daliSendInProgressGet()==0x00))
	{			
		if (_polling_Cnt==POLLING_QUERY_STATUS)
		{			
			daliLoopInstalledArrayByteGet (&apply[0]);
			polling_IncompleteCheck (&apply[0]);
			if (daliSend(DALI_POLLINGS, 0xFF05, cmd, 0xFF, &apply[0], 0x2E, 0x90)==0x01)
			{			
				_polling_BusyCnt=POLLING_BUSY_CNT_MAX;
			}		
		}			
		if (_polling_Cnt==POLLING_QUERY_ACTUAL_LEVEL) 
		{		
			daliLoopActualLevelApplyGet (&apply[0]);
			polling_IncompleteCheck (&apply[0]);
			if (daliSend(DALI_POLLINGS, 0xFF05, cmd, 0xFF, &apply[0], 0x2E, 0xA0)==0x01)
			{
				_polling_BusyCnt=POLLING_BUSY_CNT_MAX;	
			}
		}			
		if (_polling_Cnt==POLLING_QUERY_EMERGENCY_MODE)
		{			
			if (daliLoopDeviceTypeMatchArrayByteGet (&apply[0],0x01)!=0x00)
			{
				polling_IncompleteCheck (&apply[0]);
				if (daliSend(DALI_POLLINGS, 0xFF05, cmd, 0xFF, &apply[0], 0x30, 0xFA)==0x01)
				{			
					_polling_BusyCnt=POLLING_BUSY_CNT_MAX;
				}
			}						
			else
			{
				polling_CntInc();
			}
		}			
		if (_polling_Cnt==POLLING_QUERY_FAILURE_STATUS)
		{
			if (daliLoopDeviceTypeMatchArrayByteGet (&apply[0],0x01)!=0x00)
			{
				polling_IncompleteCheck (&apply[0]);
				if (daliSend(DALI_POLLINGS, 0xFF05, cmd, 0xFF, &apply[0], 0x30, 0xFC)==0x01)
				{			
					_polling_BusyCnt=POLLING_BUSY_CNT_MAX;
				}
			}
			else
			{
				polling_CntInc();
			}
		}			
		if (_polling_Cnt==POLLING_QUERY_EMERGENCY_STATUS)
		{
			if (daliLoopDeviceTypeMatchArrayByteGet (&apply[0],0x01)!=0x00)
			{					
				if (historicoTest_Send()==0x01)
				{					
					polling_IncompleteCheck (&apply[0]);					
					if (daliSend(DALI_POLLINGS, 0xFF05, cmd, 0xFF, &apply[0], 0x30, 0xFD)==0x01)
					{					
						_polling_BusyCnt=POLLING_BUSY_CNT_MAX;
					}		
				}					
			}
			else
			{
				polling_CntInc();
			}															
		}		
	}
}		

