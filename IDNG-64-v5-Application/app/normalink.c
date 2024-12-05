#include "config.h"
#include "rtcc.h"
#include "w5100/socket.h"
#include "w5100/w5100.h"
#include "normalink.h"
#include "dali.h"
#include "mainCfg.h"

unsigned char normaLinkReply;
unsigned int normaLinkPacketId;
unsigned char normaLinkReplyToIp[4];

unsigned char normaLinkDaliBloquearAccesoABusEnabled;
unsigned char normaLinkDaliAccesoExclusivoABusEnabled;
unsigned char normaLinkDaliAccesoExclusivoABusIp[4];
unsigned char normaLinkDaliAccesoExclusivoABusTimeoutCnt;
unsigned char normaLinkDaliBloquearAccesoABusTimeoutCnt;

extern unsigned int portDest;


unsigned char _normaLinkMasterIp[4];

unsigned char _normaLinkMasterPingTicks;

extern unsigned char _broadcastAddress[4];


void normaLinkMasterPing (unsigned char *address)
{
	_normaLinkMasterIp[0]=address[0];
	_normaLinkMasterIp[1]=address[1];
	_normaLinkMasterIp[2]=address[2];
	_normaLinkMasterIp[3]=address[3];
	_normaLinkMasterPingTicks=2;
}

void normaLinkTick (void)
{
	
	if (_normaLinkMasterPingTicks>0)
	{
		_normaLinkMasterPingTicks--;		
	}
	else
	{
		_normaLinkMasterIp[0]=0;
		_normaLinkMasterIp[1]=0;
		_normaLinkMasterIp[2]=0;
		_normaLinkMasterIp[3]=0;
	}
	
	if (normaLinkDaliAccesoExclusivoABusEnabled==0x01)
	{
		if 	(normaLinkDaliAccesoExclusivoABusTimeoutCnt>0)
		{
			normaLinkDaliAccesoExclusivoABusTimeoutCnt--;
		}
		else
		{
			daliAccessHandlerAllowAllModules();
			normaLinkDaliAccesoExclusivoABusEnabled=0x00;
			
		}
	}
	
	if (normaLinkDaliBloquearAccesoABusEnabled==0x01)
	{
		if (normaLinkDaliBloquearAccesoABusTimeoutCnt>15)
		{
			normaLinkDaliBloquearAccesoABusTimeoutCnt--;
		}
		else
		{
			daliAccessHandlerAllowAllModules();
			normaLinkDaliBloquearAccesoABusEnabled=0x00;
			
		}
	}	
}



unsigned char normaLinkDaliAccesoExclusivoABusEnable (unsigned char *address)
{
	if (daliAccessHandlerAllowOnlyThisModule (DALI_NORMALINK, 0x00, 0x01)==0x01)
	{
		normaLinkDaliAccesoExclusivoABusIp[0]=address[0];
		normaLinkDaliAccesoExclusivoABusIp[1]=address[1];
		normaLinkDaliAccesoExclusivoABusIp[2]=address[2];
		normaLinkDaliAccesoExclusivoABusIp[3]=address[3];
		normaLinkDaliAccesoExclusivoABusEnabled=0x01;
		normaLinkDaliAccesoExclusivoABusTimeoutCnt=15;
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

void normaLinkDaliBloquearAccesoABusEnable (void)
{
	normaLinkDaliBloquearAccesoABusEnabled=0x01;
	normaLinkDaliBloquearAccesoABusTimeoutCnt=15;
}

void normaLinkDaliAccesoExclusivoABusDisable (void)
{
	daliAccessHandlerAllowAllModules();
	normaLinkDaliAccesoExclusivoABusEnabled=0x00;
}

void normaLinkDaliBloquearAccesoABusDisable (void)
{
	daliAccessHandlerAllowAllModules();
	normaLinkDaliBloquearAccesoABusEnabled=0x00;	
}
















void normalinkBufferDaliSend (unsigned char *address, unsigned int packetId, unsigned char frameType, unsigned char *dest, unsigned char *com)
{
	unsigned char apply[8];
	unsigned char respuesta[6];
	unsigned char tipoDeFrame;
	
	
	if ((normaLinkDaliBloquearAccesoABusEnabled==0x00) && ((normaLinkDaliAccesoExclusivoABusEnabled==0x00) || (address[0]==normaLinkDaliAccesoExclusivoABusIp[0]) && (address[1]==normaLinkDaliAccesoExclusivoABusIp[1]) && (address[2]==normaLinkDaliAccesoExclusivoABusIp[2]) && (address[3]==normaLinkDaliAccesoExclusivoABusIp[3])))
	{
		daliStop();
		if (daliBufferSend(DALI_NORMALINK, packetId, frameType, &dest[0], &com[0])==0x01)
		{		
			normaLinkReply=0x01;
			normaLinkPacketId=packetId;
			normaLinkReplyToIp[0]=address[0];
			normaLinkReplyToIp[1]=address[1];
			normaLinkReplyToIp[2]=address[2];
			normaLinkReplyToIp[3]=address[3];	
		}
		else
		{
			tipoDeFrame=0x82;			
			respuesta[0]=tipoDeFrame;
			respuesta[1]=(unsigned char)(packetId/0x0100);
			respuesta[2]=(unsigned char)(packetId&0x00FF);	
			respuesta[3]=0x00;
			respuesta[4]=0x00;			
			ethSendFrame(0,&respuesta[0],5,&normaLinkReplyToIp[0], portDest);					
		}
	}
	else
	{
			tipoDeFrame=0x82;			
			respuesta[0]=tipoDeFrame;
			respuesta[1]=(unsigned char)(packetId/0x0100);
			respuesta[2]=(unsigned char)(packetId&0x00FF);	
			respuesta[3]=0x00;
			respuesta[4]=0x00;			
			ethSendFrame(0,&respuesta[0],5,&normaLinkReplyToIp[0], portDest);					
	}
}





void normalinkDaliSend (unsigned char *address, unsigned int packetId, unsigned char daliFrameType, unsigned char daliTransceiverDest, unsigned char daliTransceiverCommandHigh, unsigned char daliTransceiverCommandLow)
{
	unsigned char apply[8];
	unsigned char respuesta[6];
	unsigned char tipoDeFrame;	
	#ifdef DEBUG_NORMALINK_SEND
		unsigned char debugFrame[DEBUG_NORMALINK_SEND_FRAME_LENGHT];
	#endif		
	if ((normaLinkDaliBloquearAccesoABusEnabled==0x00) && ((normaLinkDaliAccesoExclusivoABusEnabled==0x00) || (address[0]==normaLinkDaliAccesoExclusivoABusIp[0]) && (address[1]==normaLinkDaliAccesoExclusivoABusIp[1]) && (address[2]==normaLinkDaliAccesoExclusivoABusIp[2]) && (address[3]==normaLinkDaliAccesoExclusivoABusIp[3])))
	{
		daliStop();
		if (daliSend(DALI_NORMALINK, packetId, daliFrameType, daliTransceiverDest, &apply[0], daliTransceiverCommandHigh, daliTransceiverCommandLow)==0x01)
		{		
			normaLinkReply=0x01;
			normaLinkPacketId=packetId;
			normaLinkReplyToIp[0]=address[0];
			normaLinkReplyToIp[1]=address[1];
			normaLinkReplyToIp[2]=address[2];
			normaLinkReplyToIp[3]=address[3];	
		}
		else
		{
			tipoDeFrame=0x82;			
			respuesta[0]=tipoDeFrame;
			respuesta[1]=(unsigned char)(packetId/0x0100);
			respuesta[2]=(unsigned char)(packetId&0x00FF);	
			respuesta[3]=0x00;
			respuesta[4]=0x00;			
			ethSendFrame(0,&respuesta[0],5,&normaLinkReplyToIp[0], portDest);					
		}
	}
	else
	{
			tipoDeFrame=0x82;			
			respuesta[0]=tipoDeFrame;
			respuesta[1]=(unsigned char)(packetId/0x0100);
			respuesta[2]=(unsigned char)(packetId&0x00FF);	
			respuesta[3]=0x00;
			respuesta[4]=0x00;			
			ethSendFrame(0,&respuesta[0],5,&normaLinkReplyToIp[0], portDest);					
	}	
	#ifdef DEBUG_NORMALINK_SEND
		debugFrame[0]=DEBUG_FRAME_TYPE;
		debugFrame[1]=0xFF;	
		debugFrame[2]=0xFF;						
		debugFrame[3]=DEBUG_NORMALINK_SEND_OP;	
		debugFrame[4]=address[0];
		debugFrame[5]=address[1];
		debugFrame[6]=address[2];
		debugFrame[7]=address[3];
		debugFrame[8]=(unsigned char)(packetId/0x0100);
		debugFrame[9]=(unsigned char)(packetId&0x00FF);	
		debugFrame[10]=daliFrameType;
		debugFrame[11]=daliTransceiverDest;
		debugFrame[12]=daliTransceiverCommandHigh;
		debugFrame[13]=daliTransceiverCommandLow;			
		ethSendFrame(0,&debugFrame[0],DEBUG_NORMALINK_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_NORMALINK_SEND_PORT);	
	#endif		
}

void normalinkDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)									
{
	unsigned char respuesta[6];
	unsigned char tipoDeFrame;
	unsigned char crc;	
	unsigned char valid;
	unsigned char answer;
	if (normaLinkReply==0x01)
	{	
		if (normaLinkPacketId==daliSentSenderPacketId)
		{
			if ((daliSentCommandHigh==0x2C) || (daliSentCommandHigh==0x2B) || (daliSentCommandHigh==0x2D))
			{
				valid=0x01;
				answer=0x00;		
			}
			if ((daliSentCommandHigh==0x2E) || (daliSentCommandHigh==0x30))
			{
				valid=daliReplyValid;
				answer=daliReplyAnswer;
			}
			tipoDeFrame=0x82;
			respuesta[0]=tipoDeFrame;
			respuesta[1]=(normaLinkPacketId/0x0100);
			respuesta[2]=(normaLinkPacketId&0x00FF);	
			respuesta[3]=valid;
			respuesta[4]=answer;			
			ethSendFrame(0,&respuesta[0],5,&normaLinkReplyToIp[0], portDest);		
		}
		else
		{
		}
	}
	else
	{
	}
	normaLinkReply=0x00;
}









unsigned char normaLinkDestCorrecto (unsigned char _dest)
{
	if ((_dest==0xFF) ||	((_dest&0xE1)==0x81) ||	((_dest&0x81)==0x01))
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}	
}

