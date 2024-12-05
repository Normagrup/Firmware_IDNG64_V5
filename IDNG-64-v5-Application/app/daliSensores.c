// Los Query de modo y número de serie deberían ser por lazo.


// 3 EVENTOS DEBUG:
// DEBUG INFO: daliSensoresDaliNotifyCb->registro de estado recibido.
// DEBUG INFO: daliSensoresDaliNotifyCb->nivel de luz recibido.
// DEBUG INFO: daliSensoresDaliNotifyCb->feedback comando recibido.

#define CONECTAR_CON_DALI
#define CONECTAR_CON_SENSORES


#include "dataTypes.h"

#include "sensores.h"


#ifndef ISO
    #include "sensoresConfig.h"
#endif

#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT



#ifdef  CONECTAR_CON_DALI
    #include "dali.h"
#endif

#define COMANDO_DALI_RECALL_MAX_LEVEL                   0x05
#define COMANDO_DALI_OFF                                0x00
#define COMANDO_DALI_UP                                 0x01
#define COMANDO_DALI_DOWN                               0x02


#define COMANDO_DALI_STEP_UP                            0x03
#define COMANDO_DALI_STEP_DOWN                          0x04


#define COMANDO_DALI_GOTO_SCENE_0												(0x10+0)
#define COMANDO_DALI_GOTO_SCENE_1												(0x10+1)
#define COMANDO_DALI_GOTO_SCENE_2												(0x10+2)
#define COMANDO_DALI_GOTO_SCENE_3												(0x10+3)
#define COMANDO_DALI_GOTO_SCENE_4												(0x10+4)
#define COMANDO_DALI_GOTO_SCENE_5												(0x10+5)
#define COMANDO_DALI_GOTO_SCENE_6												(0x10+6)
#define COMANDO_DALI_GOTO_SCENE_7												(0x10+7)
#define COMANDO_DALI_GOTO_SCENE_8												(0x10+8)
#define COMANDO_DALI_GOTO_SCENE_9												(0x10+9)
#define COMANDO_DALI_GOTO_SCENE_10											(0x10+10)
#define COMANDO_DALI_GOTO_SCENE_11											(0x10+11)
#define COMANDO_DALI_GOTO_SCENE_12											(0x10+12)
#define COMANDO_DALI_GOTO_SCENE_13											(0x10+13)
#define COMANDO_DALI_GOTO_SCENE_14											(0x10+14)
#define COMANDO_DALI_GOTO_SCENE_15											(0x10+15)

#define COMANDO_DALI_REST					0xE0
#define COMANDO_DALI_RELIGHT			0xE1
#define COMANDO_DALI_INHIBIT			0xE2


#define COMANDO_DALI_ON_AND_STEP_UP                     0x08


// COMANDOS EXTENDIDOS lo son del 224 al 255 0xE0 a 0xFF.

#define COMANDO_DALI_QUERY_SENSOR_STATUS                0xEE
#define COMANDO_DALI_QUERY_SENSOR_NIVEL                 0xEF

#define COMANDO_DALI_QUERY_NUM_SERIE_EJERCICIO_H        0xF0
#define COMANDO_DALI_QUERY_NUM_SERIE_EJERCICIO_L        0xF1
#define COMANDO_DALI_QUERY_NUM_SERIE_MES                0xF2
#define COMANDO_DALI_QUERY_NUM_SERIE_ORDEN_TRABAJO_0    0xF3
#define COMANDO_DALI_QUERY_NUM_SERIE_ORDEN_TRABAJO_1    0xF4
#define COMANDO_DALI_QUERY_NUM_SERIE_ORDEN_TRABAJO_2    0xF5
#define COMANDO_DALI_QUERY_NUM_SERIE_ID_0               0xF6
#define COMANDO_DALI_QUERY_NUM_SERIE_ID_1               0xF7
#define COMANDO_DALI_QUERY_NUM_SERIE_ID_2               0xF8
#define COMANDO_DALI_STORE_DTR_AS_MODO_INTERFAZ         0xF9
#define COMANDO_DALI_QUERY_TIPO_INTERFAZ                0xFA
#define COMANDO_DALI_QUERY_MODO_INTERFAZ                0xFB



#define NIVEL_NO_APLICABLE                              0xFF


#define REGISTRO_ESTADO_BIT_SHORT_PRESS                 0x01
#define REGISTRO_ESTADO_BIT_LONG_PRESS                  0x02
#define REGISTRO_ESTADO_BIT_LONG_RELEASE                0x04
#define REGISTRO_ESTADO_BIT_PRESENCY                    0x08
#define REGISTRO_ESTADO_BIT_VACANCY                     0x10
#define REGISTRO_ESTADO_BIT_REPORTE_LUZ                 0x20
//#define REGISTRO_ESTADO_BIT_KEEP_ALIVE                  0x80      // El evento KEEP alive se enviará el registro de estado como esté.


BOOL daliSensoresQuerySensorStatusEnviar(unsigned char sensorAddress);

extern unsigned char _enviando;

unsigned char _enviandoSteps;

unsigned char querySensorRetries;



void daliSensoresSysTick (void)
{
	if (_enviando==1)
	{
		if (_enviandoSteps>0)
		{
			_enviandoSteps--;
		}
		else
		{
			// FORZAR LIBERACION			
			_enviando=0;			
			normaLinkSensoresDebugTransceptorLiberado();			
		}	
	}	
}


void daliSensoresPollingTmpDisable (void)
{
    #ifdef  CONECTAR_CON_DALI
        polling_TmpDisable();
    #endif
}
void daliSensoresPollingEndTmpDisable (void)
{
    #ifdef  CONECTAR_CON_DALI
        polling_EndTmpDisable();
    #endif
}
void daliSensoresQuiereComunicar (unsigned char sensorAddress)
{
    daliSensoresQuerySensorStatusEnviar(sensorAddress);
}
BOOL daliSensoresQuerySensorStatusEnviar(unsigned char sensorAddress)
{    
    // La parte de pollings llama a esta función indicandole que el sensor con dirección sensorAddress quiere comunicar.
		unsigned char _apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &_apply[0], 0x2E, COMANDO_DALI_QUERY_SENSOR_STATUS);
		}
		return answer;
    //#else
    //if (sensorAddress!=0xFF)
    //{
    //}
     //   return TRUE;
    //#endif
		
		
}

BOOL daliSensoresQueryBackupSensorStatusEnviar(unsigned char sensorAddress)
{    
		unsigned char _apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &_apply[0], 0x2E, 0xFC);
		}
		return answer;	
}


BOOL daliSensoresGoToSceneEnviar (unsigned char numeroGrupo, unsigned char scene)
{
		#ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();						
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2B, COMANDO_DALI_GOTO_SCENE_0+scene);
			actualLevelCmdNotify(((numeroGrupo<<1)+0x81), (COMANDO_DALI_GOTO_SCENE_0+scene), 0xFF);				
		}
		return answer;
    #else
    if (numeroGrupo!=0xFF)
    {
    }
        return TRUE;
    #endif
	
	
	
}

BOOL daliSensoresRestEnviar (unsigned char numeroGrupo)
{
	  #ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();						
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2C, COMANDO_DALI_REST);
		}
		return answer;
    #else
    if (numeroGrupo!=0xFF)
    {
    }
		return TRUE;
    #endif
}

BOOL daliSensoresRelightEnviar (unsigned char numeroGrupo)
{
		#ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();						
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2C, COMANDO_DALI_RELIGHT);
		}
		return answer;
    #else
    if (numeroGrupo!=0xFF)
    {
    }
		return TRUE;
    #endif
}

BOOL daliSensoresInhibitEnviar (unsigned char numeroGrupo)
{
		#ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();						
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2C, COMANDO_DALI_INHIBIT);
		}
		return answer;
    #else
    if (numeroGrupo!=0xFF)
    {
    }
		return TRUE;
    #endif
}

BOOL daliSensoresRecallMaxLevelEnviar (unsigned char numeroGrupo)
{    
    #ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2B, COMANDO_DALI_RECALL_MAX_LEVEL);
			actualLevelCmdNotify(((numeroGrupo<<1)+0x81), (COMANDO_DALI_RECALL_MAX_LEVEL+1), 0xFF);				
		}
		return answer;
    #else
    if (numeroGrupo!=0xFF)
    {
    }
        return TRUE;
    #endif
}
BOOL daliSensoresOffEnviar (unsigned char numeroGrupo)
{     
    #ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2B, COMANDO_DALI_OFF);
			actualLevelCmdNotify(((numeroGrupo<<1)+0x81), (COMANDO_DALI_OFF+1), 0xFF);				
		}		
		return answer;
    #else
    if (numeroGrupo!=0xFF)
    {
    }
        return TRUE;
    #endif
}

BOOL daliSensoresOnAndStepUpEnviar (unsigned char numeroGrupo)
{        
    #ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2B, COMANDO_DALI_ON_AND_STEP_UP);
		}		
    return answer;
		#else
    if (numeroGrupo!=0xFF)
    {
    }
        return TRUE;
    #endif
}

BOOL daliSensoresUpEnviar (unsigned char numeroGrupo)
{        
    #ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2B, COMANDO_DALI_UP);
			//answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2B, COMANDO_DALI_STEP_UP);
			actualLevelCmdNotify(((numeroGrupo<<1)+0x81), (COMANDO_DALI_UP+1), 0xFF);				
		}		
				return answer;
    #else
    if (numeroGrupo!=0xFF)
    {
    }
        return TRUE;
    #endif
}
BOOL daliSensoresDownEnviar (unsigned char numeroGrupo)
{      
    #ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2B, COMANDO_DALI_DOWN);
			//answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x81), &apply[0], 0x2B, COMANDO_DALI_STEP_DOWN);
			actualLevelCmdNotify(((numeroGrupo<<1)+0x81), (COMANDO_DALI_DOWN+1), 0xFF);				
		}		
		return answer;
    #else
    if (numeroGrupo!=0xFF)
    {
    }
        return TRUE;
    #endif
}

BOOL daliSensoresTcStepUpEnviar (unsigned char numeroGrupo)
{        
	unsigned char n;
	unsigned char destArray[64];
	unsigned char comArray[64];
	
	#ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			for (n=0;n<64;n++)
			{
				destArray[n]=0xFF;
				comArray[n]=0xFF;			
			}			
			destArray[1]=0xC1;
			comArray[1]=0x08;		
			destArray[0]=((numeroGrupo<<1)+0x81);
			comArray[0]=0xE9;
			answer=daliBufferSend(DALI_SENSORS, 0xFFFF, 0x17, &destArray[0],&comArray[0]);		
			answer=TRUE;
		}		
		return answer;
		
	#else
		if (numeroGrupo!=0xFF)
		{
		}
		return TRUE;
	#endif
}

BOOL daliSensoresTcStepDownEnviar (unsigned char numeroGrupo)
{        
	unsigned char n;
	unsigned char destArray[64];
	unsigned char comArray[64];
	
	#ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			for (n=0;n<64;n++)
			{
				destArray[n]=0xFF;
				comArray[n]=0xFF;			
			}			
			destArray[1]=0xC1;
			comArray[1]=0x08;		
			destArray[0]=((numeroGrupo<<1)+0x81);
			comArray[0]=0xE8;
			answer=daliBufferSend(DALI_SENSORS, 0xFFFF, 0x17, &destArray[0],&comArray[0]);
			answer=TRUE;
		}		
		return answer;
	#else
		if (numeroGrupo!=0xFF)
		{
		}
		return TRUE;
	#endif
}

BOOL daliSensoresComandoArcoDirectoEnviar(unsigned char numeroGrupo, unsigned char nivel)
{     
    #ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0xFFFF, 0x01, ((numeroGrupo<<1)+0x80), &apply[0], 0x2B, nivel);		
			actualLevelCmdNotify(((numeroGrupo<<1)+0x81), 0x00, 0xFF);				
		}		
		return answer;
	
		

    #else
    //if ((numeroGrupo!=0xFF) && (nivel!=0xFF))
    //{
    //}
    //return TRUE;
    #endif
}
BOOL daliSensoresQuerySensorNivelEnviar(unsigned char sensorAddress)
{     
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_SENSOR_NIVEL);        
		}		
		return answer;  
}

BOOL daliSensoresQueryBackupSensorNivelEnviar(unsigned char sensorAddress)
{     
		unsigned char apply[8];
		BOOL answer=FALSE;	
		if (_enviando==0)
		{
			_enviando=1;
			_enviandoSteps=200;
			daliStop();			
			answer=daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, 0xFD);        
		}		
		return answer;  
}

void daliSensoresSensorStatusProcesar (unsigned char sensorAddress, unsigned char registroEstado)
{
    if ((registroEstado&REGISTRO_ESTADO_BIT_REPORTE_LUZ)==REGISTRO_ESTADO_BIT_REPORTE_LUZ)
    {
        // Si es un light report pide el nivel.				
				interfacesStore(sensorAddress, (registroEstado&0xDF), NIVEL_NO_APLICABLE);
        daliSensoresQuerySensorNivelEnviar(sensorAddress);
    }
		else
		{		
		   interfacesStore(sensorAddress, registroEstado, NIVEL_NO_APLICABLE);
		}
}
void daliSensoresNivelLuzRecibidoProcesar (unsigned char sensorAddress, unsigned char nivel)
{
    // Pasa pasa la información a sensores.
    #ifdef CONECTAR_CON_SENSORES
        interfacesStore(sensorAddress, REGISTRO_ESTADO_BIT_REPORTE_LUZ, nivel);
    #endif
}
//static void daliSensoresTipoInterfazRecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
//{
//}
static void daliSensoresModoInterfazRecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{      
    
    #ifdef  CONECTAR_CON_DALI
        unsigned char apply[8];
        daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_NUM_SERIE_EJERCICIO_H);
    #endif
	if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
static void daliSensoresNumSerieEjercicioHRecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{        

    #ifdef  CONECTAR_CON_DALI
        unsigned char apply[8];
        daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_NUM_SERIE_EJERCICIO_L);
    #endif
	    if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
static void daliSensoresNumSerieEjercicioLRecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{      

    #ifdef  CONECTAR_CON_DALI
        unsigned char apply[8];
        daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_NUM_SERIE_MES);
    #endif
	    if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
static void daliSensoresNumSerieMesRecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{      

    #ifdef  CONECTAR_CON_DALI
        unsigned char apply[8];
        daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_NUM_SERIE_ORDEN_TRABAJO_0);
    #endif
	    if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
static void daliSensoresNumSerieOrdenTrabajo0RecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{  

    #ifdef  CONECTAR_CON_DALI
        unsigned char apply[8];
        daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_NUM_SERIE_ORDEN_TRABAJO_1);
    #endif
	    if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
static void daliSensoresNumSerieOrdenTrabajo1RecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{  

    #ifdef  CONECTAR_CON_DALI
        unsigned char apply[8];
        daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_NUM_SERIE_ORDEN_TRABAJO_2);
    #endif
	    if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
static void daliSensoresNumSerieOrdenTrabajo2RecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{      

    #ifdef  CONECTAR_CON_DALI
        unsigned char apply[8];
        daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_NUM_SERIE_ID_0);
    #endif
	    if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
static void daliSensoresNumSerieId0RecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{

    #ifdef  CONECTAR_CON_DALI
        unsigned char apply[8];
        daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_NUM_SERIE_ID_1);
    #endif
	    if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
static void daliSensoresNumSerieId1RecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{      

    #ifdef  CONECTAR_CON_DALI
		unsigned char apply[8];
		daliSend(DALI_SENSORS, 0x01, 0x16, ((sensorAddress<<1)+1), &apply[0], 0x2E, COMANDO_DALI_QUERY_NUM_SERIE_ID_2);
    #endif
	    if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
static void daliSensoresNumSerieId2RecibidoProcesar(unsigned char sensorAddress, unsigned char answer)
{
    if ((sensorAddress!=0xFF) && (answer!=0xFF))
    {
    }
}
void daliSensoresDaliNotifyCb(unsigned int daliSentSenderPacketId, unsigned char daliSentFrameType, unsigned char daliSentDest, unsigned char *daliSentApply, unsigned char daliSentCommandHigh, unsigned char daliSentCommandLow, unsigned char daliReplyFrameType, unsigned char daliReplyValid, unsigned char *daliReplyValidArray, unsigned char daliReplyAnswer, unsigned char *daliReplyAnswerArray)
{
    unsigned char sensorAddress;

    if ((daliSentSenderPacketId!=0xFFFF) && (daliSentFrameType!=0xFF) && (daliSentDest!=0xFF) && (daliSentApply[0]!=0xFF) &&  (daliSentCommandHigh!=0xFF) && (daliSentCommandLow!=0xFF) && (daliReplyFrameType!=0xFF) && (daliReplyValid!=0xFF) && (daliReplyValidArray[0]!=0xFF) && (daliReplyAnswer!=0xFF) && (daliReplyAnswerArray[0]!=0xFF))
    {
    }
    // Todo lo que envio me rebota aquí, si pido el registro de estado me rebota aquí.
    // Si recibe un registro de estado lo procesa:
    // daliSensoresSensorStatusProcesar (unsigned char sensorAddress, unsigned char registroEstado);
    // DEBUG registro de estado recibido.
    // Si recibe un registro de nivel de luz lo procesa:
    // daliSensoresNivelLuzRecibidoProcesar (sensorAddress, registroEstado);
    // DEBUG nivel de luz recibido.
    // Si recibe la confirmación de un comando de momento no hace nada salvo depurar.
    // DEBUG feedback comando recibido.
    sensorAddress=(daliSentDest>>1);


		if (daliSentCommandHigh==0x2B)
		{
			_enviando=0;					
		}
		
		if ((daliSentCommandHigh==0x2E) && ((daliSentCommandLow==COMANDO_DALI_QUERY_SENSOR_STATUS) || (daliSentCommandLow==0xFC)))
		{
			_enviando=0;					
		}
		
		if ((daliSentCommandHigh==0x2E) && ((daliSentCommandLow==COMANDO_DALI_QUERY_SENSOR_NIVEL) ||  (daliSentCommandLow==0xFD)))
		{
			_enviando=0;					
		}
		
		//if ((daliSentFrameType==0x16) && (daliReplyAnswer==2) && (sensorAddress<MAX_SENSORES_DALI) && (daliSentCommandLow==COMANDO_DALI_QUERY_SENSOR_STATUS))
		//{
		//	daliSensoresSensorStatusProcesar (sensorAddress, 0xFF);			
		//}

    if ((daliSentFrameType==0x16) &&  (sensorAddress<MAX_SENSORES_DALI) && (daliSentCommandHigh==0x2E))
    {
				if (daliReplyValid==0x01)
        {
            if ((daliSentCommandLow==COMANDO_DALI_QUERY_SENSOR_STATUS) || (daliSentCommandLow==0xFC))
            {
								querySensorRetries=3;
								daliSensoresSensorStatusProcesar (sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_SENSOR_NIVEL)
            {
								querySensorRetries=3;  
								daliSensoresNivelLuzRecibidoProcesar (sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_TIPO_INTERFAZ)
            {
                daliSensoresModoInterfazRecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_MODO_INTERFAZ)
            {
                daliSensoresModoInterfazRecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_NUM_SERIE_EJERCICIO_H)
            {
                daliSensoresNumSerieEjercicioHRecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_NUM_SERIE_EJERCICIO_L)
            {
                daliSensoresNumSerieEjercicioLRecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_NUM_SERIE_MES)
            {
                daliSensoresNumSerieMesRecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_NUM_SERIE_ORDEN_TRABAJO_0)
            {
                daliSensoresNumSerieOrdenTrabajo0RecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_NUM_SERIE_ORDEN_TRABAJO_1)
            {
                daliSensoresNumSerieOrdenTrabajo1RecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_NUM_SERIE_ORDEN_TRABAJO_2)
            {
                daliSensoresNumSerieOrdenTrabajo2RecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_NUM_SERIE_ID_0)
            {
                daliSensoresNumSerieId0RecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_NUM_SERIE_ID_1)
            {
                daliSensoresNumSerieId1RecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
            else if (daliSentCommandLow==COMANDO_DALI_QUERY_NUM_SERIE_ID_2)
            {
                daliSensoresNumSerieId2RecibidoProcesar(sensorAddress, daliReplyAnswer);
            }
        }
				else 
				{
					if (daliSentCommandLow==COMANDO_DALI_QUERY_SENSOR_STATUS)
					{
						if 			((daliReplyAnswer==0x01) && (querySensorRetries>0))				daliSensoresQuerySensorStatusEnviar(sensorAddress);						//N.C.						
						else if ((daliReplyAnswer==0x02) && (querySensorRetries>0))				daliSensoresQueryBackupSensorStatusEnviar(sensorAddress);			//F.E.
						if (querySensorRetries>0)	querySensorRetries--;
					}
					else if (daliSentCommandLow==0xFC)
					{
							daliSensoresQueryBackupSensorStatusEnviar(sensorAddress);	
							if (querySensorRetries>0)	querySensorRetries--;
					}
					else if (daliSentCommandLow==COMANDO_DALI_QUERY_SENSOR_NIVEL)
					{						
						if 			((daliReplyAnswer==0x01) &&	(querySensorRetries>0))	daliSensoresQuerySensorNivelEnviar(sensorAddress);						//N.C.
						else if ((daliReplyAnswer==0x02) &&	(querySensorRetries>0))	daliSensoresQueryBackupSensorNivelEnviar(sensorAddress);			//F.E.
						if (querySensorRetries>0)	querySensorRetries--;
						
					}
					else if (daliSentCommandLow==0xFD)
					{
							daliSensoresQueryBackupSensorNivelEnviar(sensorAddress);	
							if (querySensorRetries>0)	querySensorRetries--;
					}
				}
    }
		

		
		QuerySensorTypeCb 		(daliSentCommandLow, daliReplyValid, daliReplyAnswer);
		QuerySensorEnabledCb	(daliSentCommandLow, daliReplyValid, daliReplyAnswer);
		QuerySensorModeCb			(daliSentCommandLow, daliReplyValid, daliReplyAnswer);
		QuerySensorSerial1Cb	(daliSentCommandLow, daliReplyValid, daliReplyAnswer);
		QuerySensorSerial2Cb	(daliSentCommandLow, daliReplyValid, daliReplyAnswer);
		QuerySensorSerial3Cb	(daliSentCommandLow, daliReplyValid, daliReplyAnswer);
		QuerySensorSerial4Cb	(daliSentCommandLow, daliReplyValid, daliReplyAnswer);		
		
		cmd_START_CONFIG_cb (daliSentCommandLow);
		cmd_FINISH_CONFIG_cb (daliSentCommandLow);
		cmd_ENABLE_DISABLE_cb (daliSentCommandLow);
		cmd_SET_MODE_cb (daliSentCommandLow);		
		cmd_QUERY_ENABLE_DISABLE_cb(daliSentCommandLow, daliReplyValid, daliReplyAnswer);
		cmd_QUERY_MODE_cb (daliSentCommandLow, daliReplyValid, daliReplyAnswer);

		cmd_DTR_cb (daliSentDest);
		cmd_SET_SETPOINT_cb (daliSentCommandLow);
		cmd_QUERY_SETPOINT_cb (daliSentCommandLow, daliReplyValid, daliReplyAnswer);	
		
		
		
		
}
