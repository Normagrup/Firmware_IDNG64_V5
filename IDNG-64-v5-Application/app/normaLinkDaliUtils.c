// 01 00 00 00 FF E4 00 00			Reboot
// 01 00 00 00 FF E4 10 00			daliBootloaderModelHGet
// 01 00 00 00 FF E4 11 00			daliBootloaderModelMGet
// 01 00 00 00 FF E4 12 00			daliBootloaderVersionGet
// 01 00 00 00 FF E4 13 00			daliBootloaderStatusGet
// 03 00 00 00 FF E4 10 DD 00 	daliBootloaderModelHSet
// 03 00 00 00 FF E4 11 DD 00 	daliBootloaderModelMSet
// 03 00 00 00 FF E4 12 DD 00 	daliBootloaderVersionSet
// 03 00 00 00 FF E4 13 DD 00 	daliBootloaderStatusSet





#include "normalink.h"
#include "ethFramming.h"
#include "daliUtils.h"


extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];	//Ip del host que me envía el paquete
extern unsigned int portDest;		//Puerto del host que me envía el paquete

void normaLinkDaliUtilsT3 (unsigned char comL, unsigned char dest, unsigned char data)
{
	unsigned char _tipoDeFrame;
	unsigned char _respuesta[6];	
	
	// B o o t l o a d e r   M o d e l  H  S e t 
	if (comL==0x10)
	{
		daliUtilsBootloaderModelHSet(dest,data);
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	// B o o t l o a d e r 	M o d e l	 L  S e t 
	if (comL==0x11)
	{
		daliUtilsBootloaderModelLSet(dest,data);
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
	}
	
	// B o o t l o a d e r	V e r s i o n		S e t 
	if (comL==0x12)
	{
		daliUtilsBootloaderVersionSet(dest,data);
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
	}
	
	// B o o t l o a d e r	S t a t u s 	S e t 
	if (comL==0x13)
	{
		daliUtilsBootloaderStatusSet(dest,data);
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
	}	
	
}

void normaLinkDaliUtilsT4 (unsigned char comL)
{
	unsigned char _tipoDeFrame;
	unsigned char _respuesta[6];
	// w r i t e     p a g e
	if (comL==0x14)
	{
		unsigned char pageContent[130];
		unsigned char n;	
		for (n=0;n<130;n++)
		{
			pageContent[n]=recibireth[(136-n)];			
		}
		daliUtilsBootloaderWritePage(&pageContent[0]);	
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);					
	}
	
		// DETENER TRANSCEPTOR.
	if (comL==0x40)
	{	
//		_daliStop=0x01;
		
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
		
	}
	
	// REANUDAR TRANSCEPTOR.
	if (comL==0x41)
	{
//		_daliStop=0x00;
		
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	// PATILLA AVR BOOT A 0 [ACTIVE].
	if (comL==0x42)
	{
		
		boardAVRBootON();
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	// PATILLA AVR BOOT A 1 [INACTIVE].
	if (comL==0x43)
	{
		boardAVRBootOFF();
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	// GRABAR DATOS
	if (comL==0x44)
	{
		unsigned char n;	
		for (n=0;n<130;n++)
		{
			spi1Write(0xFB, recibireth[(136-n)]);
		}			
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	// CONSULTAR ESTADO BOOTLOADER.
	if (comL==0x45)
	{
		unsigned char bootStatus;		
		bootStatus=spi1Read(0xFA);		
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=bootStatus;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
	}	
	
	// REINICIAR TRANSCEIVER
	if (comL==0x46)
	{
		spi1Write(0xFF, 0xA0);	
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);				
	}	
	
}

void normaLinkDaliUtilsT1 (unsigned char comL, unsigned char dest)
{
	unsigned char _tipoDeFrame;
	unsigned char _valid;
	unsigned char _answer;
	unsigned char _respuesta[6];
	
	if (comL==0x00)
	{
		daliUtilsRebootSend (dest);
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);					
	}
	
	if (comL==0x10)
	{
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];			
		if (daliUtilsBootloaderModelHGet(dest,&_valid,&_answer)==0x01)
		{
			_respuesta[3]=_valid;
			_respuesta[4]=_answer;			
		}
		else
		{
			_respuesta[3]=0x00;
			_respuesta[4]=0x00;
		}
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);					
	}
	
	if (comL==0x11)
	{
		daliUtilsBootloaderModelLGet(dest,&_valid,&_answer);
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=_valid;
		_respuesta[4]=_answer;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}
	
	if (comL==0x12)
	{
		daliUtilsBootloaderVersionGet(dest,&_valid,&_answer);
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=_valid;
		_respuesta[4]=_answer;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);					
	}
	
	if (comL==0x13)
	{
		daliUtilsBootloaderStatusGet(dest,&_valid,&_answer);
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=_valid;
		_respuesta[4]=_answer;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}	
	
	if (comL==0x20)
	{
		if (daliUtilsBlinkSend(dest,3000)==0x01)
		{
			_tipoDeFrame=0x82;
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x01;
			_respuesta[4]=0x00;
		}
		else
		{
			_tipoDeFrame=0x82;
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x00;
			_respuesta[4]=0x00;
		}
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}		
	
	if (comL==0x21)
	{
		daliUtilsBlinkTerminate ();
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x00;
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}		
}
