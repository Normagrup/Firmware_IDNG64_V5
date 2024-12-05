#include "dali.h"
#include "daliUtils.h"
#include "polling.h"
#include "mainCfg.h"

unsigned char	_daliUtilsBlink;
unsigned char _daliUtilsBlinkCnt1;
unsigned int _daliUtilsBlinkCnt2;
unsigned int _daliUtilsBlinkPeriod;						//100 equivale a unos 31 segundos

unsigned char _daliUtilsBlinkOnOff;
unsigned char _daliUtilsBlinkDest;
unsigned int _daliUtilsRnd;	
unsigned char _daliUtilsDaliSentValid;
unsigned char _daliUtilsDaliSentAnswer;
unsigned char _daliUtilsDaliSentRecibido;

unsigned char _daliUtilsBlinkStartIdentificationCnt;

unsigned char	_daliUtilsBlinkNotif;

extern unsigned char _broadcastAddress[4];

void botoneraTestOn (unsigned char period);
void botoneraTestOff (void);


	
unsigned char daliUtilsDaliSend (unsigned char daliSentDest, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char *valid, unsigned char *answer)
{
	unsigned char apply[8];	
	if (daliSend(DALI_UTILS, _daliUtilsRnd, 0x01, daliSentDest, &apply[0], daliSentCommandHigh, daliSentCommandLow)==0x01)
	{
			_daliUtilsDaliSentRecibido=0;
			while(_daliUtilsDaliSentRecibido==0x00)
			{
				daliDoTask();
			}			
			*valid=_daliUtilsDaliSentValid;
			*answer=_daliUtilsDaliSentAnswer;			
			return 0x01;
	}	
	else
	{
		return 0x00;
	}
}


void daliUtilsDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)									
{
	_daliUtilsDaliSentRecibido=0x01;
	_daliUtilsDaliSentValid=daliReplyValid;
	_daliUtilsDaliSentAnswer=daliReplyAnswer;
}




void daliUtilsDoTask (void)
{
	_daliUtilsRnd++;	
}

unsigned char daliUtilsBootloaderModelHSet(unsigned char dest,unsigned char data)
{
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		daliUtilsDaliSend(0xC3,0x2B,99,&_valid, &_answer);		//DTR1 99
		daliUtilsDaliSend(0xA3,0x2B,228,&_valid, &_answer);		//DTR 228
		daliUtilsDaliSend(dest,0x2D,0x81,&_valid, &_answer);	//ENABLE WRITE MEMORY (REP)
		daliUtilsDaliSend(0xC7,0x2B,data,&_valid, &_answer);	//WRITE TO MEMORY LOCATION	
		daliAccessHandlerAllowAllModules();
		return 0x01;
	}
	else
	{
		return 0x00;
	}	
}
unsigned char daliUtilsBootloaderModelLSet(unsigned char dest,unsigned char data)
{
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		daliUtilsDaliSend(0xC3,0x2B,99,&_valid, &_answer);		//DTR1 99
		daliUtilsDaliSend(0xA3,0x2B,229,&_valid, &_answer);		//DTR 229
		daliUtilsDaliSend(dest,0x2D,0x81,&_valid, &_answer);	//ENABLE WRITE MEMORY (REP)
		daliUtilsDaliSend(0xC7,0x2B,data,&_valid, &_answer);	//WRITE TO MEMORY LOCATION	
		daliAccessHandlerAllowAllModules();
		return 0x01;
	}
	else
	{
		return 0x00;
	}	
}
unsigned char daliUtilsBootloaderVersionSet(unsigned char dest,unsigned char data)
{
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		daliUtilsDaliSend(0xC3,0x2B,99,&_valid, &_answer);		//DTR1 99
		daliUtilsDaliSend(0xA3,0x2B,230,&_valid, &_answer);		//DTR 230
		daliUtilsDaliSend(dest,0x2D,0x81,&_valid, &_answer);	//ENABLE WRITE MEMORY (REP)
		daliUtilsDaliSend(0xC7,0x2B,data,&_valid, &_answer);	//WRITE TO MEMORY LOCATION		
		daliAccessHandlerAllowAllModules();
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}
unsigned char daliUtilsBootloaderStatusSet(unsigned char dest,unsigned char data)
{
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		daliUtilsDaliSend(0xC3,0x2B,99,&_valid, &_answer);		//DTR1 99
		daliUtilsDaliSend(0xA3,0x2B,236,&_valid, &_answer);		//DTR 230
		daliUtilsDaliSend(dest,0x2D,0x81,&_valid, &_answer);	//ENABLE WRITE MEMORY (REP)
		daliUtilsDaliSend(0xC7,0x2B,data,&_valid, &_answer);	//WRITE TO MEMORY LOCATION		
		daliAccessHandlerAllowAllModules();
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}




unsigned char daliUtilsBootloaderModelHGet(unsigned char dest, unsigned char *valid, unsigned char *answer)
{
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		daliUtilsDaliSend(0xC3,0x2B,99,&_valid, &_answer);		//DTR1 99
		daliUtilsDaliSend(0xA3,0x2B,228,&_valid, &_answer);		//DTR 228
		daliUtilsDaliSend(dest,0x2B,0xC5,valid, answer);	
		daliAccessHandlerAllowAllModules();
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

unsigned char daliUtilsBootloaderModelLGet(unsigned char dest, unsigned char *valid, unsigned char *answer)
{
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		daliUtilsDaliSend(0xC3,0x2B,99,&_valid, &_answer);		//DTR1 99
		daliUtilsDaliSend(0xA3,0x2B,229,&_valid, &_answer);		//DTR 229
		daliUtilsDaliSend(dest,0x2B,0xC5,valid, answer);
		daliAccessHandlerAllowAllModules();
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

unsigned char daliUtilsBootloaderVersionGet(unsigned char dest, unsigned char *valid, unsigned char *answer)
{
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		daliUtilsDaliSend(0xC3,0x2B,99,&_valid, &_answer);		//DTR1 99
		daliUtilsDaliSend(0xA3,0x2B,230,&_valid, &_answer);		//DTR 230
		daliUtilsDaliSend(dest,0x2B,0xC5,valid, answer);
		daliAccessHandlerAllowAllModules();
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

unsigned char daliUtilsBootloaderStatusGet(unsigned char dest, unsigned char *valid, unsigned char *answer)
{
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		daliUtilsDaliSend(0xC3,0x2B,99,&_valid, &_answer);		//DTR1 99
		daliUtilsDaliSend(0xA3,0x2B,236,&_valid, &_answer);		//DTR 236
		daliUtilsDaliSend(dest,0x2B,0xC5,valid, answer);
		daliAccessHandlerAllowAllModules();
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

unsigned char daliUtilsBootloaderWritePage(unsigned char *pageContent)
{
	unsigned char n;	
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		for (n=0;n<130;n++)
		{
			daliUtilsDaliSend(0xC7,0x2B,pageContent[n],&_valid, &_answer);				//WRITE TO MEMORY LOCATION	
		}	
		daliAccessHandlerAllowAllModules();	
		return 0x01;	
	}
	else
	{
		return 0x00;
	}
	
}


unsigned char daliUtilsRebootSend (unsigned char dest)
{
	unsigned char _valid;
	unsigned char _answer;
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{
		daliUtilsDaliSend(0xC3,0x2B,99,&_valid, &_answer);		//DTR1 99
		daliUtilsDaliSend(0xA3,0x2B,99,&_valid, &_answer);		//DTR 99
		daliUtilsDaliSend(dest,0x2D,0x81,&_valid, &_answer);	//ENABLE WRITE MEMORY (REP)
		daliUtilsDaliSend(0xC7,0x2B,0x01,&_valid, &_answer);	//WRITE TO MEMORY LOCATION					
		daliAccessHandlerAllowAllModules();			
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}



unsigned char daliUtilsBlinkGet (void)
{
	return _daliUtilsBlink;	
}

void daliUtilsBlinkTerminate (void)
{
	unsigned char apply[8];	
	if (_daliUtilsBlink==0x01)
	{
		while (daliSend(DALI_UTILS, 0xFFFF, 0x01, 0xFF, &apply[0], 0x2B, 0x05)==0x00)
		{
			daliDoTask();
		}
		daliAccessHandlerAllowAllModules();	
		_daliUtilsBlink=0x00;
		botoneraTestOff();
	}
}




unsigned char daliUtilsBlinkSend (unsigned char dest, unsigned int period)
{
	unsigned char apply[8];	
	#ifdef DEBUG_UTILS_SEND
		unsigned char debugFrame[5];	
	#endif	
	if (daliAccessHandlerAllowOnlyThisModule (DALI_UTILS, 0x00, 0x01)==0x01)
	{	
		if ((_daliUtilsBlink==0x00) || ((_daliUtilsBlink==0x01) && (_daliUtilsBlinkDest!=dest)))
		{
			_daliUtilsBlink=0x01;
			_daliUtilsBlinkOnOff=0x00;
			_daliUtilsBlinkDest=dest;
			#ifdef DEBUG_UTILS_SEND
				debugFrame[0]=DEBUG_FRAME_TYPE;
				debugFrame[1]=0xFF;	
				debugFrame[2]=0xFF;						
				debugFrame[3]=DEBUG_UTILS_SEND_OP;
				debugFrame[4]=DEBUG_UTILS_SEND_SUB_OP_MAX;
				ethSendFrame(0,&debugFrame[0],DEBUG_UTILS_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_UTILS_SEND_PORT);	
			#endif	
			daliSend(DALI_UTILS, 0xFFFF, 0x01, 0xFF, &apply[0], 0x2B, 0x05);					
			while (daliSend(DALI_UTILS, 0xFFFF, 0x01, _daliUtilsBlinkDest, &apply[0], 0x2C, 0xF0)==0x00)
			{
				daliDoTask();
			}			
			_daliUtilsBlinkCnt1=0;
			_daliUtilsBlinkCnt2=0;		
			_daliUtilsBlinkStartIdentificationCnt=0;
		}
		else
		{
			_daliUtilsBlinkCnt2=0;
		}		
		_daliUtilsBlinkPeriod=period;			
		botoneraTestOn (0xFF);		
		return 0x01;
	}
	else
	{
		return 0x00;
	}	
}

void daliUtilsTick (void)
{
	unsigned char apply[8];
	#ifdef DEBUG_UTILS_SEND
		unsigned char debugFrame[DEBUG_UTILS_SEND_FRAME_LENGHT];	
	#endif	
	if (_daliUtilsBlink==0x01)
	{
		if (_daliUtilsBlinkCnt1<50)
		{
			_daliUtilsBlinkCnt1++;		
		}
		else
		{	
			if (_daliUtilsBlinkCnt2<_daliUtilsBlinkPeriod)
			{
				if (_daliUtilsBlinkOnOff==0x00)
				{
					_daliUtilsBlinkOnOff=0x01;				
					#ifdef DEBUG_UTILS_SEND
						debugFrame[0]=DEBUG_FRAME_TYPE;
						debugFrame[1]=0xFF;
						debugFrame[2]=0xFF;
						debugFrame[3]=DEBUG_UTILS_SEND_OP;
						debugFrame[4]=DEBUG_UTILS_SEND_SUB_OP_OFF;
						ethSendFrame(0,&debugFrame[0],DEBUG_UTILS_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_UTILS_SEND_PORT);	
					#endif	
					daliSend(DALI_UTILS, 0xFFFF, 0x01, _daliUtilsBlinkDest, &apply[0], 0x2B, 0x00);
	
				}
				else
				{
					_daliUtilsBlinkOnOff=0x00;	
					_daliUtilsBlinkNotif=1;
					#ifdef DEBUG_UTILS_SEND
						debugFrame[0]=DEBUG_FRAME_TYPE;
						debugFrame[1]=0xFF;	
						debugFrame[2]=0xFF;						
						debugFrame[3]=DEBUG_UTILS_SEND_OP;
						debugFrame[4]=DEBUG_UTILS_SEND_SUB_OP_MAX;
						ethSendFrame(0,&debugFrame[0],DEBUG_UTILS_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_UTILS_SEND_PORT);	
					#endif						
					daliSend(DALI_UTILS, 0xFFFF, 0x01, _daliUtilsBlinkDest, &apply[0], 0x2B, 0x05);					

					if (_daliUtilsBlinkStartIdentificationCnt<20)
					{
						_daliUtilsBlinkStartIdentificationCnt++;						
					}
					else
					{
						_daliUtilsBlinkStartIdentificationCnt=0;
						while (daliSend(DALI_UTILS, 0xFFFF, 0x01, _daliUtilsBlinkDest, &apply[0], 0x2C, 0xF0)==0x00)
						{
							daliDoTask();
						}
					}					
				}
				_daliUtilsBlinkCnt2++;
				_daliUtilsBlinkCnt1=0;
			}
			else
			{
				daliUtilsBlinkTerminate();		
			}			
		}		
	}	
}
