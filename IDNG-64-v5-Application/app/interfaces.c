#define INTERFACES_DEF
    #include "interfaces.h"
#undef INTERFACES_DEF
#define INC_INTERFACES_NUMERO_SERIE_S
    #include "interfaces.h"
#undef INC_INTERFACES_NUMERO_SERIE_S
#define INC_INTERFACES_MODOS_INTERFAZ_E
    #include "interfaces.h"
#undef INC_INTERFACES_MODOS_INTERFAZ_E
#define INC_INTERFACES_TIPO_INTERFAZ_E
    #include "interfaces.h"
#undef INC_INTERFACES_TIPO_INTERFAZ_E
#define INC_INTERFACES_EVENTOS_TIPOS_E
    #include "interfaces.h"
#undef INC_INTERFACES_EVENTOS_TIPOS_E
#define INC_INTERFACES_EVENTOS_S
    #include "interfaces.h"
#undef INC_INTERFACES_EVENTOS_S
#define INC_INTERFACES_CONFIGURACION_S
    #include "interfaces.h"
#undef INC_INTERFACES_CONFIGURACION_S

#define INC_SENSORES_ORIGEN_NOTIFICACION_S
    #include "sensores.h"
#undef INC_SENSORES_ORIGEN_NOTIFICACION_S

#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT

#define INC_SENSORES_PRT
    #include "sensores.h"
#undef INC_SENSORES_PRT
#include "dataTypes.h"
#include "sensoresConfig.h"
#include "normaLinkSensores.h"
#include "utils.h"

/* Los KEEP_ALIVE son muy importantes en el caso de DETECTORES_PRESENCIA_FLANCO y PULSADORES DE ACCION TEMPORIZADA.
 *
 * Se generarán cuando no hay motivos para establecer comunicación teniendo en cuenta únicamente esas funciones
 * quiere decir que si está mandando LIGHT_REPORTS da igual, generará también el KEEP ALIVE.
 * Si por cualquier otro motivo tampoco se estableció comunicación se generará un KEEP ALIVE igualmente.
 * DET_PRESENCIA modo nivel comunica continuamente, REG_LUZ también.
 */


#define NIVEL_NO_APLICABLE                              	0xFF
/* IDNG-EAD */
#define REGISTRO_ESTADO_BIT_SHORT_PRESS                 	0x01
#define REGISTRO_ESTADO_BIT_LONG_PRESS                  	0x02
#define REGISTRO_ESTADO_BIT_LONG_RELEASE                	0x04
#define REGISTRO_ESTADO_BIT_PRESENCY                    	0x08
#define REGISTRO_ESTADO_BIT_VACANCY                     	0x10
#define REGISTRO_ESTADO_BIT_REPORTE_LUZ                 	0x20

/* IDNG-4P */
#define REGISTRO_ESTADO_BIT_SHORT_PRESS_1                 0x01
#define REGISTRO_ESTADO_BIT_SHORT_PRESS_2                 0x02
#define REGISTRO_ESTADO_BIT_SHORT_PRESS_3                 0x04
//#define REGISTRO_ESTADO_BIT_PRESENCY                    0x08	// Ya definido.
//#define REGISTRO_ESTADO_BIT_VACANCY                     0x10	// Ya definido.
#define REGISTRO_ESTADO_BIT_SHORT_PRESS_4                 0x40


#define REGISTRO_ESTADO_BIT_LONG_PRESS_1									0x01
#define REGISTRO_ESTADO_BIT_LONG_PRESS_2									0x02
#define REGISTRO_ESTADO_BIT_LONG_PRESS_3									0x04
#define REGISTRO_ESTADO_BIT_LONG_PRESS_4									0x08
#define REGISTRO_ESTADO_BIT_LONG_RELEASE_1								0x10
#define REGISTRO_ESTADO_BIT_LONG_RELEASE_2								0x20
#define REGISTRO_ESTADO_BIT_LONG_RELEASE_3								0x40
#define REGISTRO_ESTADO_BIT_LONG_RELEASE_4								0x80



// 1 + 1 + 1 + 1 + 16 + 48 + 48 + 1 + 1 + 4 + 1 + 1 + 16 + 16 = 156 bytes

unsigned char 				_interfazNotificarPendiente;
unsigned char 				_interfazAddress;
unsigned char 				_registroEstado;
unsigned char 				_lectura;
BOOL 									_inhibido[INTERFACES_MAX];
BOOL 									_sentidoAscendente[INTERFACES_MAX][4];	//64
BOOL 									_posicionEncendido[INTERFACES_MAX][4];	//64	
unsigned char 				_buscandoInterfaces;
unsigned char 				_buscandoInterfacesCnt;
unsigned char					_buscandoInterfacesIp[4];
unsigned char 				_interfacesEncontrados;
unsigned char 				_interfacesInstaladosCnt;
unsigned char					interfazCalculoPendiente[16];			// Por cada interfaz; Me han indicado que se ha producido un cambio por tanto iniciaré la consulta.
unsigned char					interfazCalculoPendienteCnt[16];	// Por cada interfaz; Como no sé cuantos equipos tienen mi interfaz lo cierro por timeout.
//unsigned short int		interfazCalculoEquipos[16];				// Por cada interfaz
//unsigned short int		interfazCalculoApagados[16];			// Por cada interfaz
//unsigned short int 		interfazCalculoNivelAcu[16];			// Por cada interfaz


unsigned char _interfacesUnfreezeCnt=30;
unsigned char _interfacesUnfreezeSend;

static void interfacesSolicitar (interfacesConfiguracion configuracionInterfaz, unsigned char interfazAddress, interfacesNumeroSerie numeroSerie, unsigned char registroEstado, unsigned char lectura);
interfacesEventos interfacesIDNGEADEventoGet(interfacesModosInterfaz modoInterfaz, unsigned char *registroEstado, unsigned char lectura, BOOL *posicionEncendido, BOOL *sentidoAscendente);
interfacesEventos interfacesIDNG4PEventoGet(interfacesModosInterfaz modoInterfaz, unsigned char *registroEstado, unsigned char *registroAux, BOOL *posicionEncendido, BOOL *sentidoAscendente);



static void interfacesSolicitar (interfacesConfiguracion configuracionInterfaz, unsigned char interfazAddress, interfacesNumeroSerie numeroSerie, unsigned char registroEstado, unsigned char lectura)
{
    sensoresOrigenNotificacion  origenNotificacion;
    interfacesEventos           evento;	
		origenNotificacion.tipoOrigen=SENSORES_ORIGEN_NOTIFICACION_INTERFAZ;
    origenNotificacion.interfazAddressOrigen=interfazAddress;
    origenNotificacion.interfazNumeroSerie=numeroSerie;			
		do{				
		
			if (configuracionInterfaz.tipoInterfaz==INTERFACES_TIPO_INTERFAZ_IDNGEAD)	evento=interfacesIDNGEADEventoGet(configuracionInterfaz.modoInterfaz, &registroEstado, lectura, &_posicionEncendido[interfazAddress][0], &_sentidoAscendente[interfazAddress][0]);
			if (configuracionInterfaz.tipoInterfaz==INTERFACES_TIPO_INTERFAZ_IDNG4P)	evento=interfacesIDNG4PEventoGet(configuracionInterfaz.modoInterfaz, &registroEstado, &lectura, &_posicionEncendido[interfazAddress][0], &_sentidoAscendente[interfazAddress][0]);				
			if ((evento.eventoTipo!=INTERFACES_EVENTO_NINGUNO) && (evento.eventoTipo!=INTERFACES_EVENTO_KEEP_ALIVE))
			{	
				sensoresNotificar (origenNotificacion, evento);
				normaLinkSensoresEnviar(origenNotificacion.interfazNumeroSerie, evento);
				normaLinkSensoresDoTask(); 		
			}					
		} while (evento.eventoTipo!=INTERFACES_EVENTO_NINGUNO);	
}







BOOL interfacesBuscar (unsigned char *ip)
{	
	return TRUE;
}

static void interfacesConfiguracionArrayFromStructGet (interfacesConfiguracion configuracion, unsigned char *cfg)
{
	unsigned short int ptr=0;	
	bytePush(configuracion.instalado, &cfg[ptr],&ptr);
	bytePush(configuracion.desactivado, &cfg[ptr],&ptr);
	bytePush(configuracion.tipoInterfaz, &cfg[ptr],&ptr);
	bytePush(configuracion.modoInterfaz, &cfg[ptr],&ptr);
	arrayPush(&configuracion.numeroSerie.byte[0], &cfg[ptr],&ptr,4);
}

BOOL interfacesConfiguracionSet (unsigned char interfazAddress, unsigned char *cfg, const unsigned char *interfacesCfgIp, unsigned char packetId_1, unsigned char packetId_2)
{			
	if (sensorStoringStart (interfazAddress, cfg[1], cfg[3],  interfacesCfgIp, packetId_1, packetId_2, &cfg[0])==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
	
	//if ((sensorStoringStart (interfazAddress, cfg[1], cfg[3],  interfacesCfgIp, packetId_1, packetId_2)==1) && (configInterfacesConfiguracionSet(interfazAddress, &cfg[0])==1))
	//{
	//	return 1;
	//}
	//else
	//{
	//	return 0;
	//}
}

interfacesConfiguracion interfacesConfiguracionGet (unsigned char interfazAddress)
{
    interfacesConfiguracion configuracionRet;
		unsigned char arrayCfg[13];
		unsigned short int ptr=0;	
		if (configInterfacesConfiguracionGet(interfazAddress, &arrayCfg[0])==TRUE)
		{
			bytePop(&arrayCfg[ptr],(unsigned char*)&configuracionRet.instalado,&ptr);										//1				
		}
		else
		{
			configuracionRet.instalado=FALSE;
		}		
		if (configuracionRet.instalado==TRUE)
		{
			bytePop(&arrayCfg[ptr],(unsigned char*)&configuracionRet.desactivado,&ptr);									//2
			bytePop(&arrayCfg[ptr],(unsigned char*)&configuracionRet.tipoInterfaz,&ptr);								//3
			bytePop(&arrayCfg[ptr],(unsigned char*)&configuracionRet.modoInterfaz,&ptr);								//4
			arrayPop(&arrayCfg[ptr], &configuracionRet.numeroSerie.byte[0],&ptr,4);											//8			
		}
		else
		{
			configuracionRet.instalado=FALSE;
			configuracionRet.desactivado=FALSE;
			configuracionRet.modoInterfaz=INTERFACES_MODO_INTERFAZ_NO_OPERATIVO;
			configuracionRet.tipoInterfaz=INTERFACES_TIPO_INTERFAZ_NO_OPERATIVO;
			configuracionRet.numeroSerie.byte[0]=0xFF;
			configuracionRet.numeroSerie.byte[1]=0xFF;
			configuracionRet.numeroSerie.byte[2]=0xFF;			
			configuracionRet.numeroSerie.byte[3]=0xFF;			
		}	
		return configuracionRet;
}

void interfacesStore (unsigned char interfazAddress, unsigned char registroEstado, unsigned char lectura)
{
	_interfazNotificarPendiente=1;
	_interfazAddress=interfazAddress;
	_registroEstado=registroEstado;
	_lectura=lectura;	
}

void interfacesNotificar (unsigned char interfazAddress, unsigned char registroEstado, unsigned char lectura)
{
    // Por ahora se llama sólo desde la parte DALI, en el futuro se hará también desde la parte UART.
    interfacesConfiguracion configuracionInterfaz;
    configuracionInterfaz=interfacesConfiguracionGet(interfazAddress);
    if (configuracionInterfaz.instalado==TRUE)
    {
        if (configuracionInterfaz.desactivado==FALSE)
        {
					interfacesSolicitar (configuracionInterfaz, interfazAddress, configuracionInterfaz.numeroSerie, registroEstado, lectura);
        }
        else
        {
            // [Desactivado].
        }
    }
    else
    {
        //  [No Instalado].
    }
}


interfacesEventos interfacesIDNGEADEventoGet(interfacesModosInterfaz modoInterfaz, unsigned char *registroEstado, unsigned char lectura, BOOL *posicionEncendido, BOOL *sentidoAscendente)
{
  	interfacesEventos evento;
    evento.eventoTipo=INTERFACES_EVENTO_NINGUNO;
    if(modoInterfaz==INTERFACES_MODO_INTERFAZ_NO_OPERATIVO)
    {
    }    
		else if ((modoInterfaz==INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE) || (modoInterfaz==INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_DESCENDENTE) || (modoInterfaz==INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_LONG))
    {
				if ((*registroEstado&REGISTRO_ESTADO_BIT_LONG_RELEASE)==REGISTRO_ESTADO_BIT_LONG_RELEASE)
        {
            evento.eventoTipo=INTERFACES_EVENTO_LONG_RELEASE;
						evento.sentidoAscendente=0xFF;
						evento.posicionEncender=0xFF;
						evento.lectura=0xFF;					
        }			
				if ((*registroEstado&REGISTRO_ESTADO_BIT_SHORT_PRESS)==REGISTRO_ESTADO_BIT_SHORT_PRESS)
        {
            // También puedo hacerlo desde la función de pulsadoresBasculantes, pero tengo
            // que informar si es propio o no.
            // >> TOGGLE posicionEncendido del sensor correspondiente.
            if (*posicionEncendido==FALSE)
            {
                *posicionEncendido=TRUE;								
            }
            else
            {
                *posicionEncendido=FALSE;								
            }						
						// Puedo ponerlo arriba pero así me queda más claro.
						if (*posicionEncendido==TRUE)
						{
								*sentidoAscendente=FALSE;
						}
						else
						{
								*sentidoAscendente=TRUE;
						}					
            // << TOGGLE posicionEncendido del sensor correspondiente.
            evento.eventoTipo=INTERFACES_EVENTO_SHORT_PRESS;	
						evento.posicionEncender=*posicionEncendido;
        }
        if ((*registroEstado&REGISTRO_ESTADO_BIT_LONG_PRESS)==REGISTRO_ESTADO_BIT_LONG_PRESS)
        {
            // También puedo hacerlo desde la función de pulsadoresRegulacion, pero tengo
            // que informar si es propio o no.
            // >> TOGGLE sentidoAscendente del sensor correspondiente.						
						if ((*posicionEncendido==FALSE) && (*sentidoAscendente==TRUE))
						{
							*posicionEncendido=TRUE;							
						}
						else
						{			
						}						
            // << TOGGLE sentidoAscendente del sensor correspondiente.
            evento.sentidoAscendente=*sentidoAscendente;
            evento.eventoTipo=INTERFACES_EVENTO_LONG_PRESS;
						evento.posicionEncender=0xFF;
						evento.lectura=0xFF;						
						if (*sentidoAscendente==FALSE)
						{
							*sentidoAscendente=TRUE;
						}
						else
						{
               *sentidoAscendente=FALSE;
						}											
        }
    }
    else if(modoInterfaz==INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO)
    {
        if ((*registroEstado&REGISTRO_ESTADO_BIT_PRESENCY)==REGISTRO_ESTADO_BIT_PRESENCY)
        {
            evento.eventoTipo=INTERFACES_EVENTO_PRESENCY;
        }
    }
    else if(modoInterfaz==INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL)
    {
        if ((*registroEstado&REGISTRO_ESTADO_BIT_PRESENCY)==REGISTRO_ESTADO_BIT_PRESENCY)
        {
            evento.eventoTipo=INTERFACES_EVENTO_PRESENCY;
        }
        if ((*registroEstado&REGISTRO_ESTADO_BIT_VACANCY)==REGISTRO_ESTADO_BIT_VACANCY)
        {
            evento.eventoTipo=INTERFACES_EVENTO_VACANCY;
        }
    }
    else if((modoInterfaz==INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_EXT) || (modoInterfaz==INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_INT))
    {
        if ((*registroEstado&REGISTRO_ESTADO_BIT_PRESENCY)==REGISTRO_ESTADO_BIT_PRESENCY)
        {
            evento.eventoTipo=INTERFACES_EVENTO_PRESENCY;
        }
        if ((*registroEstado&REGISTRO_ESTADO_BIT_REPORTE_LUZ)==REGISTRO_ESTADO_BIT_REPORTE_LUZ)
        {
						evento.lectura=lectura;  
						evento.eventoTipo=INTERFACES_EVENTO_LIGHT_REPORT;
        }
    }
    else if((modoInterfaz==INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_EXT) || (modoInterfaz==INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_INT))
    {
        if ((*registroEstado&REGISTRO_ESTADO_BIT_PRESENCY)==REGISTRO_ESTADO_BIT_PRESENCY)
        {
            evento.eventoTipo=INTERFACES_EVENTO_PRESENCY;
        }
        if ((*registroEstado&REGISTRO_ESTADO_BIT_VACANCY)==REGISTRO_ESTADO_BIT_VACANCY)
        {
            evento.eventoTipo=INTERFACES_EVENTO_VACANCY;
        }
        if ((*registroEstado&REGISTRO_ESTADO_BIT_REPORTE_LUZ)==REGISTRO_ESTADO_BIT_REPORTE_LUZ)
        {
            evento.lectura=lectura;  
						evento.eventoTipo=INTERFACES_EVENTO_LIGHT_REPORT;
        }
    }
    else if((modoInterfaz==INTERFACES_MODO_INTERFAZ_REG_EXT) || (modoInterfaz==INTERFACES_MODO_INTERFAZ_REG_INT))
    {
        if ((*registroEstado&REGISTRO_ESTADO_BIT_REPORTE_LUZ)==REGISTRO_ESTADO_BIT_REPORTE_LUZ)
        {
            evento.lectura=lectura;
            evento.eventoTipo=INTERFACES_EVENTO_LIGHT_REPORT;
        }
    }
		(*registroEstado)=0x00;
		evento.canal=0x00;
    return evento;
}


interfacesEventos interfacesIDNG4PEventoGet(interfacesModosInterfaz modoInterfaz, unsigned char *registroEstado, unsigned char *registroAux, BOOL *posicionEncendido, BOOL *sentidoAscendente)
{
    interfacesEventos evento;


		evento.eventoTipo=INTERFACES_EVENTO_NINGUNO;
			
	
    if(modoInterfaz==INTERFACES_MODO_INTERFAZ_NO_OPERATIVO)
    {
    }    
		else if ((modoInterfaz==INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE) || (modoInterfaz==INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_DESCENDENTE) || (modoInterfaz==INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_LONG) || (modoInterfaz==INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL))
    {
				if (((*registroEstado&REGISTRO_ESTADO_BIT_SHORT_PRESS_1)==REGISTRO_ESTADO_BIT_SHORT_PRESS_1) || ((*registroEstado&REGISTRO_ESTADO_BIT_SHORT_PRESS_2)==REGISTRO_ESTADO_BIT_SHORT_PRESS_2) || ((*registroEstado&REGISTRO_ESTADO_BIT_SHORT_PRESS_3)==REGISTRO_ESTADO_BIT_SHORT_PRESS_3) || ((*registroEstado&REGISTRO_ESTADO_BIT_SHORT_PRESS_4)==REGISTRO_ESTADO_BIT_SHORT_PRESS_4))
        {
            // También puedo hacerlo desde la función de pulsadoresBasculantes, pero tengo
            // que informar si es propio o no.
            // >> TOGGLE posicionEncendido del sensor correspondiente.
            
					
						if ((*registroEstado&REGISTRO_ESTADO_BIT_SHORT_PRESS_1)==REGISTRO_ESTADO_BIT_SHORT_PRESS_1)				{evento.canal=0x00;				(*registroEstado)=(*registroEstado)&0xFE;}
						else if ((*registroEstado&REGISTRO_ESTADO_BIT_SHORT_PRESS_2)==REGISTRO_ESTADO_BIT_SHORT_PRESS_2)	{evento.canal=0x01;				(*registroEstado)=(*registroEstado)&0xFD;}
						else if ((*registroEstado&REGISTRO_ESTADO_BIT_SHORT_PRESS_3)==REGISTRO_ESTADO_BIT_SHORT_PRESS_3)	{evento.canal=0x02;				(*registroEstado)=(*registroEstado)&0xFB;}
						else if ((*registroEstado&REGISTRO_ESTADO_BIT_SHORT_PRESS_4)==REGISTRO_ESTADO_BIT_SHORT_PRESS_4)	{evento.canal=0x03;				(*registroEstado)=(*registroEstado)&0xBF;}					
						if (posicionEncendido[evento.canal]==FALSE)
            {
                posicionEncendido[evento.canal]=TRUE;								
            }
            else
            {
                posicionEncendido[evento.canal]=FALSE;								
            }						
						// Puedo ponerlo arriba pero así me queda más claro.
						if (posicionEncendido[evento.canal]==TRUE)
						{
								sentidoAscendente[evento.canal]=FALSE;
						}
						else
						{
								sentidoAscendente[evento.canal]=TRUE;
						}					
            // << TOGGLE posicionEncendido del sensor correspondiente.
            //evento.posicionEncender=posicionEncendido[evento.canal];						
            evento.eventoTipo=INTERFACES_EVENTO_SHORT_PRESS;	
						evento.posicionEncender=posicionEncendido[evento.canal];							
						evento.sentidoAscendente=0xFF;						
						evento.lectura=0xFF;
        }
				else if ((*registroEstado&REGISTRO_ESTADO_BIT_PRESENCY)==REGISTRO_ESTADO_BIT_PRESENCY)
        {
						(*registroEstado)=(*registroEstado)&0xF7;
						evento.canal=0x00;
						evento.eventoTipo=INTERFACES_EVENTO_PRESENCY;
						evento.posicionEncender=0xFF;
						evento.sentidoAscendente=0xFF;	
						evento.lectura=0xFF;			
        }
        else if ((*registroEstado&REGISTRO_ESTADO_BIT_VACANCY)==REGISTRO_ESTADO_BIT_VACANCY)
        {
						(*registroEstado)=(*registroEstado)&0xEF;								
						evento.canal=0x00;
						evento.eventoTipo=INTERFACES_EVENTO_VACANCY;
						evento.posicionEncender=0xFF;
						evento.sentidoAscendente=0xFF;	
						evento.lectura=0xFF;			
        }							
        else if ((*registroEstado&REGISTRO_ESTADO_BIT_REPORTE_LUZ)==REGISTRO_ESTADO_BIT_REPORTE_LUZ)
				{				
					if (((*registroAux&REGISTRO_ESTADO_BIT_LONG_PRESS_1)==REGISTRO_ESTADO_BIT_LONG_PRESS_1) || ((*registroAux&REGISTRO_ESTADO_BIT_LONG_PRESS_2)==REGISTRO_ESTADO_BIT_LONG_PRESS_2) || ((*registroAux&REGISTRO_ESTADO_BIT_LONG_PRESS_3)==REGISTRO_ESTADO_BIT_LONG_PRESS_3) || ((*registroAux&REGISTRO_ESTADO_BIT_LONG_PRESS_4)==REGISTRO_ESTADO_BIT_LONG_PRESS_4))
					{
						if ((*registroAux&REGISTRO_ESTADO_BIT_LONG_PRESS_1)==REGISTRO_ESTADO_BIT_LONG_PRESS_1)				{evento.canal=0x00;				(*registroAux)=(*registroAux)&0xFE;}
						else if ((*registroAux&REGISTRO_ESTADO_BIT_LONG_PRESS_2)==REGISTRO_ESTADO_BIT_LONG_PRESS_2)		{evento.canal=0x01;				(*registroAux)=(*registroAux)&0xFD;}
						else if ((*registroAux&REGISTRO_ESTADO_BIT_LONG_PRESS_3)==REGISTRO_ESTADO_BIT_LONG_PRESS_3)		{evento.canal=0x02;				(*registroAux)=(*registroAux)&0xFB;}
						else if ((*registroAux&REGISTRO_ESTADO_BIT_LONG_PRESS_4)==REGISTRO_ESTADO_BIT_LONG_PRESS_4)		{evento.canal=0x03;				(*registroAux)=(*registroAux)&0xF7;}										
						(*registroEstado)=(*registroEstado)&0xDF;	
						if ((posicionEncendido[evento.canal]==FALSE) && (sentidoAscendente[evento.canal]==TRUE))
						{
							posicionEncendido[evento.canal]=TRUE;							
						}	
						else
						{			
						}						
						// << TOGGLE sentidoAscendente del sensor correspondiente.
						evento.sentidoAscendente=sentidoAscendente[evento.canal];
						evento.eventoTipo=INTERFACES_EVENTO_LONG_PRESS;
						evento.posicionEncender=0xFF;
						evento.lectura=0xFF;						
						if (sentidoAscendente[evento.canal]==FALSE)
						{
							sentidoAscendente[evento.canal]=TRUE;
						}
						else
						{
							sentidoAscendente[evento.canal]=FALSE;
						}									
					}
					else if (((*registroAux&REGISTRO_ESTADO_BIT_LONG_RELEASE_1)==REGISTRO_ESTADO_BIT_LONG_RELEASE_1) || ((*registroAux&REGISTRO_ESTADO_BIT_LONG_RELEASE_2)==REGISTRO_ESTADO_BIT_LONG_RELEASE_2) || ((*registroAux&REGISTRO_ESTADO_BIT_LONG_RELEASE_3)==REGISTRO_ESTADO_BIT_LONG_RELEASE_3) || ((*registroAux&REGISTRO_ESTADO_BIT_LONG_RELEASE_4)==REGISTRO_ESTADO_BIT_LONG_RELEASE_4))
					{
							if ((*registroAux&REGISTRO_ESTADO_BIT_LONG_RELEASE_1)==REGISTRO_ESTADO_BIT_LONG_RELEASE_1)				{evento.canal=0x00;				(*registroAux)=(*registroAux)&0xEF;}
							else if ((*registroAux&REGISTRO_ESTADO_BIT_LONG_RELEASE_2)==REGISTRO_ESTADO_BIT_LONG_RELEASE_2)		{evento.canal=0x01;				(*registroAux)=(*registroAux)&0xDF;}
							else if ((*registroAux&REGISTRO_ESTADO_BIT_LONG_RELEASE_3)==REGISTRO_ESTADO_BIT_LONG_RELEASE_3)		{evento.canal=0x02;				(*registroAux)=(*registroAux)&0xBF;}
							else if ((*registroAux&REGISTRO_ESTADO_BIT_LONG_RELEASE_4)==REGISTRO_ESTADO_BIT_LONG_RELEASE_4)		{evento.canal=0x03;				(*registroAux)=(*registroAux)&0x7F;}										
							(*registroEstado)=(*registroEstado)&0xDF;
							evento.eventoTipo=INTERFACES_EVENTO_LONG_RELEASE;
							evento.posicionEncender=0xFF;
							evento.lectura=0xFF;			
					}
				}
    }		
    return evento;
}


void interfacesInit (void)
{	
	unsigned char 						interfazAddress;		
	interfacesConfiguracion 	interfazConfiguracion;	
	_interfacesInstaladosCnt=0;
	for (interfazAddress=0;interfazAddress<16;interfazAddress++)
	{
		interfazConfiguracion=interfacesConfiguracionGet(interfazAddress);				
		
		if (interfazConfiguracion.instalado==TRUE)
		{
			_interfacesInstaladosCnt++;
		}		
	}	
}

void interfacesDoTask(void)
{
	 if (_interfazNotificarPendiente==0x01)
	 {
			_interfazNotificarPendiente=0x00;
			interfacesNotificar (_interfazAddress, _registroEstado, _lectura);		 
	 }	 
	 
	if (_interfacesUnfreezeSend==0x01)
	{
		_interfacesUnfreezeSend=0x00;
		daliSensoresQuerySensorStatusEnviar(0xFF);
	}
	 
}

void interfacesSysTick (void)
{
}

void interfacesRtcTick (void)
{	
	if 	(_buscandoInterfaces==0x01)
	{		
		if (_buscandoInterfacesCnt<7)
		{
			_buscandoInterfacesCnt++;
			normaLinkSensoresBuscarInterfacesSend(&_buscandoInterfacesIp[0],0x01,0x01,0xFF,_buscandoInterfacesCnt);
			
		}
		else
		{
			normaLinkSensoresBuscarInterfacesSend(&_buscandoInterfacesIp[0],0x01,0x02,0x01,_buscandoInterfacesCnt);
			_buscandoInterfaces=0;
		}		
	}	
	
	if (_interfacesUnfreezeCnt==0)
	{
		_interfacesUnfreezeCnt=0xFF;		
	}
	else if (_interfacesUnfreezeCnt!=0xFF)
	{
		if (_interfacesUnfreezeCnt<5)	
		{
			_interfacesUnfreezeSend=0x01;
		}
		_interfacesUnfreezeCnt--;				
	}
}

unsigned char interfacesInstaladosCntGet(void)
{
	return _interfacesInstaladosCnt;
}
