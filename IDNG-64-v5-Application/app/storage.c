#include "storage.h"
#include "m25p.h"
unsigned char _storageStatus;
unsigned int _storageIdRnd;
unsigned int _storagePtr;

unsigned char storageReadStart (void)
{
	if ((_storageStatus==STORAGE_OP_STOP) || (_storageStatus==STORAGE_OP_READING))
	{
		_storagePtr=0x0000;
		_storageStatus=STORAGE_OP_READING;
		return 0x01;
	}
	else
	{
		return 0x00;
	}	
}

unsigned char storageWriteStart (void)
{
	unsigned char status;	
	if ((_storageStatus==STORAGE_OP_STOP) || (_storageStatus==STORAGE_OP_WRITING))
	{
		m25pWriteEnable();
		m25pSectorErase (4,0,0);
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);	
		_storagePtr=0x0000;
		_storageStatus=STORAGE_OP_WRITING;
		return 0x01;
	}
	else
	{
		return 0x00;
	}
	
}









void storageReadWriteFinish (void)
{
	unsigned char status;	
	unsigned char buffer[4];	
	buffer[0]=(unsigned char)(_storagePtr/0xFFFF);
	buffer[1]=(unsigned char)(_storagePtr&0xFFFF);	
	buffer[2]=(unsigned char)(_storageIdRnd/0x0100);
	buffer[3]=(unsigned char)(_storageIdRnd&0x00FF);	
	if (_storageStatus==STORAGE_OP_WRITING)
	{
		m25pWriteEnable();
		m25pWriteBytes(0x04,0xFF,0xFC,&buffer[0],4);	
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);	
	}	
	_storageStatus=STORAGE_OP_STOP;
}

unsigned int storageIdGet (void)
{
	unsigned char buffer[2];
	unsigned int storageId;
	m25pReadBytes(0x04,0xFF,0xFE,&buffer[0],2);	
	storageId=(buffer[0]*0x0100)+buffer[1];
	return storageId;
}

unsigned char storageWrite (unsigned char *buffer, unsigned int size)
{
	unsigned int storageLastPos;
	unsigned char status;	
	unsigned int n;	
	storageLastPos=_storagePtr+size;		
	if ((_storageStatus==STORAGE_OP_WRITING) && (storageLastPos<=0xFFFB) && (size<=0x0100))
	{
		if ((_storagePtr/0x0100)==((storageLastPos-1)/0x0100))
		{
			m25pWriteEnable();
			m25pWriteBytes(0x04,((unsigned char)(_storagePtr/0x0100)),((unsigned char)(_storagePtr&0x00FF)),&buffer[0],size);	
			do
			{		
				status=m25pStatusRead();
			}while((status&0x01)==0x01);		
			_storagePtr=storageLastPos;			
		}
		else
		{
			for (n=0;n<size;n++)
			{			
				m25pWriteEnable();
				m25pWriteBytes(0x04,((unsigned char)(_storagePtr/0x0100)),((unsigned char)(_storagePtr&0x00FF)),&buffer[n],1);	
				do
				{		
					status=m25pStatusRead();
				}while((status&0x01)==0x01);								
				_storagePtr++;
			}			
		}
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

unsigned char storageRead (unsigned char *buffer, unsigned int size)
{
	unsigned int storageLastPos;
	storageLastPos=_storagePtr+size;
	if ((_storageStatus==STORAGE_OP_READING) && (storageLastPos<=0xFFFB))
	{
		m25pReadBytes(0x04,((unsigned char)(_storagePtr/0x0100)),((unsigned char)(_storagePtr&0x00FF)),buffer,size);	
		_storagePtr=storageLastPos;
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

void storageDoTask (void)
{
	if (_storageIdRnd<0xFFFE)
	{	
		_storageIdRnd++;
	}
	else
	{
		_storageIdRnd=0x00;
	}	
}
