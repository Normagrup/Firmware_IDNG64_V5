#include "normalink.h"
#include "config.h"
#include "rtcc.h"
#include "config.h"
#include "historico.h"
#include "storage.h"
#include "ethFramming.h"
#include "mainCfg.h"

#ifndef TIME_S
	typedef struct time_s
	{
			unsigned char hours;
			unsigned char minutes;
			unsigned char seconds;
	} Time;
#define	TIME_S
#endif

#ifndef DATE_S
	typedef struct date_s
	{
		unsigned char day;
		unsigned char weekday;
		unsigned char month;
		unsigned char year;
	} Date;
#define	DATE_S
#endif

extern unsigned char recibireth[320];		// Búffer para paquete entrante
extern unsigned char ipDest[4];	//Ip del host que me envía el paquete
extern unsigned int portDest;		//Puerto del host que me envía el paquete
extern unsigned char macAdrress[6];


void normaLinkCfgInfoPasarelaT4(unsigned char _comL)
{
	unsigned char _tipoDeFrame;
	unsigned char _data[16];
	unsigned char _respuesta[272];
	unsigned char _lenght;
	unsigned short int ptr=0;	
	_lenght=recibireth[6];	
	if ((_comL==0x01) && (_lenght==0x03))
	{
		unsigned char ipAddress[4];		
		arrayPop (&recibireth[7], &ipAddress[0], &ptr, 4);		
		configIpAddressSet(&ipAddress[0], 0x01);
		ethSendACK(&recibireth[2],&ipDest[0]);	
	}	
	if ((_comL==0x03) && (_lenght==0x03))
	{
		unsigned char subMask[4];
		arrayPop (&recibireth[7], &subMask[0], &ptr, 4);				
		configSubmaskAddressSet(&subMask[0], 0x01);
		ethSendACK(&recibireth[2],&ipDest[0]);				
	}	
	if ((_comL==0x05) && (_lenght==0x03))
	{
		unsigned char gtwAddress[4];
		arrayPop (&recibireth[7], &gtwAddress[0], &ptr, 4);		
		configGatewayAddressSet(&gtwAddress[0], 0x01);		
		ethSendACK(&recibireth[2],&ipDest[0]);			
	}	
	if ((_comL==0x11) && (_lenght==0x0F))
	{
		unsigned char nombreEdificio[16];
		arrayPop (&recibireth[7], &nombreEdificio[0], &ptr, 16);	
		configNombreEdificioSet(&nombreEdificio[0], 0x01);
		ethSendACK(&recibireth[2],&ipDest[0]);		
	}	
	if ((_comL==0x21) && (_lenght==0x0F))
	{
		unsigned char nombreLinea[16];
		arrayPop (&recibireth[7], &nombreLinea[0], &ptr, 16);	
		configNombreLineaSet(&nombreLinea[0], 0x01);
		ethSendACK(&recibireth[2],&ipDest[0]);	
	}	
	if ((_comL==0x31) && (_lenght==0x03))
	{
		Date date;			
		date.weekday=recibireth[7];
		date.day=recibireth[8];
		date.month=recibireth[9];
		date.year=recibireth[10];
		rtccDateSet(date);			
		ethSendACK(&recibireth[2],&ipDest[0]);					
	}	
	if ((_comL==0x33) && (_lenght==0x02))
	{
		Time time;
		time.hours=recibireth[7];
		time.minutes=recibireth[8];
		time.seconds=recibireth[9];			
		rtccTimeSet(time);			
		ethSendACK(&recibireth[2],&ipDest[0]);			
	}	
	if ((_comL==0x51) && (_lenght==0x0F))
	{
		unsigned char passAdmin[16];		
		arrayPop (&recibireth[7], &passAdmin[0], &ptr, 16);	
		configAdminPassSet(&passAdmin[0], 0x01);		
		ethSendACK(&recibireth[2],&ipDest[0]);	
	}	
	if ((_comL==0x55) && (_lenght==0x0F))
	{
		unsigned char passMantenedor[16];		
		arrayPop (&recibireth[7], &passMantenedor[0], &ptr, 16);	
		configMantenedorPassSet(&passMantenedor[0], 0x01);
		ethSendACK(&recibireth[2],&ipDest[0]);	
	}	
	if ((_comL==0x84) && (_lenght==0x01))
	{
		unsigned short int events2read;
		unsigned short int readableEvents;
		unsigned short int n;
		unsigned char _dataTx[16];				
		events2read=(recibireth[7]*256)+recibireth[8];		
		readableEvents=historicoReadableEventsCntGet();	
		wdtReloadEnable();		
		if (events2read>readableEvents)	events2read=readableEvents;				
		for (n=0;n<events2read;n++)
		{
			if (historicoRead (n, &_respuesta[4])==0x01)
			{
				_tipoDeFrame=0x83;
				_respuesta[0]=_tipoDeFrame;
				_respuesta[1]=recibireth[2];
				_respuesta[2]=recibireth[3];	
				_respuesta[3]=0x0F;
				ethSendFrame(0,&_respuesta[0],20,&ipDest[0], portDest);	
				wdtReload();
			}			
			else
			{
				n=events2read;
			}					
		}		
	}	
	if ((_comL==0xA3) && (_lenght==0x00))	// Leer
	{
		unsigned char readBuffer[256];
		unsigned char dataRx[2];	
		unsigned int n;		
		dataRx[0]=recibireth[7];
		if (storageRead (&readBuffer[0], (dataRx[0]+1))==0x01)
		{
			_tipoDeFrame=0x83;			
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=dataRx[0];
			for (n=0;n<=dataRx[0];n++)
			{
				_respuesta[(4+n)]=readBuffer[n];
			}		
			ethSendFrame(0,&_respuesta[0],(dataRx[0]+5),&ipDest[0], portDest);			
		}
		else
		{
			_tipoDeFrame=0x82;
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x00;
			_respuesta[4]=0x00;		
			ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);		
		}
	}		
	if (_comL==0xA4)	// Escribir
	{		
		unsigned char dataRx[256];	
		unsigned int n;	
		for (n=0;n<=_lenght;n++)
		{
			dataRx[n]=recibireth[7+n];			
		}		
		if (storageWrite (&dataRx[0],(_lenght+1))==0x01)
		{
			ethSendACK(&recibireth[2],&ipDest[0]);					
		}
		else
		{
			_tipoDeFrame=0x82;
			_respuesta[0]=_tipoDeFrame;
			_respuesta[1]=recibireth[2];
			_respuesta[2]=recibireth[3];	
			_respuesta[3]=0x00;
			_respuesta[4]=0x00;
			ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);		
		}		
	}		
	if ((_comL==0xC1) && (_lenght==0xFF))	// Escribir en bootloader
	{
		unsigned char dataRx[256];	
		unsigned int n;	
		for (n=0;n<=256;n++)
		{
			dataRx[n]=recibireth[7+n];			
		}			
		eBootloaderTransceiver_Write (&dataRx[0]);			
		ethSendACK(&recibireth[2],&ipDest[0]);	
	}
	
	if ((_comL==0x46) && (_lenght==1))
	{
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];			
		_respuesta[4]=0x00;		
		if ((recibireth[7]==MODEL_H) && (recibireth[8]==MODEL_L))
		{
			_respuesta[3]=0x01;
		}
		else
		{
			_respuesta[3]=0x00;
		}		
		board_i2c_write_bootloader_status(0x01);			
		wdtReboot();
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}
}

void normaLinkCfgInfoPasarelaT0(unsigned char _comL)
{
	unsigned char _respuesta[21];
	unsigned char _tipoDeFrame;
	unsigned char _lenght;
	unsigned char _data[16];
	
	unsigned char ipAddress[4];
	unsigned char submaskAddress[4];
	unsigned char gatewayAddress[4];	
	unsigned char nombreEdificio[16];
	unsigned char nombreLinea[16];
	unsigned char passAdmin[16];
	unsigned char passMantenedor[16];		
	unsigned short int ptr=0;
	
	if (_comL==0x00)
	{
		configIpAddressGet(&ipAddress[0]);
		_respuesta[0]=0x83;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=0x03;
		ptr=4;					
		arrayPush(&ipAddress[0],&_respuesta[ptr],&ptr,4);
		ethSendFrame(0,&_respuesta[0],ptr,&ipDest[0], 2234);				
	}	
	if (_comL==0x02)
	{
		configSubmaskAddressGet(&submaskAddress[0]);
		_respuesta[0]=0x83;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=0x03;
		ptr=4;					
		arrayPush(&submaskAddress[0],&_respuesta[ptr],&ptr,4);
		ethSendFrame(0,&_respuesta[0],ptr,&ipDest[0], 2234);						
	}	
	if (_comL==0x04)
	{		
		configGatewayAddressGet(&gatewayAddress[0]);
		_respuesta[0]=0x83;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=0x03;
		ptr=4;					
		arrayPush(&gatewayAddress[0],&_respuesta[ptr],&ptr,4);
		ethSendFrame(0,&_respuesta[0],ptr,&ipDest[0], 2234);				
	}	
	if (_comL==0x08)
	{
		_respuesta[0]=0x83;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=0x05;
		ptr=4;					
		arrayPush(&macAdrress[0],&_respuesta[ptr],&ptr,6);
		ethSendFrame(0,&_respuesta[0],ptr,&ipDest[0], 2234);		
	}	
	if (_comL==0x10)
	{
		configNombreEdificioGet(&nombreEdificio[0]);	
		_respuesta[0]=0x83;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=0x0F;
		ptr=4;					
		arrayPush(&nombreEdificio[0],&_respuesta[ptr],&ptr,16);
		ethSendFrame(0,&_respuesta[0],ptr,&ipDest[0], 2234);
	}	
	if (_comL==0x20)
	{
		configNombreLineaGet(&nombreLinea[0]);	
		_respuesta[0]=0x83;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=0x0F;
		ptr=4;					
		arrayPush(&nombreLinea[0],&_respuesta[ptr],&ptr,16);
		ethSendFrame(0,&_respuesta[0],ptr,&ipDest[0], 2234);
	}	
	if (_comL==0x30)
	{
		Date date;		
		date=rtccDateGet();		
		_tipoDeFrame=0x83;
		_lenght=0x03;
		_data[3]=date.weekday;
		_data[2]=date.day;
		_data[1]=date.month;
		_data[0]=date.year;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[3];
		_respuesta[5]=_data[2];
		_respuesta[6]=_data[1];
		_respuesta[7]=_data[0];		
		ethSendFrame(0,&_respuesta[0],8,&ipDest[0], portDest);	
	}	
	if (_comL==0x32)
	{
		Time time;
		time=rtccTimeGet();
		_tipoDeFrame=0x83;
		_lenght=0x02;
		_data[2]=time.hours;
		_data[1]=time.minutes;
		_data[0]=time.seconds;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[2];
		_respuesta[5]=_data[1];
		_respuesta[6]=_data[0];
		ethSendFrame(0,&_respuesta[0],7,&ipDest[0], portDest);	
	}
	if (_comL==0x40)
	{
		_tipoDeFrame=0x83;
		_lenght=0x01;
		_data[0]=MODEL_H;	// MODEL_HH
		_data[1]=MODEL_L;	// MODEL_LOW
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[0];
		_respuesta[5]=_data[1];
		ethSendFrame(0,&_respuesta[0],6,&ipDest[0], portDest);			
	}	
	if (_comL==0x42)
	{
		_tipoDeFrame=0x83;
		_lenght=0x01;
		_data[0]=FW_VERSION_MAJOR;	// FW_HIGH
		_data[1]=FW_VERSION_MINOR;	// FW_LOW
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[0];
		_respuesta[5]=_data[1];
		ethSendFrame(0,&_respuesta[0],6,&ipDest[0], portDest);	
	}	
	if (_comL==0x43)
	{		
		_tipoDeFrame=0x83;
		_lenght=0x00;
		_data[0]=daliTransceiverGetFwVer();
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[0];		
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}		
	if (_comL==0x50)
	{
		_tipoDeFrame=0x83;
		_lenght=0x0F;		
		configAdminPassGet(&passAdmin[0]);
		_data[15]=passAdmin[0];
		_data[14]=passAdmin[1];
		_data[13]=passAdmin[2];
		_data[12]=passAdmin[3];
		_data[11]=passAdmin[4];
		_data[10]=passAdmin[5];
		_data[9]=passAdmin[6];
		_data[8]=passAdmin[7];
		_data[7]=passAdmin[8];
		_data[6]=passAdmin[9];
		_data[5]=passAdmin[10];
		_data[4]=passAdmin[11];
		_data[3]=passAdmin[12];
		_data[2]=passAdmin[13];
		_data[1]=passAdmin[14];
		_data[0]=passAdmin[15];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[15];
		_respuesta[5]=_data[14];
		_respuesta[6]=_data[13];
		_respuesta[7]=_data[12];
		_respuesta[8]=_data[11];
		_respuesta[9]=_data[10];		
		_respuesta[10]=_data[9];		
		_respuesta[11]=_data[8];		
		_respuesta[12]=_data[7];		
		_respuesta[13]=_data[6];		
		_respuesta[14]=_data[5];		
		_respuesta[15]=_data[4];		
		_respuesta[16]=_data[3];		
		_respuesta[17]=_data[2];		
		_respuesta[18]=_data[1];		
		_respuesta[19]=_data[0];		
		ethSendFrame(0,&_respuesta[0],20,&ipDest[0], portDest);	
	}	
	if (_comL==0x54)
	{
		_tipoDeFrame=0x83;
		_lenght=0x0F;		
		configMantenedorPassGet(&passMantenedor[0]);
		_data[15]=passMantenedor[0];
		_data[14]=passMantenedor[1];
		_data[13]=passMantenedor[2];
		_data[12]=passMantenedor[3];
		_data[11]=passMantenedor[4];
		_data[10]=passMantenedor[5];
		_data[9]=passMantenedor[6];
		_data[8]=passMantenedor[7];
		_data[7]=passMantenedor[8];
		_data[6]=passMantenedor[9];
		_data[5]=passMantenedor[10];
		_data[4]=passMantenedor[11];
		_data[3]=passMantenedor[12];
		_data[2]=passMantenedor[13];
		_data[1]=passMantenedor[14];
		_data[0]=passMantenedor[15];
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[15];
		_respuesta[5]=_data[14];
		_respuesta[6]=_data[13];
		_respuesta[7]=_data[12];
		_respuesta[8]=_data[11];
		_respuesta[9]=_data[10];		
		_respuesta[10]=_data[9];		
		_respuesta[11]=_data[8];		
		_respuesta[12]=_data[7];		
		_respuesta[13]=_data[6];		
		_respuesta[14]=_data[5];		
		_respuesta[15]=_data[4];		
		_respuesta[16]=_data[3];		
		_respuesta[17]=_data[2];		
		_respuesta[18]=_data[1];		
		_respuesta[19]=_data[0];		
		ethSendFrame(0,&_respuesta[0],20,&ipDest[0], portDest);			
	}	
	if (_comL==0x80)		// Obtiene el numero de eventos leibles
	{
		unsigned int _historicoCnt;
		_tipoDeFrame=0x83;
		_lenght=0x01;				
		_historicoCnt=historicoReadableEventsCntGet();		
		_data[1]=(unsigned char)(_historicoCnt/0x0100);
		_data[0]=(unsigned char)(_historicoCnt&0x00FF);
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[1];
		_respuesta[5]=_data[0];		
		ethSendFrame(0,&_respuesta[0],6,&ipDest[0], portDest);		
	}	
	if (_comL==0x88) 
	{
		historicoReset();
		ethSendACK(&recibireth[2],&ipDest[0]);		
	}	
	if (_comL==0xA0)	// Iniciar R
	{
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[4]=0x00;		
		if (storageReadStart()==0x01)
		{
			_respuesta[3]=0x01;
		}
		else
		{
			_respuesta[3]=0x00;
		}	
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);	
	}	
	if (_comL==0xA1)	// Iniciar W
	{	
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[4]=0x00;
		if (storageWriteStart()==0x01)
		{
			_respuesta[3]=0x01;
		}
		else
		{
			_respuesta[3]=0x00;
		}
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}	
	if (_comL==0xA2)	// Obtener ID de Modificaciones
	{
		unsigned int storageId;				
		_tipoDeFrame=0x83;
		_lenght=0x01;				
		storageId=storageIdGet();
		_data[1]=(unsigned char)(storageId/0x0100);
		_data[0]=(unsigned char)(storageId&0x00FF);
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];
		_respuesta[3]=_lenght;
		_respuesta[4]=_data[1];
		_respuesta[5]=_data[0];
		ethSendFrame(0,&_respuesta[0],6,&ipDest[0], portDest);			
	}	
	if (_comL==0xA6)	// Finalizar R/W
	{
		storageReadWriteFinish();
		ethSendACK(&recibireth[2],&ipDest[0]);			
	}		
	if (_comL==0xA6)	// Finalizar R/W
	{
		storageReadWriteFinish();
		ethSendACK(&recibireth[2],&ipDest[0]);		
	}		
	if (_comL==0xC0) 	// Iniciar Embedded Bootloader Write
	{		
		eBootloaderTransceiver_WriteStart();		
		ethSendACK(&recibireth[2],&ipDest[0]);		
	}	
	
	if (_comL==0xC2)	// Iniciar Embedded Bootloader Read
	{
		eBootloaderTransceiver_ReadStart();
		ethSendACK(&recibireth[2],&ipDest[0]);			
	}	
	
	if (_comL==0xC3)	// Leer
	{
		unsigned char readBuffer[256];
		unsigned int n;		
		eBootloaderTransceiver_Read(&readBuffer[0]);		
		_tipoDeFrame=0x83;		
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[3]=0xFF;
		for (n=0;n<256;n++)
		{
			_respuesta[(4+n)]=readBuffer[n];
		}	
		ethSendFrame(0,&_respuesta[0],261,&ipDest[0], portDest);					
	}	
	
	if (_comL==0xC4)	// Go
	{
		_tipoDeFrame=0x82;
		_respuesta[0]=_tipoDeFrame;
		_respuesta[1]=recibireth[2];
		_respuesta[2]=recibireth[3];	
		_respuesta[4]=0x00;			
		if (eBootloaderTransceiver_Go()==0x01)
		{
			_respuesta[3]=0x01;
		}
		else
		{
			_respuesta[3]=0x00;
		}			
		ethSendFrame(0,&_respuesta[0],5,&ipDest[0], portDest);			
	}	
	if (_comL==0x46)
	{
		board_i2c_write_bootloader_status(0x01);			
		wdtReboot();
		ethSendACK(&recibireth[2],&ipDest[0]);
	}
}


