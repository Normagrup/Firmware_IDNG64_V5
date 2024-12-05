#include "config.h"
#include "rtcc.h"
#include "daliLoop.h"
#include "polling.h"
#include "historico.h"
#include "ethTransceiver.h"
#include "ethFramming.h"
#include "normalink.h"
#include "wdt.h"
#include "utils.h"
#include "mainCfg.h"

extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];	//Ip del host que me envía el paquete
extern unsigned int portDest;		//Puerto del host que me envía el paquete



void normaLinkCmdPasarelaT5(unsigned char _comL)
{
	unsigned short int _lenghtRx;
	unsigned char _respuesta[268];
	_lenghtRx=(recibireth[6]*256)+recibireth[7];
	
	if ((_comL==0xA0) && (_lenghtRx==2))		// LEER FLASH [SECTOR][PAGINA]
	{		
		_respuesta[0]=0x84;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];			
		_respuesta[3]=1;								// Lenght HIGH.
		_respuesta[4]=2;								// Lenght LOW.
		_respuesta[5]=recibireth[8];		// Rebote SECTOR.
		_respuesta[6]=recibireth[9];		// Rebote PÁGINA.			
		if (recibireth[8]<8)
		{		
			m25pReadBytes(recibireth[8], recibireth[9],0,&_respuesta[7],256);	
			ethSendFrame(0,&_respuesta[0],263,&ipDest[0], portDest);		
		}		
	}
	
	if ((_comL==0xA1) && (_lenghtRx==258))		// GRABAR FLASH.	[SECTOR][PÁGINA][CONTENIDO].
	{
		unsigned char status;			
		m25pWriteEnable();
		m25pWriteBytes(recibireth[8],recibireth[9],0,&recibireth[10],256);	
		do
		{		
			status=m25pStatusRead();
		}while((status&0x01)==0x01);	
		_respuesta[0]=0x84;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];			
		_respuesta[3]=0;								// Lenght HIGH.
		_respuesta[4]=3;								// Lenght LOW.
		_respuesta[5]=recibireth[8];		// Rebote SECTOR.
		_respuesta[6]=recibireth[9];		// Rebote PÁGINA.	
		_respuesta[7]=0x01;							// Hecho.
		ethSendFrame(0,&_respuesta[0],8,&ipDest[0], portDest);	
	}
	
	if ((_comL==0xA2) && (_lenghtRx==1))		// BORRAR SECTOR [SECTOR].
	{		
		unsigned char status;			
		if (recibireth[7]<8)
		{		
			m25pWriteEnable();
			m25pSectorErase (recibireth[7],0,0);
			do
			{		
				status=m25pStatusRead();
			}while((status&0x01)==0x01);
			_respuesta[0]=0x84;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];			
			_respuesta[3]=0;								// Lenght HIGH.
			_respuesta[4]=2;								// Lenght LOW.
			_respuesta[5]=recibireth[8];		// Rebote SECTOR.
			_respuesta[6]=0x01;							// Hecho.
			ethSendFrame(0,&_respuesta[0],7,&ipDest[0], portDest);				
		}			
	}
	
	
	
	

	#ifdef	DEBUG
	if ((_comL==0xA3) && (_lenghtRx==6))		// LEER RAM		POSICION MEMORIA 4 bytes  0x200000C0 a 0x20001FFF	 size
	{
		// >> 54 43 97 05 00 01 A3 01 A3 00 06 20 00 12 04 00 05 00
		// << 54 43 97 84 01 A3 20 00 12 04 00 05 26 CC 06 11 00 6C
		unsigned int memLoc;
		unsigned char *memPtr;
		unsigned short int size;
		unsigned short int ptr;			
		unsigned short int n;		
		uint32Pop(&recibireth[8], &memLoc, &ptr);		
		uint16Pop(&recibireth[12], &size, &ptr);				
		if (size>256)	size=256;		
		memPtr=(unsigned char*)memLoc;			
		_respuesta[0]=0x84;
		_respuesta[1]=recibireth[2];	
		_respuesta[2]=recibireth[3];			
		_respuesta[3]=recibireth[8];	//LOC
		_respuesta[4]=recibireth[9];	//LOC
		_respuesta[5]=recibireth[10];	//LOC
		_respuesta[6]=recibireth[11];	//LOC
		_respuesta[7]=recibireth[12];	//SIZE
		_respuesta[8]=recibireth[13];	//SIZE
		for (n=0;n<size;n++)
		{
			_respuesta[9+n]=memPtr[n];
		}
		ethSendFrame(0,&_respuesta[0],(9+size),&ipDest[0], portDest);	
	}
	#endif




}






void normaLinkCmdPasarelaT0(unsigned char _comL)
{
	unsigned char _tipoDeFrame;
	unsigned char _respuesta[6];
	
	// [000] REINICIALIZAR PASARELA
	if (_comL==0x00) 
	{
		wdtReboot();
	ethSendACK(&recibireth[2],&ipDest[0]);		
	}	
	
		// [001] REINICIALIZAR CONFIGURACION DE RED
	if (_comL==0x01) 
	{
		ethTransceiverReset();
	ethSendACK(&recibireth[2],&ipDest[0]);	
	}	
		
	// [025] ACCESO EXCLUSIVO A BUS
	if (_comL==0x25) 
	{
		if (normaLinkDaliAccesoExclusivoABusEnable(&ipDest[0])==0x01)
		{
	ethSendACK(&recibireth[2],&ipDest[0]);			
		}
		else
		{
			_tipoDeFrame=0x82;			
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0;
			_respuesta[4]=0;	
			ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
		}

	}	
	// [026] FIN ACCESO EXCLUSIVO A BUS
	if (_comL==0x26) 
	{
		normaLinkDaliAccesoExclusivoABusDisable();
	ethSendACK(&recibireth[2],&ipDest[0]);			
	}	
	// [025] ESTADO BLOQUEO ACCESO A BUS
	// [026] ESTADO ACCESO EXCLUSIVO A BUS
	
	if (_comL==0xFF)																		// PING
	{
		_tipoDeFrame=0x82;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0x01;
		_respuesta[4]=0x07;		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}
}


