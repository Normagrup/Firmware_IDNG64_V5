/*

05/02/2019	SOPORTE FUNCIÓN CORRIDOR.

*/



// POSIBILIDAD DE REALIZAR UN ESCALADO EN SYSTICK PARA NO SOBRECARGAR TANTO EL MICRO.
// PTE PASO A STRUCT _sensoresGruposAccionDAPCPending[MAX_GRUPOS][2];
// PTE PASO A STRUCT _sensoresGruposAccionQuerySensorStatusPending[MAX_GRUPOS][2];
#define INC_SENSORES_GRUPOS
    #include "sensoresGrupos.h"
#undef INC_SENSORES_GRUPOS

#include "utils.h"
#include "config.h"
#include "macros.h"

		extern unsigned char daliLoopActualLevel[64];

// VARIABLES. 208 bytes
    unsigned char _sensoresGruposAccionRecallMaxLevelPending[MAX_GRUPOS];                     // Debe transmitirse RECALL MAX LEVEL								[16]			[02]													
    unsigned char _sensoresGruposAccionOffPending[MAX_GRUPOS];                                // Debe transmitirse OFF														[16]			[02]
    unsigned char _sensoresGruposAccionUpPending[MAX_GRUPOS];                                 // Debe transmitirse UP															[16]			[02]
		
		
		unsigned char _sensoresGruposAccionInhibitPending[MAX_GRUPOS];
		unsigned char	_sensoresGruposAccionRelightPending[MAX_GRUPOS];
		unsigned char _sensoresGruposAccionRestPending[MAX_GRUPOS];	
		
		
		unsigned char _sensoresGruposAccionTcStepUpPending[MAX_GRUPOS];                           // Debe transmitirse TC STEP UP											[16]			[02]
    unsigned char _sensoresGruposAccionTcStepDownPending[MAX_GRUPOS];                         // Debe transmitirse TC STEP DOWN										[16]			[02]
		
		unsigned char _sensoresGruposAccionOnAndStepUpPending[MAX_GRUPOS];                        // Debe transmitirse ...														[16]			[02]
    unsigned char _sensoresGruposAccionDownPending[MAX_GRUPOS];                               // Debe transmitirse DOWN														[16]			[02]
    unsigned char _sensoresGruposAccionDAPCPending[MAX_GRUPOS][2];                            // Debe transmitirse DAPC, NIVEL										[32]			[16]
		unsigned char _sensoresGruposAccionGoToScenePending[MAX_GRUPOS][2];                       // Debe transmitirse GOTO SCENE, SCENE							[32]			[16]
    unsigned char _sensoresGruposAccionQuerySensorStatusPending[MAX_GRUPOS][2];               // Debe hacerse Query Sensor Status, SENSORADDRESS	[32]			[16]
		
		UINT16 _sensoresGruposAlgunoOperativo[MAX_GRUPOS];																				//																									[32]			[32]								
																																															// ---------------------------------------------------------------------
																																															//																									[208]			[90]
// VARIABLES.	


		

	
		

// FUNCIONES.


		void _sensoresGruposAlgunoOperativoClear (void)
		{
			unsigned char numeroGrupo;
			for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
			{
				_sensoresGruposAlgunoOperativo[numeroGrupo]=0;
			}			
		}
		
		void	_sensoresGruposAlgunoOperativoSet (unsigned char numeroGrupo, sensoresTipos tipo)
		{
			SETBIT(_sensoresGruposAlgunoOperativo[numeroGrupo],((unsigned char)tipo));
		}


    BOOL _sensoresGruposAlgunoOperativoGet (unsigned char numeroGrupo, sensoresTipos tipo)
    {        
				if (BITVAL(_sensoresGruposAlgunoOperativo[numeroGrupo],((unsigned char)tipo))==0x01)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
    }
    BOOL _sensoresGruposDesactivadoGet(UINT16 matrizGrupos, unsigned char numeroGrupo)
    {
        if (BITVAL(matrizGrupos, numeroGrupo)==0x01)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    BOOL _sensoresGruposForMeGet(UINT16 matrizGrupos, unsigned char numeroGrupo)
    {
        if (BITVAL(matrizGrupos, numeroGrupo)==0x01)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    BOOL _sensoresGruposAlgunEquipoApagadoGet(unsigned char numeroGrupo)
    {
     		
			
				unsigned char valores[3];
				unsigned char retVal=FALSE;
				// Devuelve TRUE en caso de que en el grupo haya algún equipo apagado, con la finalidad de cambiar el estado de los reguladores de luz.
        if (numeroGrupo<MAX_GRUPOS)
        {										
					actualLevel_GrupoValoresGet (numeroGrupo, &valores[0]);					
					if (valores[1]>0)
					{
						retVal=TRUE;
					}				
				}
				return retVal;
				//return FALSE;
			
			
			//if (daliLoopActualLevel[1]==0x00)
			//{
			//	return TRUE;
			//}
			//else
			//{
			//	return FALSE;
			//}
			
			
    }
    
		BOOL _sensoresGruposPulsadoresDetectoresPresenciaVacancyGet (unsigned char numeroGrupo)
    {
        // No sé que uso tiene.
        if (numeroGrupo<MAX_GRUPOS)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }		
		void _sensoresGruposAccionRestSet(unsigned char numeroGrupo)
		{
			_sensoresGruposAccionRestPending[numeroGrupo]=2;
			_sensoresGruposAccionRelightPending[numeroGrupo]=0;
			_sensoresGruposAccionInhibitPending[numeroGrupo]=0;
		}		
		void _sensoresGruposAccionRelightSet(unsigned char numeroGrupo)
		{
			_sensoresGruposAccionRelightPending[numeroGrupo]=2;
			_sensoresGruposAccionRestPending[numeroGrupo]=0;			
			_sensoresGruposAccionInhibitPending[numeroGrupo]=0;
		}
		void _sensoresGruposAccionInhibitSet(unsigned char numeroGrupo)
		{
			_sensoresGruposAccionInhibitPending[numeroGrupo]=2;
			_sensoresGruposAccionRelightPending[numeroGrupo]=0;
			_sensoresGruposAccionRestPending[numeroGrupo]=0;			
		}		
		void _sensoresGruposAccionRecallMaxLevelSet(unsigned char numeroGrupo)
    {        
        _sensoresGruposAccionUpPending[numeroGrupo]=0;
        _sensoresGruposAccionDownPending[numeroGrupo]=0;
        _sensoresGruposAccionOffPending[numeroGrupo]=0;
        _sensoresGruposAccionDAPCPending[numeroGrupo][0]=0;
			_sensoresGruposAccionGoToScenePending[numeroGrupo][0]=0;
        _sensoresGruposAccionRecallMaxLevelPending[numeroGrupo]=1;
    }
    void _sensoresGruposAccionOffSet(unsigned char numeroGrupo)
    {
        _sensoresGruposAccionUpPending[numeroGrupo]=0;
        _sensoresGruposAccionDownPending[numeroGrupo]=0;
        _sensoresGruposAccionRecallMaxLevelPending[numeroGrupo]=0;
        _sensoresGruposAccionDAPCPending[numeroGrupo][0]=0;
			
			 _sensoresGruposAccionGoToScenePending[numeroGrupo][0]=0;
			
        _sensoresGruposAccionOffPending[numeroGrupo]=1;
    }

    void _sensoresGruposAccionDAPCSet(unsigned char numeroGrupo, unsigned char nivel, BOOL origenReguladoresLuz)
    {
        if (origenReguladoresLuz==TRUE)
        {
            if ((_sensoresGruposAccionUpPending[numeroGrupo]==0) && (_sensoresGruposAccionDownPending[numeroGrupo]==0) &&(_sensoresGruposAccionRecallMaxLevelPending[numeroGrupo]==0) && (_sensoresGruposAccionOffPending[numeroGrupo]==0))
            {
                _sensoresGruposAccionDAPCPending[numeroGrupo][0]=1;
                _sensoresGruposAccionDAPCPending[numeroGrupo][1]=nivel;
            }
        }
        else
        {
            _sensoresGruposAccionUpPending[numeroGrupo]=0;
            _sensoresGruposAccionDownPending[numeroGrupo]=0;
            _sensoresGruposAccionRecallMaxLevelPending[numeroGrupo]=0;
            _sensoresGruposAccionOffPending[numeroGrupo]=0;
            _sensoresGruposAccionDAPCPending[numeroGrupo][0]=1;
						_sensoresGruposAccionGoToScenePending[numeroGrupo][0]=0;
            _sensoresGruposAccionDAPCPending[numeroGrupo][1]=nivel;
        }
    }
		
		void _sensoresGruposAccionOnAndStepUpSet(unsigned char numeroGrupo)
    {
        // DEBUG Se ha borrado un flag previamente marcado.
        _sensoresGruposAccionOnAndStepUpPending[numeroGrupo]=1;        
    }

		
    void _sensoresGruposAccionUpSet(unsigned char numeroGrupo)
    {
        // DEBUG Se ha borrado un flag previamente marcado.
        _sensoresGruposAccionDownPending[numeroGrupo]=0;
        _sensoresGruposAccionUpPending[numeroGrupo]=1;
    }
    void _sensoresGruposAccionDownSet(unsigned char numeroGrupo)
    {
        // DEBUG Se ha borrado un flag previamente marcado.
        _sensoresGruposAccionUpPending[numeroGrupo]=0;
        _sensoresGruposAccionDownPending[numeroGrupo]=1;
    }
		
		void _sensoresGruposAccionTcStepUpSet(unsigned char numeroGrupo)
    {
        // DEBUG Se ha borrado un flag previamente marcado.
        _sensoresGruposAccionTcStepDownPending[numeroGrupo]=0;
        _sensoresGruposAccionTcStepUpPending[numeroGrupo]=1;
    }
    void _sensoresGruposAccionTcStepDownSet(unsigned char numeroGrupo)
    {
        // DEBUG Se ha borrado un flag previamente marcado.
        _sensoresGruposAccionTcStepUpPending[numeroGrupo]=0;
        _sensoresGruposAccionTcStepDownPending[numeroGrupo]=1;
    }
		
		
		
    void _sensoresGruposAccionQuerySensorStatusSet(unsigned char numeroGrupo, unsigned char interfazAddress)
    {
        // DEBUG Se ha borrado un flag previamente marcado.
        _sensoresGruposAccionQuerySensorStatusPending[numeroGrupo][0]=1;
        _sensoresGruposAccionQuerySensorStatusPending[numeroGrupo][1]=interfazAddress;
    }
    
		
		void _sensoresGruposAccionGoToSceneSet(unsigned char numeroGrupo, unsigned char scene)
		{
			_sensoresGruposAccionGoToScenePending[numeroGrupo][0]=0x01;         
			_sensoresGruposAccionGoToScenePending[numeroGrupo][1]=scene;			
		}
		
		
		
		void sensoresGruposInit (void)
    {
        sensoresGruposPulsadoresAccionFijaInit();
        sensoresGruposPulsadoresAccionBasculanteInit();
        sensoresGruposPulsadoresAccionTemporizadaInit();
        sensoresGruposDetectoresPresenciaInit();
        sensoresGruposReguladoresLuzInit();
        sensoresGruposPulsadoresAccionRegulacionInit();
				sensoresGruposPulsadoresTWInit();
    }
    void sensoresGruposRtcTick (void)
    {
        sensoresGruposPulsadoresAccionFijaRtcTick();
        sensoresGruposPulsadoresAccionBasculanteRtcTick();
        sensoresGruposPulsadoresAccionTemporizadaRtcTick();
        sensoresGruposDetectoresPresenciaRtcTick();
        sensoresGruposReguladoresLuzRtcTick();
        sensoresGruposPulsadoresAccionRegulacionRtcTick();
				sensoresGruposPulsadoresTWRtcTick();
    }
    void sensoresGruposSysTick (void)
    {
        sensoresGruposPulsadoresAccionFijaSysTick();
        sensoresGruposPulsadoresAccionBasculanteSysTick();
        sensoresGruposPulsadoresAccionTemporizadaSysTick();
        sensoresGruposDetectoresPresenciaSysTick();
        sensoresGruposReguladoresLuzSysTick();
        sensoresGruposPulsadoresAccionRegulacionSysTick();
				sensoresGruposPulsadoresTWSysTick();
    }
    
		void _sensoresGruposOnActionCall(unsigned char numeroGrupo)
		{
			sensoresGruposConfiguracion configuracionGrupo;
			configuracionGrupo=sensoresGruposConfiguracionGet(numeroGrupo);	
			if ((configuracionGrupo.onAction==0xFF) || (configuracionGrupo.onAction==ACCION_RECALL_MAX_LEVEL))
			{
				_sensoresGruposAccionRecallMaxLevelSet(numeroGrupo);
			}
			else if (configuracionGrupo.onAction==ACCION_OFF)
			{
					// [Off].
					_sensoresGruposAccionOffSet(numeroGrupo);
			}
			else if ((configuracionGrupo.onAction>=ACCION_GOTO_SCENE_0) && (configuracionGrupo.onAction<=ACCION_GOTO_SCENE_15))
			{
				// [Goto SCENE].									
				circGruposInhibidosSet(numeroGrupo, 0);
				circGruposInhibidosSet(numeroGrupo, 1);									
				_sensoresGruposAccionGoToSceneSet(numeroGrupo,(configuracionGrupo.onAction-ACCION_GOTO_SCENE_0));
			}					
		}
		
		void _sensoresGruposOffActionCall(unsigned char numeroGrupo)
		{
			sensoresGruposConfiguracion configuracionGrupo;
			configuracionGrupo=sensoresGruposConfiguracionGet(numeroGrupo);	
			if ((configuracionGrupo.offAction==0xFF) || (configuracionGrupo.offAction==ACCION_OFF))				
			{
				
				_sensoresGruposAccionOffSet(numeroGrupo);
			}
			else if (configuracionGrupo.offAction==ACCION_RECALL_MAX_LEVEL)
			{
					_sensoresGruposAccionRecallMaxLevelSet(numeroGrupo);					
			}
			else if ((configuracionGrupo.offAction>=ACCION_GOTO_SCENE_0) && (configuracionGrupo.offAction<=ACCION_GOTO_SCENE_15))
			{
				// [Goto SCENE].									
				circGruposInhibidosSet(numeroGrupo, 0);
				circGruposInhibidosSet(numeroGrupo, 1);									
				_sensoresGruposAccionGoToSceneSet(numeroGrupo,(configuracionGrupo.offAction-ACCION_GOTO_SCENE_0));
			}		
		}		
		
 		void sensoresGruposNotificar (unsigned char sensorAddress, sensoresOrigenNotificacion origenNotificacion, sensoresConfiguracion configuracionSensor, sensoresEventos evento)
    {
        UINT16 matrizGruposAplicable;
        // [w] Falta gestionar la desactivacion.
        matrizGruposAplicable=configuracionSensor.matrizGrupos;
        switch (configuracionSensor.tipoSensor)
        {
            case SENSORES_TIPOS_ACCION_FIJA:
                if (evento.eventoTipo==SENSORES_EVENTO_SHORT_PRESS)
                {
                    sensoresGruposPulsadoresAccionFijaNotificar (matrizGruposAplicable,  configuracionSensor.accion);
                }
                break;
            case SENSORES_TIPOS_ACCION_BASCULANTE:
                if (evento.eventoTipo==SENSORES_EVENTO_SHORT_PRESS)
                {
                    sensoresGruposPulsadoresAccionBasculanteNotificar (matrizGruposAplicable, evento, configuracionSensor.accion);
                }
                break;
            case SENSORES_TIPOS_ACCION_TEMPORIZADA:
                if (evento.eventoTipo==SENSORES_EVENTO_SHORT_PRESS)
                {
                    sensoresGruposPulsadoresAccionTemporizadaNotificar (matrizGruposAplicable);
                }
                break;
            case SENSORES_TIPOS_ACCION_FIJA_REGULACION:
                if (evento.eventoTipo==SENSORES_EVENTO_SHORT_PRESS)
                {
                    sensoresGruposPulsadoresAccionFijaNotificar (matrizGruposAplicable,  configuracionSensor.accion);
                }
                if ((evento.eventoTipo==SENSORES_EVENTO_LONG_PRESS) || (evento.eventoTipo==SENSORES_EVENTO_LONG_RELEASE))
                {                  
									if (configuracionSensor.accion[0][0]==ACCION_OFF)
									{
										evento.sentidoAscendente=FALSE;
									}
									else if (configuracionSensor.accion[0][0]==ACCION_RECALL_MAX_LEVEL)
									{
										evento.sentidoAscendente=TRUE;
									}											
									sensoresGruposPulsadoresAccionRegulacionNotificar (origenNotificacion, matrizGruposAplicable, evento);
                }
                break;
            case SENSORES_TIPOS_ACCION_BASCULANTE_REGULACION:
                if (evento.eventoTipo==SENSORES_EVENTO_SHORT_PRESS)
                {
                    sensoresGruposPulsadoresAccionBasculanteNotificar (matrizGruposAplicable, evento, configuracionSensor.accion);
                }
                if ((evento.eventoTipo==SENSORES_EVENTO_LONG_PRESS) || (evento.eventoTipo==SENSORES_EVENTO_LONG_RELEASE))
                {
                    sensoresGruposPulsadoresAccionRegulacionNotificar (origenNotificacion, matrizGruposAplicable, evento);										
                }
                break;
            case SENSORES_TIPOS_ACCION_TEMPORIZADA_REGULACION:
                if (evento.eventoTipo==SENSORES_EVENTO_SHORT_PRESS)
                {
                    sensoresGruposPulsadoresAccionTemporizadaNotificar (matrizGruposAplicable);
                }
                if ((evento.eventoTipo==SENSORES_EVENTO_LONG_PRESS) || (evento.eventoTipo==SENSORES_EVENTO_LONG_RELEASE))
                {
                    sensoresGruposPulsadoresAccionRegulacionNotificar (origenNotificacion, matrizGruposAplicable, evento);
                }
                break;
            case SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL:
                if ((evento.eventoTipo==SENSORES_EVENTO_PRESENCY) || (evento.eventoTipo==SENSORES_EVENTO_VACANCY))
                {
                    sensoresGruposDetectoresPresenciaNotificar (sensorAddress, matrizGruposAplicable, evento);
                }
                break;
            case SENSORES_TIPOS_DETECTOR_PRESENCIA_FLANCO:
								// if ((evento.eventoTipo==SENSORES_EVENTO_PRESENCY) || (evento.eventoTipo==SENSORES_EVENTO_KEEP_ALIVE))
								if (evento.eventoTipo==SENSORES_EVENTO_PRESENCY)
                {
                    sensoresGruposDetectoresPresenciaNotificar (sensorAddress, matrizGruposAplicable, evento);
                }
            break;
            case SENSORES_TIPOS_REGULADOR_LUZ:
                if (evento.eventoTipo==SENSORES_EVENTO_LIGHT_REPORT)
                {
                    sensoresGruposReguladoresLuzNotificar (matrizGruposAplicable, evento);
                }
            break;
            case SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL_REGULADOR_LUZ:
                if ((evento.eventoTipo==SENSORES_EVENTO_PRESENCY) || (evento.eventoTipo==SENSORES_EVENTO_VACANCY))
                {
                    sensoresGruposDetectoresPresenciaNotificar (sensorAddress, matrizGruposAplicable, evento);
                }
                if (evento.eventoTipo==SENSORES_EVENTO_LIGHT_REPORT)
                {
                    sensoresGruposReguladoresLuzNotificar (matrizGruposAplicable, evento);
                }
                break;

            case SENSORES_TIPOS_DETECTOR_PRESENCIA_FLANCO_REGULADOR_LUZ:
                if ((evento.eventoTipo==SENSORES_EVENTO_PRESENCY) || (evento.eventoTipo==SENSORES_EVENTO_KEEP_ALIVE))
                {
                    sensoresGruposDetectoresPresenciaNotificar (sensorAddress, matrizGruposAplicable, evento);
                }
                if (evento.eventoTipo==SENSORES_EVENTO_LIGHT_REPORT)
                {
                    sensoresGruposReguladoresLuzNotificar (matrizGruposAplicable, evento);
                }
            break;
								
						case SENSORES_TIPOS_PULSADOR_TW:
							{								
								if ((evento.eventoTipo==SENSORES_EVENTO_LONG_PRESS) || (evento.eventoTipo==SENSORES_EVENTO_LONG_RELEASE))
								{							
									sensoresGruposPulsadoresTWNotificar (origenNotificacion, matrizGruposAplicable, evento);
								}
							}						
							break;
								
            default:
                break;
        }
    }
    void sensoresGruposDoTask (void)
    {
        unsigned char numeroGrupo;
				unsigned char skipQuerySensorStatus=0;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if (_sensoresGruposAccionRecallMaxLevelPending[numeroGrupo]>0)
            {
                if (_sensoresDALIrecallMaxLevelEnviar(numeroGrupo)==TRUE)
                {
                    // Debug Recall Max level en buffer.
                    _sensoresGruposAccionRecallMaxLevelPending[numeroGrupo]--;
                }
            }
            else if (_sensoresGruposAccionOffPending[numeroGrupo]>0)
            {
                if (_sensoresDALIoffEnviar(numeroGrupo)==TRUE)
                {
                    // Debug Off en buffer.
                    _sensoresGruposAccionOffPending[numeroGrupo]--;
                }
            }
            else if (_sensoresGruposAccionDAPCPending[numeroGrupo][0]>0)
            {
                if (_sensoresDALIcomandoArcoDirectoEnviar(numeroGrupo, _sensoresGruposAccionDAPCPending[numeroGrupo][1])==TRUE)
                {
                    // Debug DAPC en buffer.
                    _sensoresGruposAccionDAPCPending[numeroGrupo][0]--;
                }
            }
						else if (_sensoresGruposAccionOnAndStepUpPending[numeroGrupo]>0)
            {
                if (_sensoresDALIonAndStepUpEnviar(numeroGrupo)==TRUE)
                {
										//skipQuerySensorStatus=1;
                    _sensoresGruposAccionOnAndStepUpPending[numeroGrupo]--;
                }
            }
            else if (_sensoresGruposAccionUpPending[numeroGrupo]>0)
            {
                if (_sensoresDALIupEnviar(numeroGrupo)==TRUE)
                {
                    // Debug Up en buffer.
										//skipQuerySensorStatus=1;
                    _sensoresGruposAccionUpPending[numeroGrupo]--;
                }
            }
            else if (_sensoresGruposAccionDownPending[numeroGrupo]>0)
            {
                if (_sensoresDALIdownEnviar(numeroGrupo)==TRUE)
                {
                    // Debug Down en buffer.
										//skipQuerySensorStatus=1;
                    _sensoresGruposAccionDownPending[numeroGrupo]--;
                }
            }
						else if (_sensoresGruposAccionTcStepUpPending[numeroGrupo]>0)
						{
							if (_sensoresDALItcStepUpEnviar(numeroGrupo)==TRUE)
							{
								_sensoresGruposAccionTcStepUpPending[numeroGrupo]--;
							}
						}
						else if (_sensoresGruposAccionTcStepDownPending[numeroGrupo]>0)
						{
							if (_sensoresDALItcStepDownEnviar(numeroGrupo)==TRUE)
							{
								_sensoresGruposAccionTcStepDownPending[numeroGrupo]--;
							}
						}						
						else if (_sensoresGruposAccionGoToScenePending[numeroGrupo][0]>0)
						{
								if (_sensoresDALIgoToSceneEnviar(numeroGrupo,_sensoresGruposAccionGoToScenePending[numeroGrupo][1])==TRUE)
                {
                    // Debug Down en buffer.
										//skipQuerySensorStatus=1;
                    _sensoresGruposAccionGoToScenePending[numeroGrupo][0]--;
                }							
						}			
						else if (_sensoresGruposAccionRestPending[numeroGrupo]>0)
						{
							if (_sensoresDALIRestEnviar(numeroGrupo)==TRUE)
							{
								_sensoresGruposAccionRestPending[numeroGrupo]--;
							}
						}
						else if (_sensoresGruposAccionRelightPending[numeroGrupo]>0)
						{
							if (_sensoresDALIRelightEnviar(numeroGrupo)==TRUE)
							{
								_sensoresGruposAccionRelightPending[numeroGrupo]--;
							}
						}
						else if (_sensoresGruposAccionInhibitPending[numeroGrupo]>0)
						{
							if (_sensoresDALIInhibitEnviar(numeroGrupo)==TRUE)
							{
								_sensoresGruposAccionInhibitPending[numeroGrupo]--;
							}
						}					
            else if ((_sensoresGruposAccionQuerySensorStatusPending[numeroGrupo][0]>0) && (skipQuerySensorStatus==0))
            {
                if (_sensoresDALIquerySensorStatusEnviar(_sensoresGruposAccionQuerySensorStatusPending[numeroGrupo][1])==TRUE)
                {
                    // Debug Query sensor en buffer.
                    _sensoresGruposAccionQuerySensorStatusPending[numeroGrupo][0]--;
                }
            }
						
						
						
						
						
						
        }
        sensoresGruposPulsadoresAccionFijaDoTask();
        sensoresGruposPulsadoresAccionBasculanteDoTask();
        sensoresGruposPulsadoresAccionTemporizadaDoTask();
        sensoresGruposDetectoresPresenciaDoTask();
        sensoresGruposReguladoresLuzDoTask();
        sensoresGruposPulsadoresAccionRegulacionDoTask();
				sensoresGruposPulsadoresTWDoTask();

    }



		sensoresGruposConfiguracion sensoresGruposConfiguracionGet(unsigned char numeroGrupo)
		{
			sensoresGruposConfiguracion configuracion;			
			unsigned char arrayCfg[8];			
			unsigned short int ptr=0;				
			if (configSensoresGruposConfiguracionGet(numeroGrupo, &arrayCfg[0])==TRUE)
			{	
				bytePop(&arrayCfg[ptr],(unsigned char*)&configuracion.detectoresPresenciaModo,&ptr);
				uint16Pop(&arrayCfg[ptr], &configuracion.detectoresPresenciaRetardo,&ptr);	
				uint16Pop(&arrayCfg[ptr], &configuracion.pulsadoresAccionTemporizadaRetardo,&ptr);	
				bytePop(&arrayCfg[ptr],	(unsigned char*)&configuracion.prevacancyLevel,&ptr);
				uint16Pop(&arrayCfg[ptr], &configuracion.prevacancyPeriodoCnt,&ptr);
				bytePop(&arrayCfg[ptr],	(unsigned char*)&configuracion.onAction,&ptr);
				bytePop(&arrayCfg[ptr],	(unsigned char*)&configuracion.offAction,&ptr);				
			}
			else
			{				
				configuracion.detectoresPresenciaModo=DETECTORES_PRESENCIA_MODO_INDETERMINADO;				
				configuracion.detectoresPresenciaRetardo=0xFFFF;				
				configuracion.pulsadoresAccionTemporizadaRetardo=0xFFFF;				
				configuracion.prevacancyLevel=0xFF;
				configuracion.prevacancyPeriodoCnt=0xFFFF;
				configuracion.onAction=0xFF;
				configuracion.offAction=0xFF;
			}						
			return configuracion;
		}

		
		BOOL sensoresGruposConfiguracionSet(unsigned char numeroGrupo, unsigned char *cfg)
		{			
			return configSensoresGruposConfiguracionSet(numeroGrupo, &cfg[0]);
		}
				
				
		
		// FUNCIONES.
