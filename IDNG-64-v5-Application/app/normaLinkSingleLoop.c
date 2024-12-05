#include "normalink.h"
#include "config.h"
#include "rtcc.h"
#include "daliLoop.h"
#include "test.h"
#include "temp.h"
#include "daliAddressing.h"
#include "ethFramming.h"

extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];	//Ip del host que me envía el paquete
extern unsigned int portDest;		//Puerto del host que me envía el paquete


extern unsigned char daliAddressingNewInitialisationOpPending;
extern unsigned char found;



void normaLinkSingleLoopT0(unsigned char _comL)
{
	unsigned char _tipoDeFrame;
	unsigned char _data[64];
	unsigned char _valid[8];
	unsigned char _respuesta[76];
	unsigned char _n;


	// [000] DESCRUBRIR INSTALACION
	if (_comL==0x00) 
	{
		daliAddressingLineScanStart(&ipDest[0]);			
	}
	
	


	
	

	
	
	
	// [040] DEVOLVER EQUIPOS INSTALADOS
	if (_comL==0x40) 
	{			
		_tipoDeFrame=0x80;
		_valid[0]=0xFF;						//Apply 7
		_valid[1]=0xFF;						//Apply 6
		_valid[2]=0xFF;						//Apply 5
		_valid[3]=0xFF;						//Apply 4
		_valid[4]=0xFF;						//Apply 3
		_valid[5]=0xFF;						//Apply 2
		_valid[6]=0xFF;						//Apply 1
		_valid[7]=0xFF;						//Apply 0
		for (_n=0;_n<64;_n++)
		{
				_data[_n]=daliLoopInstalledGet ((63-_n));
		}
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];		
		for (_n=0;_n<8;_n++)
		{
				_respuesta[3+_n]=_valid[_n];
		}		
		for (_n=0;_n<64;_n++)
		{
				_respuesta[11+_n]=_data[_n];
		}
		ethSendFrame(0,&_respuesta[0],75,&ipDest[0], portDest);	
	}
	
	// [041] DEVOLVER TIPO DE EQUIPOS
	if (_comL==0x41) 
	{	
		_tipoDeFrame=0x80;
		_valid[0]=daliLoopValidByteGet(3,7);
		_valid[1]=daliLoopValidByteGet(3,6);
		_valid[2]=daliLoopValidByteGet(3,5);
		_valid[3]=daliLoopValidByteGet(3,4);
		_valid[4]=daliLoopValidByteGet(3,3);
		_valid[5]=daliLoopValidByteGet(3,2);
		_valid[6]=daliLoopValidByteGet(3,1);
		_valid[7]=daliLoopValidByteGet(3,0);
		for (_n=0;_n<64;_n++)
		{
				_data[_n]=daliLoopDeviceTypeGet ((63-_n));
		}
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];		
		for (_n=0;_n<8;_n++)
		{
				_respuesta[3+_n]=_valid[_n];
		}		
		for (_n=0;_n<64;_n++)
		{
				_respuesta[11+_n]=_data[_n];
		}
		ethSendFrame(0,&_respuesta[0],75,&ipDest[0], portDest);			
	}
	
	// [042] DEVOLVER ESTADO DE COMUNICACION DE LOS EQUIPOS
	if (_comL==0x42) 
	{			
		_tipoDeFrame=0x80;
		_valid[0]=daliLoopValidByteGet(4,7);
		_valid[1]=daliLoopValidByteGet(4,6);
		_valid[2]=daliLoopValidByteGet(4,5);
		_valid[3]=daliLoopValidByteGet(4,4);
		_valid[4]=daliLoopValidByteGet(4,3);
		_valid[5]=daliLoopValidByteGet(4,2);
		_valid[6]=daliLoopValidByteGet(4,1);
		_valid[7]=daliLoopValidByteGet(4,0);
		for (_n=0;_n<64;_n++)
		{
				_data[_n]=daliLoopErrorComGet ((63-_n));
		}
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];		
		for (_n=0;_n<8;_n++)
		{
				_respuesta[3+_n]=_valid[_n];
		}		
		for (_n=0;_n<64;_n++)
		{
				_respuesta[11+_n]=_data[_n];
		}
		ethSendFrame(0,&_respuesta[0],75,&ipDest[0], portDest);	
		
	}
		
	// [060] DEVOLVER ACTUAL DIM LEVEL
	if (_comL==0x60) 
	{		
		_tipoDeFrame=0x80;
		_valid[0]=daliLoopValidByteGet(0,7);
		_valid[1]=daliLoopValidByteGet(0,6);
		_valid[2]=daliLoopValidByteGet(0,5);
		_valid[3]=daliLoopValidByteGet(0,4);
		_valid[4]=daliLoopValidByteGet(0,3);
		_valid[5]=daliLoopValidByteGet(0,2);
		_valid[6]=daliLoopValidByteGet(0,1);
		_valid[7]=daliLoopValidByteGet(0,0);		
		for (_n=0;_n<64;_n++)
		{
				_data[_n]=daliLoopActualLevelGet ((63-_n));
		}
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];		
		for (_n=0;_n<8;_n++)
		{
				_respuesta[3+_n]=_valid[_n];
		}		
		for (_n=0;_n<64;_n++)
		{
				_respuesta[11+_n]=_data[_n];
		}
		ethSendFrame(0,&_respuesta[0],75,&ipDest[0], portDest);	
	}
		
		// [061] DEVOLVER STATUS
	if (_comL==0x61) 
	{		
		_tipoDeFrame=0x80;
		_valid[0]=daliLoopValidByteGet(5,7);
		_valid[1]=daliLoopValidByteGet(5,6);
		_valid[2]=daliLoopValidByteGet(5,5);
		_valid[3]=daliLoopValidByteGet(5,4);
		_valid[4]=daliLoopValidByteGet(5,3);
		_valid[5]=daliLoopValidByteGet(5,2);
		_valid[6]=daliLoopValidByteGet(5,1);
		_valid[7]=daliLoopValidByteGet(5,0);		
		for (_n=0;_n<64;_n++)
		{
				_data[_n]=daliLoopStatusGet ((63-_n));
		}
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];		
		for (_n=0;_n<8;_n++)
		{
				_respuesta[3+_n]=_valid[_n];
		}		
		for (_n=0;_n<64;_n++)
		{
				_respuesta[11+_n]=_data[_n];
		}
		ethSendFrame(0,&_respuesta[0],75,&ipDest[0], portDest);			
	}
	
	// [080] QUERY EMERGENCY MODE
	if (_comL==0x80) 
	{		
		_tipoDeFrame=0x80;
		_valid[0]=daliLoopValidByteGet(1,7);
		_valid[1]=daliLoopValidByteGet(1,6);
		_valid[2]=daliLoopValidByteGet(1,5);
		_valid[3]=daliLoopValidByteGet(1,4);
		_valid[4]=daliLoopValidByteGet(1,3);
		_valid[5]=daliLoopValidByteGet(1,2);
		_valid[6]=daliLoopValidByteGet(1,1);
		_valid[7]=daliLoopValidByteGet(1,0);		
		for (_n=0;_n<64;_n++)
		{
				_data[_n]=daliLoopEmergencyModeGet ((63-_n));
		}
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];		
		for (_n=0;_n<8;_n++)
		{
				_respuesta[3+_n]=_valid[_n];
		}		
		for (_n=0;_n<64;_n++)
		{
				_respuesta[11+_n]=_data[_n];
		}
		ethSendFrame(0,&_respuesta[0],75,&ipDest[0], portDest);	
	}
	
		// [081] QUERY FAILURE STATUS
	if (_comL==0x81) 
	{		
		_tipoDeFrame=0x80;
		_valid[0]=daliLoopValidByteGet(2,7);
		_valid[1]=daliLoopValidByteGet(2,6);
		_valid[2]=daliLoopValidByteGet(2,5);
		_valid[3]=daliLoopValidByteGet(2,4);
		_valid[4]=daliLoopValidByteGet(2,3);
		_valid[5]=daliLoopValidByteGet(2,2);
		_valid[6]=daliLoopValidByteGet(2,1);
		_valid[7]=daliLoopValidByteGet(2,0);		
		for (_n=0;_n<64;_n++)
		{
				_data[_n]=daliLoopFailureStatusGet ((63-_n));
		}
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];		
		for (_n=0;_n<8;_n++)
		{
				_respuesta[3+_n]=_valid[_n];
		}		
		for (_n=0;_n<64;_n++)
		{
				_respuesta[11+_n]=_data[_n];
		}
		ethSendFrame(0,&_respuesta[0],75,&ipDest[0], portDest);	
	}
	
	
	
	
}


void normaLinkSingleLoopT4(unsigned char _comL)
{
	unsigned char _lenghtRx;
	unsigned char _lenghtTx;
	unsigned char _dataRx[1];
	unsigned char _dataTx[11];	
	unsigned char _tempData[10];
	unsigned char _respuesta[16];
	unsigned char _tipoDeFrame;
	unsigned char _tempNumero;
	unsigned char _tempActiva;
			
	_lenghtRx=recibireth[6];	
	

	
	// [008] NUEVA INICIALIZACION
	
	if ((_comL==0x00) && (_lenghtRx==0x00))
	{
		daliAddressingLineScanStart(&ipDest[0]);			
	}
	
	
	if ((_comL==0x08) && (_lenghtRx==0x00))
	{
		daliAddressingNewInitialisationStart(&ipDest[0],0x00);		
	}
	
	// [009] SYTEM EXTENSION
	if ((_comL==0x09) && (_lenghtRx==0x00))
	{
		daliAddressingSystemExtensionStart (&ipDest[0]);
	}	
	
	
	// [018] ADQUIRIR GRUPOS PASARELA
	if ((_comL==0x18) && (_lenghtRx==0x00))
	{
		if (daliAddressingGroupScanStart()==0x01)		
		{			
			_respuesta[4]=0x01;
		}
		else
		{
			_respuesta[4]=0x00;
		}				
		_respuesta[0]=0x83;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x00;		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	//	[019] LEER GRUPOS.
	if ((_comL==0x19) && (_lenghtRx==0x00))
	{
		unsigned char groupCnt;
		unsigned char belongsGroup[8];				
		unsigned char ptr;
		for (groupCnt=0;groupCnt<16;groupCnt++)
		{		
			_respuesta[0]=0x83;																																									//	FrameType
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];
			_respuesta[3]=0x08;																																									//	Lenght=8			
			belongsGroupGet (groupCnt, &belongsGroup[0]);			
			ptr=4;					
			bytePush(groupCnt,&_respuesta[ptr],&ptr);																														//1
			arrayPush(&belongsGroup[0],&_respuesta[ptr],&ptr,8);																								//8				
			ethSendFrame(0,&_respuesta[0],ptr,&ipDest[0], 2234);			
		}
	}
	
	//	[01A] GRABAR GRUPOS.
	if ((_comL==0x1A) && (_lenghtRx==0x08))
	{
		unsigned char group;		
		group=recibireth[7];		
		if (group<16)	
		{
			belongsGroupSet(group, &recibireth[8]);
			_respuesta[0]=0x83;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x01;		
			_respuesta[4]=0x01;		
			_respuesta[5]=group;		
			ethSendFrame(0,&_respuesta[0],6,&ipDest[0], portDest);			
		}
	}
	
	// [01B] APLICAR
	if ((_comL==0x1B) && (_lenghtRx==0x00))
	{
		configStoreInFlash();
		_respuesta[0]=0x83;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x00;		
		_respuesta[4]=0x01;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	
	
	
	
		
	if ((_comL==0x64) && (_lenghtRx==0x00))										//Obtener temporizaciones 0-16
	{
		_dataRx[0]=recibireth[7];
		
		if (tempGet (_dataRx[0], &_tempData[0])==0x01)
		{
			_dataTx[10]=0x01;
			_dataTx[9]=_tempData[0];
			_dataTx[8]=_tempData[1];
			_dataTx[7]=_tempData[2];
			_dataTx[6]=_tempData[3];
			_dataTx[5]=_tempData[4];
			_dataTx[4]=_tempData[5];
			_dataTx[3]=_tempData[6];
			_dataTx[2]=_tempData[7];
			_dataTx[1]=_tempData[8];
			_dataTx[0]=_tempData[9];
		}
		else
		{
			_dataTx[10]=0x00;
			_dataTx[9]=0x00;
			_dataTx[8]=0x00;
			_dataTx[7]=0x00;
			_dataTx[6]=0x00;
			_dataTx[5]=0x00;
			_dataTx[4]=0x00;
			_dataTx[3]=0x00;
			_dataTx[2]=0x00;
			_dataTx[1]=0x00;
			_dataTx[0]=0x00;
		}
		_tipoDeFrame=0x83;
		_lenghtTx=0x0B;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenghtTx;
		_respuesta[4]=_dataTx[10];
		_respuesta[5]=_dataTx[9];
		_respuesta[6]=_dataTx[8];
		_respuesta[7]=_dataTx[7];
		_respuesta[8]=_dataTx[6];
		_respuesta[9]=_dataTx[5];
		_respuesta[10]=_dataTx[4];
		_respuesta[11]=_dataTx[3];
		_respuesta[12]=_dataTx[2];
		_respuesta[13]=_dataTx[1];
		_respuesta[14]=_dataTx[0];
		ethSendFrame(0,&_respuesta[0],15,&ipDest[0], portDest);	
	}
		

	if ((_comL==0x65) && (_lenghtRx==0x0B)) 									//Establecer temporizaciones 0-16
	{
		
		_tempNumero=recibireth[7];
		_tempActiva=recibireth[8];
		_tempData[0]=recibireth[9];
		_tempData[1]=recibireth[10];
		_tempData[2]=recibireth[11];
		_tempData[3]=recibireth[12];
		_tempData[4]=recibireth[13];
		_tempData[5]=recibireth[14];
		_tempData[6]=recibireth[15];
		_tempData[7]=recibireth[16];
		_tempData[8]=recibireth[17];
		_tempData[9]=recibireth[18];		
		tempSet (_tempNumero, _tempActiva, &_tempData[0]);
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	if ((_comL==0x84) && (_lenghtRx==0x00))										//Obtener test 0-16
	{
		_dataRx[0]=recibireth[7];
		
		if (testGet (_dataRx[0], &_tempData[0])==0x01)
		{
			_dataTx[7]=0x01;
			_dataTx[6]=_tempData[0];
			_dataTx[5]=_tempData[1];
			_dataTx[4]=_tempData[2];
			_dataTx[3]=_tempData[3];
			_dataTx[2]=_tempData[4];
			_dataTx[1]=_tempData[5];			
			_dataTx[0]=_tempData[6];			
		}
		else
		{
			_dataTx[7]=0x00;
			_dataTx[6]=0x00;
			_dataTx[5]=0x00;
			_dataTx[4]=0x00;
			_dataTx[3]=0x00;
			_dataTx[2]=0x00;
			_dataTx[1]=0x00;
			_dataTx[0]=0x00;			
		}
		
		_tipoDeFrame=0x83;
		_lenghtTx=0x07;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenghtTx;
		_respuesta[4]=_dataTx[7];
		_respuesta[5]=_dataTx[6];
		_respuesta[6]=_dataTx[5];
		_respuesta[7]=_dataTx[4];
		_respuesta[8]=_dataTx[3];
		_respuesta[9]=_dataTx[2];
		_respuesta[10]=_dataTx[1];
		_respuesta[11]=_dataTx[0];
		ethSendFrame(0,&_respuesta[0],12,&ipDest[0], portDest);					
	}
	
	if ((_comL==0x85)	&& (_lenghtRx==0x08)) 										//Establecer test 0-16
	{
		_tempNumero=recibireth[7];
		_tempActiva=recibireth[8];
		_tempData[0]=recibireth[9];
		_tempData[1]=recibireth[10];
		_tempData[2]=recibireth[11];
		_tempData[3]=recibireth[12];
		_tempData[4]=recibireth[13];
		_tempData[5]=recibireth[14];
		_tempData[6]=recibireth[15];
		testSet (_tempNumero, _tempActiva, &_tempData[0]);		
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}	
	
	if ((_comL==0xA0) && (_lenghtRx==0x00))
	{
		qSNRead(0, &ipDest[0]);		
	}
	
	
}	