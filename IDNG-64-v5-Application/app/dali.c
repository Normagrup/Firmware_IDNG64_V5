#include "daliTransceiver.h"
#include "dali.h"
#include "polling.h"
#include "normalink.h"
#include "dali.h"
#include "temp.h"
#include "test.h"
#include "daliAddressing.h"
#include "daliUtils.h"

#define CONECTAR_CON_DALI_SENSORES
#include "daliSensores.h"
#include "mainCfg.h"

// 1 + 1 + 1 + 8 + 8 +8 +8 +8 +8 +8 + 64 + 8 + 8 +8 +8 +8 + 64 + 8 + 8 = 243

unsigned char _daliAccessHandlerStatus;
unsigned char daliPacketIdCnt;
unsigned char daliBufferSendInProgress;
unsigned char _daliBufferSendEnable[DALI_BUFFERS_CNT];
unsigned char _daliBufferSendPending[DALI_BUFFERS_CNT];
unsigned int _daliBufferSenderPacketId[DALI_BUFFERS_CNT];
unsigned char _daliBufferFrameType[DALI_BUFFERS_CNT];
unsigned char _daliBufferDest[DALI_BUFFERS_CNT];
unsigned char _daliBufferCommandHigh[DALI_BUFFERS_CNT];
unsigned char _daliBufferCommandLow[DALI_BUFFERS_CNT];
unsigned char _daliBufferApply[DALI_BUFFERS_CNT][8];
unsigned char _daliSent[DALI_BUFFERS_CNT];
unsigned int 	_daliSentSenderPacketId[DALI_BUFFERS_CNT];
unsigned char _daliSentPacketId[DALI_BUFFERS_CNT];
unsigned char _daliSentFrameType[DALI_BUFFERS_CNT];
unsigned char _daliSentDest[DALI_BUFFERS_CNT];
unsigned char _daliSentApply[DALI_BUFFERS_CNT][8];
unsigned char _daliSentCommandHigh[DALI_BUFFERS_CNT];
unsigned char _daliSentCommandLow[DALI_BUFFERS_CNT];
unsigned char daliBloqueanteEnviado;
unsigned char daliBloqueanteValid;
unsigned char daliBloqueanteAnswer;


unsigned short int _daliAccessHandlerAddr;

unsigned char	_daliBufferDestArray[DALI_BUFFER_LENGHT];
unsigned char	_daliBufferCom[DALI_BUFFER_LENGHT];

#ifdef DEBUG_DALI_SEND
	extern unsigned char _broadcastAddress[4];
#endif


unsigned char daliAccessHandlerAddrLock (unsigned short int handler)
{
	unsigned char ret=0;
	_daliAccessHandlerAddr=_daliAccessHandlerAddr+handler;		
	if (_daliAccessHandlerAddr!=0x0000)			
	{		
		ret=daliAccessHandlerAllowOnlyThisModule (DALI_ADDRESSING,0,0);
	}
	return ret;
}
void daliAccessHandlerAddrUnlock (unsigned short int handler)
{	
	if (_daliAccessHandlerAddr>=handler)			_daliAccessHandlerAddr=_daliAccessHandlerAddr-handler;			
	if (_daliAccessHandlerAddr==0x0000)				daliAccessHandlerAllowAllModules();
}




void daliAccessHandlerAllowAllModules (void)
{
	unsigned char n;
	for (n=0;n<DALI_BUFFERS_CNT;n++)
	{
		_daliBufferSendEnable[n]=0x01;		
	}	
	_daliAccessHandlerStatus=0x00;
}

unsigned char daliAccessHandlerAllowOnlyThisModule (unsigned char module, unsigned char stop, unsigned char wait)
{
	unsigned char n;
	unsigned char ret;
		
	if (_daliAccessHandlerStatus==0x00)
	{
		if (stop==0x01)
		{
			daliStop();
		}
		for (n=0;n<DALI_BUFFERS_CNT;n++)
		{
			if (n==module)
			{
				_daliBufferSendEnable[n]=0x01;
			}		
			else
			{
				_daliBufferSendEnable[n]=0x00;
			}
		}		
		while ((wait==0x01) && (daliSendInProgressGet()==0x01)) 	
		{
			daliDoTask();
		}	
		_daliAccessHandlerStatus=0x01;
		ret=0x01;
	}
	else
	{
		if (_daliBufferSendEnable[module]==0x01)
		{
			ret=0x01;
		}
		else
		{
			ret=0x00;	
		}		
	}
	return ret;
}






unsigned char daliSendInProgressGet(void)
{
	unsigned char n;
	
	for (n=0;n<DALI_BUFFERS_CNT;n++)
	{
		if (_daliSent[n]==0x01)
		{
			return 0x01;
		}		
	}
	return 0x00;
}


static unsigned char daliPacketIdCntGet (void)
{
	return daliPacketIdCnt;
}

static void daliPacketIdCntInc (void)
{
	daliPacketIdCnt++;
}

unsigned char daliBufferSend(unsigned char module, unsigned int senderPacketId, unsigned char frameType, unsigned char *dest,  unsigned char *com)
{
	unsigned char n;
	
	if (module<=DALI_BUFFERS_CNT)
	{		
		if ((_daliBufferSendPending[module]==0x00) && (_daliBufferSendEnable[module]==0x01))
		{
			_daliBufferSenderPacketId[module]=senderPacketId;
			_daliBufferFrameType[module]=frameType;
			
			
			for (n=0;n<64;n++)
			{
			//_daliBufferDestArray[n]=0x01;
			//_daliBufferCom[n]=0x02;				
			_daliBufferDestArray[n]=dest[n];
			_daliBufferCom[n]=com[n];
			}
			

			_daliBufferSendPending[module]=0x01;			
			return 0x01;
		}
		else
		{
			return 0x00;
		}		
	}
	return 0x00;
}

unsigned char daliSend(unsigned char module, unsigned int senderPacketId, unsigned char frameType, unsigned char dest, unsigned char *apply, unsigned char commandHigh, unsigned char commandLow)
{
	if (module<=DALI_BUFFERS_CNT)
	{		
		if ((_daliBufferSendPending[module]==0x00) && (_daliBufferSendEnable[module]==0x01))
		{				
			_daliBufferSenderPacketId[module]=senderPacketId;
			_daliBufferFrameType[module]=frameType;
			_daliBufferDest[module]=dest;
			_daliBufferApply[module][7]=apply[7];
			_daliBufferApply[module][6]=apply[6];
			_daliBufferApply[module][5]=apply[5];
			_daliBufferApply[module][4]=apply[4];
			_daliBufferApply[module][3]=apply[3];
			_daliBufferApply[module][2]=apply[2];
			_daliBufferApply[module][1]=apply[1];
			_daliBufferApply[module][0]=apply[0];
			_daliBufferCommandHigh[module]=commandHigh;
			_daliBufferCommandLow[module]=commandLow;		
			_daliBufferSendPending[module]=0x01;	
			return 0x01;
		}
		else
		{
			return 0x00;
		}		
	}
	return 0x00;
}

unsigned char daliDestCorrecto (unsigned char dest)
{
	if ((dest==0xFF) ||	((dest&0xE1)==0x81) ||	((dest&0x81)==0x01))
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}	
}


void daliStop (void)
{
	unsigned char n;
	if (_daliAccessHandlerStatus==0x00)
	{		
		for (n=0;n<DALI_BUFFERS_CNT;n++)
		{		
			_daliBufferSendPending[n]=0;
		}

		daliTransceiverStop();				
	}
}



void daliDoTask (void)
{
	unsigned char dataArray[64];
	unsigned char data;
	unsigned char packetId;
	unsigned char lenght;
	unsigned char n;
	#ifdef DEBUG_DALI_SEND
		unsigned char debugFrame[DEBUG_DALI_SEND_FRAME_LENGHT];	
	#endif	
	if (daliSendInProgressGet()==0x00)																						// No se está enviando nada
	{
		if (daliTransceiverStatusGet()==DALI_TRANSCEIVER_STATUS_READY)							// Se puede enviar
		{
			for (n=0;n<DALI_BUFFERS_CNT;n++)
			{
				if (_daliBufferSendPending[n]==0x01)
				{
					packetId=daliPacketIdCntGet();	
					
					#ifdef DEBUG_DALI_SEND
						debugFrame[0]=DEBUG_FRAME_TYPE;
						debugFrame[1]=0xFF;	
						debugFrame[2]=0xFF;						
						debugFrame[3]=DEBUG_DALI_SEND_OP;
						debugFrame[4]=n;
						debugFrame[5]=_daliBufferSenderPacketId[n]/0x100;
						debugFrame[6]=_daliBufferSenderPacketId[n]&0xFF00;
						debugFrame[7]=_daliBufferFrameType[n];
						debugFrame[8]=_daliBufferDest[n];
						debugFrame[9]=_daliBufferCommandHigh[n];
						debugFrame[10]=_daliBufferCommandLow[n];
						if (!((n==0x07) || (_daliBufferFrameType[n]==0x16)))
						{
							ethSendFrame(0,&debugFrame[0],DEBUG_DALI_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_DALI_SEND_PORT);			
						}
					#endif	
					
					if ((_daliBufferFrameType[n]>=0x17) && (_daliBufferFrameType[n]<=0x20))
					{
						daliBufferTransceiverSend(_daliBufferFrameType[n], packetId, &_daliBufferDestArray[0], &_daliBufferCom[0]);		
					}
					else
					{
						daliTransceiverSend(_daliBufferFrameType[n], packetId, _daliBufferDest[n], &_daliBufferApply[n][0], _daliBufferCommandHigh[n], _daliBufferCommandLow[n], data, lenght, &dataArray[0]);		
					}					
					_daliSent[n]=0x01;						
					_daliSentSenderPacketId[n]=_daliBufferSenderPacketId[n];
					_daliSentFrameType[n]=_daliBufferFrameType[n];
					_daliSentPacketId[n]=packetId;					
					_daliSentDest[n]=_daliBufferDest[n];
					_daliSentApply[n][7]=_daliBufferApply[n][7];
					_daliSentApply[n][6]=_daliBufferApply[n][6];
					_daliSentApply[n][5]=_daliBufferApply[n][5];
					_daliSentApply[n][4]=_daliBufferApply[n][4];
					_daliSentApply[n][3]=_daliBufferApply[n][3];
					_daliSentApply[n][2]=_daliBufferApply[n][2];
					_daliSentApply[n][1]=_daliBufferApply[n][1];
					_daliSentApply[n][0]=_daliBufferApply[n][0];				
					_daliSentCommandHigh[n]=_daliBufferCommandHigh[n];
					_daliSentCommandLow[n]=_daliBufferCommandLow[n];									
					daliPacketIdCntInc();				
					_daliBufferSendPending[n]=0x00;					
					n=DALI_BUFFERS_CNT;	
				}			
				else				
				{
				}
			}						
		}
		else
		{
		}
	}		
	else
	{		
		if (daliTransceiverStatusGet()==DALI_TRANSCEIVER_STATUS_READY)							// Ha terminado de enviar
		{
			for (n=0;n<DALI_BUFFERS_CNT;n++)
			{
				if (_daliSent[n]==0x01)
				{
						unsigned char frameType;
						unsigned char packetId;
						unsigned char valid;
						unsigned char validArray[8];
						unsigned char answer;
						unsigned char answerArray[64];
					
						unsigned char sensorAddress;
					
						if (daliTransceiverIncomingFrameGet (&frameType, &packetId, &valid, &validArray[0], &answer, &answerArray[0], &sensorAddress)==0x01)
						{
								if (_daliSentPacketId[n]==packetId)	// Coincide
								{								
									switch (n)
									{
										case DALI_SENSORS:
										{
											#ifdef CONECTAR_CON_DALI_SENSORES
												daliSensoresDaliNotifyCb(_daliSentSenderPacketId[n],_daliSentFrameType[n],_daliSentDest[n],&_daliSentApply[n][0],_daliSentCommandHigh[n],_daliSentCommandLow[n],frameType,valid,&validArray[0],answer,&answerArray[0]);	
												polling_BusyClear();
											#endif
											break;
										}
										
										case DALI_NORMALINK:
										{
											normalinkDaliNotifyCb(_daliSentSenderPacketId[n],_daliSentFrameType[n],_daliSentDest[n],&_daliSentApply[n][0],_daliSentCommandHigh[n],_daliSentCommandLow[n],frameType,valid,&validArray[0],answer,&answerArray[0]);	
											break;
										}
										
										case DALI_WEBSERVER:
										{
											break;
										}
										
										case DALI_TEST:
										{
											testDaliNotifyCb(_daliSentSenderPacketId[n],_daliSentFrameType[n],_daliSentDest[n],&_daliSentApply[n][0],_daliSentCommandHigh[n],_daliSentCommandLow[n],frameType,valid,&validArray[0],answer,&answerArray[0]);	
											break;
										}
										
										case DALI_TEMP:
										{
											tempDaliNotifyCb(_daliSentSenderPacketId[n],_daliSentFrameType[n],_daliSentDest[n],&_daliSentApply[n][0],_daliSentCommandHigh[n],_daliSentCommandLow[n],frameType,valid,&validArray[0],answer,&answerArray[0]);	
											break;
										}										
										
										case DALI_ADDRESSING:
										{
											daliAddressingDaliNotifyCb(_daliSentSenderPacketId[n],_daliSentFrameType[n],_daliSentDest[n],&_daliSentApply[n][0],_daliSentCommandHigh[n],_daliSentCommandLow[n],frameType,valid,&validArray[0],answer,&answerArray[0]);	
											break;
										}
										
										case DALI_UTILS:
										{
											
											daliUtilsDaliNotifyCb(_daliSentSenderPacketId[n],_daliSentFrameType[n],_daliSentDest[n],&_daliSentApply[n][0],_daliSentCommandHigh[n],_daliSentCommandLow[n],frameType,valid,&validArray[0],answer,&answerArray[0]);	
											break;
										}
										
										case DALI_POLLINGS:
										{
											polling_DaliNotifyCb(_daliSentSenderPacketId[n],_daliSentFrameType[n],_daliSentDest[n],&_daliSentApply[n][0],_daliSentCommandHigh[n],_daliSentCommandLow[n],frameType,valid,&validArray[0],answer,&answerArray[0],sensorAddress);	
											break;
										}
									}		
								}
								else
								{
										//Handler No coincide
								}			
						}			
						_daliSent[n]=0x00;
					}				
				}
			}
		}				
		daliTransceiverDoTask();	
}





















