/*

05/02/2019	SOPORTE FUNCIÓN CORRIDOR.

*/




#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT

#define INC_SENSORES_PRT
    #include "sensores.h"
#undef INC_SENSORES_PRT




#define INC_SENSORES_GRUPOS_PRT
    #include "sensoresGrupos/sensoresGrupos.h"
#undef INC_SENSORES_GRUPOS_PRT

#include "ethFramming.h"



#include "dataTypes.h"

#include "utils.h"
#include "mainCfg.h"

#define DEBUG_SENSORS_INTERFACE_RECEIVED
#define CONECTAR_CON_ETHERNET

		unsigned char _debugTransceptorLiberado;
    extern unsigned char recibireth[320];		// Búffer para paquete entrante
		extern unsigned char ipDest[4];	//Ip del host que me envía el paquete
		extern unsigned char _configIpAddress[4];
		extern unsigned char byte[MAX_SENSORES_NORMALINK][4];    
		extern unsigned char byteNotifPending[MAX_SENSORES_NORMALINK];
		extern unsigned char _interfacesInstaladosCnt;
		unsigned char notifPending;
		extern unsigned char				interfazCalculoPendiente[16];			// Por cada interfaz; Me han indicado que se ha producido un cambio por tanto iniciaré la consulta.
		extern unsigned char				interfazCalculoPendienteCnt[16];	// Por cada interfaz; Como no sé cuantos equipos tienen mi interfaz lo cierro por timeout.
		//extern unsigned short int		interfazCalculoEquipos[16];				// Por cada interfaz
		//extern unsigned short int		interfazCalculoApagados[16];			// Por cada interfaz
		//extern unsigned short int 	interfazCalculoNivelAcu[16];			// Por cada interfaz
		unsigned char 							_normaLinkSensoresEnviar;
		interfacesNumeroSerie 			_numeroSerie;
		interfacesEventos 					_evento;
		
		
		extern unsigned char _broadcastAddress[4];		
		extern unsigned char _normaLinkMasterIp[4];
		
		
		#ifdef DEBUG_SENSORS_INTERFACE_RECEIVED
			unsigned char _debugInterfaceReceived[4];
		#endif

void normaLinkSensoresDebugTransceptorLiberado(void)
{
	_debugTransceptorLiberado=1;
	
}

void normaLinkSensoresDebugInicializarIDs(void)
{
			unsigned char respuestaEth[6];
			respuestaEth[0]='I';
			respuestaEth[1]='d';
			respuestaEth[2]='I';
			respuestaEth[3]='N';
			respuestaEth[4]='T';			
			//ethSendFrame(0,&respuestaEth[0],5,&_normaLinkSensoresBcAddress[0], 2234);
	
}
		
void normaLinkSensoresEnviar (interfacesNumeroSerie numeroSerie, interfacesEventos evento)
{
  	if (_normaLinkSensoresEnviar==0)
		{	
			_normaLinkSensoresEnviar=1;
			_numeroSerie=numeroSerie;
			_evento=evento;
		}
}
static void normaLinkSensoresEventoProcesar (interfacesNumeroSerie numeroSerie, interfacesEventos evento)
{
    sensoresOrigenNotificacion origenNotificacion;
    origenNotificacion.tipoOrigen=SENSORES_ORIGEN_NOTIFICACION_NORMALINK;
    origenNotificacion.interfazNumeroSerie=numeroSerie;
    origenNotificacion.interfazCanal=0xFF;
    origenNotificacion.interfazAddressOrigen=0xFF;
    sensoresNotificar (origenNotificacion, evento);
}


void normaLinkSensoresDoTask (void)
{	
	unsigned char ethFrame[20];	
	unsigned short int ptr=0;	
	unsigned char ip[4];
	unsigned char n;	
	
	unsigned char *ipPtr;

	interfacesConfiguracion	interfazConfiguracion;
	
	if (_normaLinkSensoresEnviar==0x01)
	{		
		bytePush(0x04,&ethFrame[ptr],&ptr);															
		bytePush(0x00,&ethFrame[ptr],&ptr);		
		bytePush(0xFF,&ethFrame[ptr],&ptr);
		bytePush(0xFF,&ethFrame[ptr],&ptr);
		bytePush(0x77,&ethFrame[ptr],&ptr);		
		bytePush(0x00,&ethFrame[ptr],&ptr);		
		bytePush(0x07,&ethFrame[ptr],&ptr);				
		arrayPush(&_numeroSerie.byte[0],&ethFrame[ptr],&ptr,4);									//4
		bytePush((((_evento.canal & 0x0F)<<4)+((_evento.eventoTipo & 0x0F)<<0)),&ethFrame[ptr],&ptr);												//5		
		bytePush(_evento.posicionEncender,&ethFrame[ptr],&ptr);									//6
		bytePush(_evento.sentidoAscendente,&ethFrame[ptr],&ptr);								//7
		bytePush(_evento.lectura,&ethFrame[ptr],&ptr);													//8		
		if ((_evento.eventoTipo==INTERFACES_EVENTO_SHORT_PRESS) && (!((_normaLinkMasterIp[0]==0) && (_normaLinkMasterIp[1]==0) && (_normaLinkMasterIp[2]==0) && (_normaLinkMasterIp[3]==0))))
		{
			ipPtr=&_normaLinkMasterIp[0];		
		}
		else
		{
			ipPtr=&_broadcastAddress[0];
		}		
		ethSendFrame(0,&ethFrame[0],ptr,ipPtr, 2234);		
		_normaLinkSensoresEnviar=0x00;
	}	
	
	if (notifPending==0x01)
	{
		ip[0]=_configIpAddress[0];
		ip[1]=_configIpAddress[1];
		ip[2]=_configIpAddress[2];
		ip[3]=0xFF;		
		for (n=0;n<64;n++)
		{
			if (byteNotifPending[n]==0x01)
			{				
				ptr=0;
				bytePush(0x04,&ethFrame[ptr],&ptr);	
				bytePush(0x00,&ethFrame[ptr],&ptr);		
				bytePush(0xFF,&ethFrame[ptr],&ptr);
				bytePush(0xFF,&ethFrame[ptr],&ptr);
				bytePush(0x77,&ethFrame[ptr],&ptr);	
				bytePush(0x88,&ethFrame[ptr],&ptr);	
				bytePush(0x03,&ethFrame[ptr],&ptr);			
				arrayPush(&byte[n][0],&ethFrame[ptr],&ptr,4);			
				ethSendFrame(0,&ethFrame[0],ptr,&_broadcastAddress[0], 2234);				
				byteNotifPending[n]=0;
				
			}
		}
		notifPending=0;		
	}	
		
	for (n=0;n<_interfacesInstaladosCnt;n++)
	{	
		if (interfazCalculoPendiente[n]==0x01)
		{		
			interfazCalculoPendienteCnt[n]=100;
//			interfazCalculoEquipos[n]=0;
//			interfazCalculoApagados[n]=0;
//			interfazCalculoNivelAcu[n]=0;		
			interfazConfiguracion=interfacesConfiguracionGet(n);			
			ptr=0;
			bytePush(0x04,&ethFrame[ptr],&ptr);	
			bytePush(0x00,&ethFrame[ptr],&ptr);		
			bytePush(0xFF,&ethFrame[ptr],&ptr);
			bytePush(0xFF,&ethFrame[ptr],&ptr);
			bytePush(0x77,&ethFrame[ptr],&ptr);	
			bytePush(0x89,&ethFrame[ptr],&ptr);	
			bytePush(0x03,&ethFrame[ptr],&ptr);						
			arrayPush(&interfazConfiguracion.numeroSerie.byte[0],&ethFrame[ptr],&ptr,4);						
			ethSendFrame(0,&ethFrame[0],ptr,&_broadcastAddress[0], 2234);						
			// envia la peticion broadcast de informacion con el codigo de interfaz.		
			interfazCalculoPendiente[n]=0x02;				
		}
	}

	
}


void normaLinkSensoresSysTick (void)
{
	unsigned char n;	
	
	for (n=0;n<16;n++)
	{	
		if (interfazCalculoPendiente[n]==0x02)	
		{
			if (interfazCalculoPendienteCnt[n]>0)
			{
				interfazCalculoPendienteCnt[n]--;
			}
			else
			{
				interfazCalculoPendiente[n]=0x03;
			}	
		}
	}	
}



void normaLinkSensoresInterfazNotifPendingSet(void)
{
	notifPending=1;
}


void normaLinkSensoresBuscarInterfacesSend (unsigned char *ip, unsigned char p1, unsigned char p2, unsigned char p3, unsigned char p4)
{
		unsigned char ethFrame[8];	
		ethFrame[0]=0x83;	//	FrameType
		ethFrame[1]=0x77;	//	Cmd High
		ethFrame[2]=0x01;	//	Cmd Low
		ethFrame[3]=0x03;	//	Lenght=4
		ethFrame[4]=p1;	//		Va a iniciar ó está haciendo la operación / No va a iniciar o no está haciendo la operación.
		ethFrame[5]=p2;	//		Va a iniciar la operación	/	Está haciendo la operación / Finalizó la operación.
		ethFrame[6]=p3;	//		No aplicable / ok / nok
		ethFrame[7]=p4;	//		no aplicable / numero.
		ethSendFrame(0,&ethFrame[0],8,&ip[0], 2234);	
}





void normaLinkSensores(unsigned char comL)
{
	interfacesNumeroSerie						numeroSerie;  
	interfacesEventos								evento; 	
	unsigned short int 							ptr=0;
	unsigned char										interfazAddress;
	unsigned char										accion;
	unsigned char										grupo;	
	interfacesConfiguracion					interfazConfiguracion;
	sensoresConfiguracion						accionConfiguracion;	
	sensoresGruposConfiguracion			grupoConfiguracion;	
	unsigned char valores[3];
	unsigned char ethFrame[20];		
	
	if (comL==0x00)								// NOTIFICACION INTERFAZ
	{
		// No devuelve ACK.
		arrayPop(&recibireth[ptr+7], &numeroSerie.byte[0],&ptr,4);											//4
		bytePop(&recibireth[ptr+7],(unsigned char*)&evento.eventoTipo,&ptr);						//5
		bytePop(&recibireth[ptr+7],(unsigned char*)&evento.posicionEncender,&ptr);			//6
		bytePop(&recibireth[ptr+7],(unsigned char*)&evento.sentidoAscendente,&ptr);			//7
		bytePop(&recibireth[ptr+7],(unsigned char*)&evento.lectura,&ptr);								//8
		evento.canal=((evento.eventoTipo&0xF0)>>4);		
		evento.eventoTipo=(evento.eventoTipo&0x0F);
											
		#ifdef DEBUG_SENSORS_INTERFACE_RECEIVED
			_debugInterfaceReceived[0]=ipDest[0];
			_debugInterfaceReceived[1]=ipDest[1];
			_debugInterfaceReceived[2]=ipDest[2];
			_debugInterfaceReceived[3]=ipDest[3];
		#endif
		
		normaLinkSensoresEventoProcesar(numeroSerie, evento);		
		
		#ifdef DEBUG_SENSORS_INTERFACE_RECEIVED
			_debugInterfaceReceived[0]=127;
			_debugInterfaceReceived[1]=0;
			_debugInterfaceReceived[2]=0;
			_debugInterfaceReceived[3]=0;
		#endif
	}		
	
	if (comL==0x01)								// BUSCAR INTERFACES
	{		
		daliAddressingNewInitialisationStart(&ipDest[0], 0x01);		//0x01 means EXTENDED DEVICES.
	}	
	
	if (comL==0x02)								// ESCANEA INTERFACES
	{			
			sensorScanningStart (&ipDest[0]);	
	}	
	
	
	
	if (comL==0x03)								// LEER TODOS INTERFACES
	{
		for (interfazAddress=0;interfazAddress<16;interfazAddress++)
		{		
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x08;																													//	Lenght=08
			interfazConfiguracion=interfacesConfiguracionGet(interfazAddress);
			ptr=4;		
			bytePush(interfazAddress,&ethFrame[ptr],&ptr);																		//1
			bytePush((unsigned char)interfazConfiguracion.instalado,&ethFrame[ptr],&ptr);			//2
			bytePush((unsigned char)interfazConfiguracion.desactivado,&ethFrame[ptr],&ptr);		//3
			bytePush((unsigned char)interfazConfiguracion.tipoInterfaz,&ethFrame[ptr],&ptr);	//4
			bytePush((unsigned char)interfazConfiguracion.modoInterfaz,&ethFrame[ptr],&ptr);	//5
			arrayPush(&interfazConfiguracion.numeroSerie.byte[0],&ethFrame[ptr],&ptr,4);			//9		
			ethSendFrame(0,&ethFrame[0],ptr,&ipDest[0], 2234);			
		}	
	}	
	
	if (comL==0x04)								// LEER TODAS ACCIONES
	{		
		for (accion=0;accion<64;accion++)
		{		
			ethFrame[0]=0x83;																																									//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=10;																																									//	Lenght=10			
			accionConfiguracion=sensoresConfiguracionGet(accion);
			ptr=4;					
			bytePush(accion,&ethFrame[ptr],&ptr);																															//1
			bytePush((unsigned char)accionConfiguracion.instalado,&ethFrame[ptr],&ptr);												//2
			bytePush((unsigned char)accionConfiguracion.desactivado,&ethFrame[ptr],&ptr);											//3
			bytePush((unsigned char)accionConfiguracion.tipoSensor,&ethFrame[ptr],&ptr);											//4
			arrayPush(&accionConfiguracion.interfazNumeroSerie.byte[0],&ethFrame[ptr],&ptr,4);								//8				
			bytePush(sensoresMatrizGruposToGrupoGet(accionConfiguracion.matrizGrupos),&ethFrame[ptr],&ptr);		//9	
			bytePush(accionConfiguracion.accion[0][0],&ethFrame[ptr],&ptr);																		//10
			bytePush(accionConfiguracion.accion[0][1],&ethFrame[ptr],&ptr);																		//11
			ethSendFrame(0,&ethFrame[0],ptr,&ipDest[0], 2234);			
		}
	}	

	if (comL==0x05)								// LEER TODOS LOS GRUPOS
	{
		for (grupo=0;grupo<16;grupo++)
		{		
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=10;																													//	Lenght=11
			ptr=4;				
			grupoConfiguracion=sensoresGruposConfiguracionGet(grupo);		
			bytePush(grupo,&ethFrame[ptr],&ptr);
			bytePush(grupoConfiguracion.detectoresPresenciaModo,&ethFrame[ptr],&ptr);		
			uint16Push(grupoConfiguracion.detectoresPresenciaRetardo,&ethFrame[ptr],&ptr);	
			uint16Push(grupoConfiguracion.pulsadoresAccionTemporizadaRetardo,&ethFrame[ptr],&ptr);	
			bytePush(grupoConfiguracion.prevacancyLevel,&ethFrame[ptr],&ptr);					
			uint16Push(grupoConfiguracion.prevacancyPeriodoCnt,&ethFrame[ptr],&ptr);				
			bytePush(grupoConfiguracion.onAction,&ethFrame[ptr],&ptr);
			bytePush(grupoConfiguracion.offAction,&ethFrame[ptr],&ptr);
			ethSendFrame(0,&ethFrame[0],ptr,&ipDest[0], 2234);						
		}
	}

	if (comL==0x06)								// INICIAR CONFIGURACION
	{
		if (sensoresIniciarConfiguracion()==TRUE)
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x00;					
			ethFrame[4]=0x01;					
		}
		else
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x00;					
			ethFrame[4]=0x00;					
		}		
		ethSendFrame(0,&ethFrame[0],5,&ipDest[0], 2234);	
	}
	
	if (comL==0x08)								// CONFIGURAR INTERFAZ N
	{		
		interfazAddress=recibireth[7];
		if (interfacesConfiguracionSet (interfazAddress, &recibireth[8], &ipDest[0], recibireth[2], recibireth[3])==FALSE)
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x01;					
			ethFrame[4]=interfazAddress;		
			ethFrame[5]=0x00;		
			ethSendFrame(0,&ethFrame[0],6,&ipDest[0], 2234);
		}
	}
	
	if (comL==0x09)								// CONFIGURAR ACCION N
	{	
		accion=recibireth[7];		
		if (sensoresConfiguracionSet (accion, &recibireth[8])==TRUE)
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x01;					
			ethFrame[4]=accion;		
			ethFrame[5]=0x01;		
		}
		else
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x01;					
			ethFrame[4]=accion;		
			ethFrame[5]=0x00;		
		}
		ethSendFrame(0,&ethFrame[0],6,&ipDest[0], 2234);	
	}
	
	if (comL==0x0A)								// CONFIGURAR GRUPO N
	{	
		grupo=recibireth[7];		
		if (sensoresGruposConfiguracionSet (grupo, &recibireth[8])==TRUE)
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x01;					
			ethFrame[4]=grupo;		
			ethFrame[5]=0x01;		
		}
		else
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x01;					
			ethFrame[4]=grupo;		
			ethFrame[5]=0x00;		
		}
		ethSendFrame(0,&ethFrame[0],6,&ipDest[0], 2234);	
	}
	
	if (comL==0x0B)								// FINALIZAR CONFIGURACION
	{	
		if (sensoresFinalizarConfiguracion()==TRUE)
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x00;					
			ethFrame[4]=0x01;		
		}
		else
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x00;					
			ethFrame[4]=0x00;		
		}
		ethSendFrame(0,&ethFrame[0],5,&ipDest[0], 2234);	
	}	
	
	if (comL==0x88)								// FINALIZAR CONFIGURACION
	{	
		if (_interfacesInstaladosCnt>0)
		{
			for (interfazAddress=0;interfazAddress<_interfacesInstaladosCnt;interfazAddress++)
			{
				interfazConfiguracion=interfacesConfiguracionGet(interfazAddress);		
				
				if (interfazConfiguracion.instalado==TRUE) 
				{
					if ((interfazConfiguracion.numeroSerie.byte[0]==recibireth[7]) && (interfazConfiguracion.numeroSerie.byte[1]==recibireth[8]) && (interfazConfiguracion.numeroSerie.byte[2]==recibireth[9]) && (interfazConfiguracion.numeroSerie.byte[3]==recibireth[10]))
					{
						if ((interfazCalculoPendiente[interfazAddress]==0x00) || (interfazCalculoPendiente[interfazAddress]==0x03))
						{						
							interfazCalculoPendiente[interfazAddress]=0x01;					
						}
					}					
				}			
			}			
		}		
	}	
	
	if (comL==0xBA)
	{
		if (sensorStoringSetSetpoint (recibireth[7],recibireth[8])==0x01)
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x00;					
			ethFrame[4]=0x01;					
		}
		else
		{
			ethFrame[0]=0x83;																													//	FrameType
			ethFrame[1]=recibireth[2];
			ethFrame[2]=recibireth[3];
			ethFrame[3]=0x00;					
			ethFrame[4]=0x00;					
		}		
		ethSendFrame(0,&ethFrame[0],5,&ipDest[0], 2234);	
	}
	
	if (comL==0xBB)
	{
		sensorStoringQuerySetpoint(recibireth[7],&ipDest[0]);
	}
	
	// [] CMD DALI.		dest / com 
	if (comL==0xC0)
	{		
		unsigned int _packetId;
		_packetId=recibireth[2]*0x0100+recibireth[3];
		//sysTickDelay (10);
		normalinkDaliSend(&ipDest[0],_packetId, 0x16, recibireth[7], 0x2E, recibireth[8]);
	}	
		
	
	
	
	
	
	
	
}
