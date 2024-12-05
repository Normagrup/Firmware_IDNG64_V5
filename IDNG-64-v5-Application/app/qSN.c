#include "qSN.h"
#include "dali.h"
#include "ethFramming.h"

#define NOT_SENDING 	0
#define SENDING 			1
#define SENT 					2

unsigned char 		_qSNReadPending[7];
unsigned char    	_qSNDTRSent;
unsigned char    	_qSNDTR1Sent;
unsigned char    	_qSNReadMemoryLocationSent;
unsigned char    	_qSNReadMemoryLocationValid;
unsigned char    	_qSNReadMemoryLocationAnswer;
unsigned char			_qSNResult[6];

static void qSNDTRSend (unsigned char value);
static void qSNDTR1Send (unsigned char value);
static void qSNReadMemoryLocationSend (unsigned char address);




void qSNDoTask (void)
{
	unsigned char frame[10];
	if (_qSNReadPending[0]==0x01)
	{
		if (_qSNReadPending[2]==0x00)
		{						
			if (_qSNDTR1Sent==NOT_SENDING)									qSNDTR1Send(0x00);
			if (_qSNDTR1Sent==SENT)													qSNDTRSend(0x0B);			
			if (_qSNDTRSent==SENT)
			{
				_qSNReadMemoryLocationSent=NOT_SENDING;				
				_qSNReadMemoryLocationValid=0x00;
				_qSNReadMemoryLocationAnswer=0xFF;				
				_qSNReadPending[2]=0x01;
			}
		}			
		
		if (_qSNReadPending[2]==0x01)
		{			
			if (_qSNReadMemoryLocationSent==NOT_SENDING)		qSNReadMemoryLocationSend((_qSNResult[1]<<1)+1);			
			if (_qSNReadMemoryLocationSent==SENT)
			{
				if (_qSNReadMemoryLocationValid==0x01)
				{
					_qSNResult[2]=_qSNReadMemoryLocationAnswer;		
					_qSNReadMemoryLocationSent=NOT_SENDING;				
					_qSNReadMemoryLocationValid=0x00;
					_qSNReadMemoryLocationAnswer=0xFF;						
					_qSNReadPending[2]=0x02;
				}
				else
				{
					_qSNResult[0]=0x00;
					_qSNReadPending[2]=0x05;					
				}				
			}		
		}			
		
		
		if (_qSNReadPending[2]==0x02)
		{			
			if (_qSNReadMemoryLocationSent==NOT_SENDING)		qSNReadMemoryLocationSend((_qSNResult[1]<<1)+1);			
			if (_qSNReadMemoryLocationSent==SENT)
			{
				if (_qSNReadMemoryLocationValid==0x01)
				{
					_qSNResult[3]=_qSNReadMemoryLocationAnswer;			
					_qSNReadMemoryLocationSent=NOT_SENDING;				
					_qSNReadMemoryLocationValid=0x00;
					_qSNReadMemoryLocationAnswer=0xFF;	
					_qSNReadPending[2]=0x03;
				}
				else
				{
					_qSNResult[0]=0x00;
					_qSNReadPending[2]=0x05;
				}				
			}		
		}			
		
		if (_qSNReadPending[2]==0x03)
		{			
			if (_qSNReadMemoryLocationSent==NOT_SENDING)		qSNReadMemoryLocationSend((_qSNResult[1]<<1)+1);			
			if (_qSNReadMemoryLocationSent==SENT)
			{
				if (_qSNReadMemoryLocationValid==0x01)
				{
					_qSNResult[4]=_qSNReadMemoryLocationAnswer;		
					_qSNReadMemoryLocationSent=NOT_SENDING;				
					_qSNReadMemoryLocationValid=0x00;
					_qSNReadMemoryLocationAnswer=0xFF;						
					_qSNReadPending[2]=0x04;
				}
				else
				{
					_qSNResult[0]=0x00;
					_qSNReadPending[2]=0x05;
				}				
			}		
		}			
		
		if (_qSNReadPending[2]==0x04)
		{			
			if (_qSNReadMemoryLocationSent==NOT_SENDING)		qSNReadMemoryLocationSend((_qSNResult[1]<<1)+1);			
			if (_qSNReadMemoryLocationSent==SENT)
			{
				if (_qSNReadMemoryLocationValid==0x01)
				{
					_qSNResult[5]=_qSNReadMemoryLocationAnswer;	
					_qSNReadMemoryLocationSent=NOT_SENDING;				
					_qSNReadMemoryLocationValid=0x00;
					_qSNReadMemoryLocationAnswer=0xFF;						
					_qSNReadPending[2]=0x05;
				}
				else
				{
					_qSNResult[0]=0x00;
					_qSNReadPending[2]=0x05;
				}				
			}		
		}		
		
		if (_qSNReadPending[2]==0x05)
		{			
			frame[0]=0x83;
			frame[1]=0x40;
			frame[2]=0xA0;
			frame[3]=0x05;
			frame[4]=_qSNResult[0];
			frame[5]=_qSNResult[1];
			frame[6]=_qSNResult[2];
			frame[7]=_qSNResult[3];
			frame[8]=_qSNResult[4];
			frame[9]=_qSNResult[5];			
			ethSendFrame(0, &frame[0], 10, &_qSNReadPending[3],2234);	
			_qSNReadPending[0]=0x00;
			
			
			if (_qSNReadPending[1]<63)
			{
				_qSNReadPending[1]++;
				qSNRead(_qSNReadPending[1],&_qSNReadPending[3]);
			}
				
			
			
			
		}				
	}	
}



static void qSNDTRSend (unsigned char value)
{
	unsigned char apply[8];	
	if (_qSNDTRSent==NOT_SENDING)
	{						
		if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xA3, &apply[0], 0x2B, value)==0x01)
		{	
			_qSNDTRSent=SENDING;		
		}
	}		
}

static void qSNDTR1Send (unsigned char value)
{
	unsigned char apply[8];	
	if (_qSNDTR1Sent==NOT_SENDING)
	{						
		if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, 0xC3, &apply[0], 0x2B, value)==0x01)
		{	
			_qSNDTR1Sent=SENDING;		
		}
	}		
}

static void qSNReadMemoryLocationSend (unsigned char dest)
{
	unsigned char apply[8];
	
	if (_qSNReadMemoryLocationSent==NOT_SENDING)
	{
		if (daliSend(DALI_ADDRESSING, 0xFFFF, 0x01, dest, &apply[0], 0x2E, 0xC5)==0x01)
		{
			_qSNReadMemoryLocationSent=SENDING;
		}		
	}	
}

void qSNDTRCb (unsigned char daliSentDest)
{
	if ((_qSNDTRSent==SENDING) && (daliSentDest==0xA3))
	{
		_qSNDTRSent=SENT;
	}
}

void qSNDTR1Cb (unsigned char daliSentDest)
{
	if ((_qSNDTR1Sent==SENDING) && (daliSentDest==0xC3))
	{
		_qSNDTR1Sent=SENT;
	}
}


void qSNReadMemoryLocationCb (unsigned char daliSentCommandLow, unsigned char daliSentReplyValid, unsigned char daliSentReplyAnswer)
{
	if ((_qSNReadMemoryLocationSent==SENDING) && (daliSentCommandLow==0xC5))
	{		
		if (daliSentReplyValid==0x01)
		{
			_qSNReadMemoryLocationValid=0x01;
			_qSNReadMemoryLocationAnswer=daliSentReplyAnswer;
		}		
		else
		{
			_qSNReadMemoryLocationValid=0x00;
			_qSNReadMemoryLocationAnswer=0x00;
		}
		_qSNReadMemoryLocationSent=SENT;
	}	
}














void qSNRead (unsigned char address, unsigned char *origen)
{
	_qSNReadPending[0]=0x01;	
	if (address<64)
	{	
	_qSNReadPending[1]=address;
	_qSNReadPending[2]=0x00;		
	_qSNReadPending[3]=origen[0];
	_qSNReadPending[4]=origen[1];
	_qSNReadPending[5]=origen[2];
	_qSNReadPending[6]=origen[3];	
	_qSNDTRSent=NOT_SENDING;
	_qSNDTR1Sent=NOT_SENDING;
	_qSNReadMemoryLocationSent=NOT_SENDING;	
	_qSNResult[0]=0x01;
	_qSNResult[1]=address;
	_qSNResult[2]=0xFF;
	_qSNResult[3]=0xFF;
	_qSNResult[4]=0xFF;
	_qSNResult[5]=0xFF;				
	}
}	
	








