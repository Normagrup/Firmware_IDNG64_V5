/*

05/02/2019	SOPORTE FUNCIÓN CORRIDOR.

*/




#define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA
  #include "sensoresGruposDetectoresPresencia.h"
#undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA

#include "mainCfg.h"

#define DETECTORES_PRESENCIA_TIEMPO_PRESENCY_CUANDO_ARRANCA			30

// VARIABLES
    sensoresGruposDetectoresPresenciaEstados    _sensoresGruposDetectoresPresenciaEstado[MAX_GRUPOS];																												//	[16]												
    UINT16                                      _sensoresGruposDetectoresPresenciaRetardoCnt[MAX_GRUPOS];    																								//	[32]
    UINT16                                      _sensoresGruposDetectoresPresenciaPrevacancyPeriodoCnt[MAX_GRUPOS];   																			//	[32]					
    unsigned char                               _sensoresGruposDetectoresPresenciaPrevacancyLevel[MAX_GRUPOS];    																					//	[16]					
    unsigned char                               _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[MAX_GRUPOS];    																		//	[16]			
    BOOL                                        _sensoresGruposDetectoresPresenciaVacancyInhibido[MAX_GRUPOS];																							//	[16]		
    BOOL                                        _sensoresGruposDetectoresPresenciaPresencyInhibido[MAX_GRUPOS];																							//	[16]					
    BOOL                                        _sensoresGruposDetectoresPresenciaRtcOverflow;																															//	[16]				
    sensoresGruposDetectoresPresenciaEstados    _sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[MAX_SENSORES_NORMALINK];												//	[64]				
    UINT16                                      _sensoresGruposDetectoresPresenciaMatrizGruposAsociadosModoNivel[MAX_SENSORES_NORMALINK];										//	[128]

		extern unsigned char _broadcastAddress[4];																																																																														//  -----------------		
																																																																														//  [352]										
// VARIABLES
// PROTOTIPOS STATIC.
    static void detectoresPresenciaModoNivelEstadoIndividualSet(unsigned char sensorAddress, sensoresEventos evento);
    static void detectoresPresenciaModoNivelEstadoIndividualGet(unsigned char numeroGrupo, sensoresGruposDetectoresPresenciaEstados *estadoIndividual);
    static sensoresGruposDetectoresPresenciaEstados detectoresPresenciaModoNivelEstadoGlobalGet(unsigned char sensorAddress, sensoresGruposDetectoresPresenciaEstados *estado);
    static void sensoresGruposDetectoresPresenciaAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, sensoresGruposDetectoresPresenciaEstados estado);
    static void sensoresGruposDetectoresPreseciaModoNivelEstadoActualizar(unsigned char numeroGrupo,  sensoresEventos evento, sensoresGruposDetectoresPresenciaEstados estadoGlobal);
    static void sensoresGruposDetectoresPresenciaModoFlancoEstadoActualizar (unsigned char numeroGrupo,  sensoresEventosTipos evento, UINT16 retardo);
// PROTOTIPOS STATIC.
// FUNCIONES
    static void detectoresPresenciaModoNivelEstadoIndividualSet(unsigned char sensorAddress, sensoresEventos evento)
    {
        if (evento.eventoTipo==SENSORES_EVENTO_PRESENCY)
        {
            _sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[sensorAddress]=DETECTORES_PRESENCIA_ESTADO_PRESENCY;
        }
        else if (evento.eventoTipo==SENSORES_EVENTO_VACANCY)
        {
            _sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[sensorAddress]=DETECTORES_PRESENCIA_ESTADO_VACANCY;
        }
    }
    static void detectoresPresenciaModoNivelEstadoIndividualGet(unsigned char numeroGrupo, sensoresGruposDetectoresPresenciaEstados *estadoIndividual)
    {
        unsigned char sensorAddress;
        for (sensorAddress=0;sensorAddress<MAX_SENSORES_NORMALINK;sensorAddress++)
        {
					estadoIndividual[sensorAddress]=_sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[sensorAddress];
					/*
            if  (_sensoresGruposForMeGet(_sensoresGruposDetectoresPresenciaMatrizGruposAsociadosModoNivel[sensorAddress],numeroGrupo)==TRUE)
            {
                estadoIndividual[sensorAddress]=_sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[sensorAddress];
            }
            else
            {
                estadoIndividual[sensorAddress]=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO;
            }*/
        }
    }
    static sensoresGruposDetectoresPresenciaEstados detectoresPresenciaModoNivelEstadoGlobalGet(unsigned char sensorAddress, sensoresGruposDetectoresPresenciaEstados *estado)
    {
       /*  En esta función en caso de detectar alguno en presency se da la salida PRESENCY
        *  En caso de no detectar alguno en presency y detectar alguno en vacancy se da la salida VACANCY
        *  En caso de no detectar alguno operativo se puede dar la salida PRESENCY, la salida VACANCY o dar NO OPERATIVO que hará que todo siga en el mismo estado.
        */
			if (estado[sensorAddress]==DETECTORES_PRESENCIA_ESTADO_PRESENCY) { return DETECTORES_PRESENCIA_ESTADO_PRESENCY; }
			else if (estado[sensorAddress]==DETECTORES_PRESENCIA_ESTADO_VACANCY) { return DETECTORES_PRESENCIA_ESTADO_VACANCY; }
			else { return DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO; }
			/*
        BOOL algunoEnVacancy=FALSE;
        BOOL algunoEnPresency=FALSE;
        for (sensorAddress=0;sensorAddress<MAX_SENSORES_NORMALINK;sensorAddress++)
        {
            if (estado[sensorAddress]==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
            {
                algunoEnPresency=TRUE;
            }
            else if (estado[sensorAddress]==DETECTORES_PRESENCIA_ESTADO_VACANCY)
            {
                algunoEnVacancy=TRUE;
            }
        }
        if (algunoEnPresency==TRUE && algunoEnVacancy!=TRUE)
        {
            return DETECTORES_PRESENCIA_ESTADO_PRESENCY;
        }
        else
        {
            if (algunoEnVacancy==TRUE)
            {
                return DETECTORES_PRESENCIA_ESTADO_VACANCY;
            }
            else
            {
                // [Esto es lo que suceede cuando no hay ningún dispositivo operativo en el grupo y debería haberlo].
                return DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO;    // [w] A definir por el usuario.
                //return DETECTORES_PRESENCIA_ESTADO_VACANCY;
                //return DETECTORES_PRESENCIA_ESTADO_PRESENCY;
            }
        }
				*/
    }
    static void sensoresGruposDetectoresPresenciaAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, sensoresGruposDetectoresPresenciaEstados estado)
    {
        BOOL transmitirOrden=TRUE;			
				#ifdef DEBUG_ACTIONS_SEND
					unsigned char debugFrame[DEBUG_ACTIONS_SEND_FRAME_LENGHT];
				#endif			
        // >> A N A L I S I S     C O N F L I C T O S .
            // >> INTERRUPTOR GENERAL.
            // << INTERRUPTOR GENERAL.
            // >> PULSADORES ACCION REGULACION.
            // << PULSADORES ACCION REGULACION.
            // >> REGULADORES DE LUZ.
                if (sensoresGruposReguladoresLuzEstadoGet(numeroGrupo)!=REGULADORES_LUZ_ESTADO_NO_OPERATIVO)
                {
                    sensoresGruposReguladoresLuzEstadoActualizar ((1<<numeroGrupo));
                }
            // << REGULADORES DE LUZ.
            // >> DETECTORES DE PRESENCIA Y PULSADORES DE ACCION TEMPORIZADA.                
                if (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)!=PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO)
                {
                    if (((estado==DETECTORES_PRESENCIA_ESTADO_PREVACANCY) || (estado==DETECTORES_PRESENCIA_ESTADO_VACANCY)) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO))
                    {
                        transmitirOrden=FALSE;
                    }
                }
            // >> DETECTORES DE PRESENCIA Y PULSADORES DE ACCION TEMPORIZADA.
        // << A N A L I S I S     C O N F L I C T O S .
        // >> E J E C U C I O N .
            if (transmitirOrden==TRUE)
            {
								#ifdef DEBUG_ACTIONS_SEND
									debugFrame[0]=DEBUG_FRAME_TYPE;
									debugFrame[1]=0xFF;	
									debugFrame[2]=0xFF;						
									debugFrame[3]=DEBUG_ACTIONS_SEND_OP;
									debugFrame[4]=numeroGrupo;
									debugFrame[5]=DEBUG_ACTIONS_SEND_SUB_OP_DETECTORES_PRESENCIA;
									debugFrame[6]=estado;
									ethSendFrame(0,&debugFrame[0],DEBUG_ACTIONS_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_ACTIONS_SEND_PORT);	
								#endif	  
								if (estado==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
                {
									//_sensoresGruposAccionRecallMaxLevelSet(numeroGrupo);									
									_sensoresGruposOnActionCall(numeroGrupo);
				
                }
                else if (estado==DETECTORES_PRESENCIA_ESTADO_PREVACANCY)
                {
                    _sensoresGruposAccionDAPCSet(numeroGrupo,_sensoresGruposDetectoresPresenciaPrevacancyLevel[numeroGrupo],FALSE);
                }
                else if (estado==DETECTORES_PRESENCIA_ESTADO_VACANCY)
                {
									//_sensoresGruposAccionOffSet(numeroGrupo);									
									_sensoresGruposOffActionCall(numeroGrupo);																	
                }
            }
        // << E J E C U C I O N .
        
        
        
        
    }
		
    static void sensoresGruposDetectoresPreseciaModoNivelEstadoActualizar(unsigned char numeroGrupo,  sensoresEventos evento, sensoresGruposDetectoresPresenciaEstados estadoGlobal)
    {
        sensoresMatrizConflictoComportamientos comportamiento;
				sensoresGruposConfiguracion configuracionGrupo;
        if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
        {
            if (evento.eventoTipo==SENSORES_EVENTO_VACANCY)
            {
                if (estadoGlobal==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
                {
                }
                else if (estadoGlobal==DETECTORES_PRESENCIA_ESTADO_VACANCY)
                {
                    if (_sensoresGruposDetectoresPresenciaPrevacancyLevel[numeroGrupo]!=0xFF)
                    {
                        if (_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]==FALSE)  sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, DETECTORES_PRESENCIA_ESTADO_PREVACANCY);
                        _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PREVACANCY;										
												configuracionGrupo=sensoresGruposConfiguracionGet(numeroGrupo);														
                        _sensoresGruposDetectoresPresenciaPrevacancyPeriodoCnt[numeroGrupo]=configuracionGrupo.prevacancyPeriodoCnt;
                    }
                    else
                    {
                        if (_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]==FALSE) sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, DETECTORES_PRESENCIA_ESTADO_VACANCY);
                        _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_VACANCY;
                    }
                    _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;

                }
            }
        }
        else if ((_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PREVACANCY) || (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_VACANCY))
        {
            if (evento.eventoTipo==SENSORES_EVENTO_PRESENCY)
            {
                if (estadoGlobal==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
                {
                    if (_sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]==FALSE)
                    {
                        sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, DETECTORES_PRESENCIA_ESTADO_PRESENCY);
                    }
                    _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
                    _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PRESENCY;
                }
            }
        }
    }
		

    static void sensoresGruposDetectoresPresenciaModoFlancoEstadoActualizar (unsigned char numeroGrupo,  sensoresEventosTipos evento, UINT16 retardo)
    {
        sensoresMatrizConflictoComportamientos comportamiento;
				sensoresGruposConfiguracion configuracionGrupo;
        if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
        {
            if (evento==SENSORES_EVENTO_PRESENCY)
            {
                _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]=retardo;
                //sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, TRUE);
            }
            else if (evento==SENSORES_EVENTO_VACANCY)
            {
                if (_sensoresGruposDetectoresPresenciaPrevacancyLevel[numeroGrupo]!=0xFF)
                {
                    if (_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]==FALSE)  sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, DETECTORES_PRESENCIA_ESTADO_PREVACANCY);
                    _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PREVACANCY;								
										configuracionGrupo=sensoresGruposConfiguracionGet(numeroGrupo);										
                    _sensoresGruposDetectoresPresenciaPrevacancyPeriodoCnt[numeroGrupo]=configuracionGrupo.prevacancyPeriodoCnt;
                }
                else
                {
                    if (_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]==FALSE) sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, DETECTORES_PRESENCIA_ESTADO_VACANCY);
                    _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_VACANCY;
                }
                _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
            }
        }
        else if ((_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PREVACANCY) || (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_VACANCY))
        {
            if (evento==SENSORES_EVENTO_PRESENCY)
            {
                _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]=retardo;
                _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PRESENCY;
                if (_sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]==FALSE)
                {
                    sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, TRUE);
                }
            }
            else if (evento==SENSORES_EVENTO_VACANCY)
            {
            }
        }
    }
    sensoresGruposDetectoresPresenciaEstados sensoresGruposDetectoresPresenciaEstadoGet (unsigned char numeroGrupo)
    {
        sensoresGruposDetectoresPresenciaEstados answer;
        if ((_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PREVACANCY) || (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_VACANCY))
        {
            answer=DETECTORES_PRESENCIA_ESTADO_VACANCY;
        }
        else 
        {
            answer=_sensoresGruposDetectoresPresenciaEstado[numeroGrupo];
        }        
        return answer;
    }
    UINT16 sensoresGruposDetectoresPresenciaRetardoCntGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo];
    }
    unsigned char sensoresGruposDetectoresPresenciaReiteracionPeriodoCntGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo];
    }
    BOOL sensoresGruposDetectoresPresenciaVacancyInhibicionGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo];
    }
    BOOL sensoresGruposDetectoresPresenciaPresencyInhibicionGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo];
    }
    void sensoresGruposDetectoresPresenciaVacancyInhibir(unsigned char numeroGrupo)
    {
        _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=TRUE;
    }
    void sensoresGruposDetectoresPresenciaPresencyInhibir(unsigned char numeroGrupo)
    {
        _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=TRUE;
    }
    void sensoresGruposDetectoresPresenciaInhibicionesBorrar (unsigned char numeroGrupo)
    {
        _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
        _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
    }
		void sensoresGruposDetectoresPresenciaNotificar (unsigned char sensorAddress, UINT16 matrizGruposAplicable, sensoresEventos evento)
		{
				sensoresGruposConfiguracion configuracion;
				unsigned char numeroGrupo;
				sensoresGruposDetectoresPresenciaEstados estadoIndividual[MAX_SENSORES_NORMALINK];
				sensoresGruposDetectoresPresenciaEstados estadoGlobal[MAX_GRUPOS];
																								
				for (numeroGrupo = 0; numeroGrupo < MAX_GRUPOS; numeroGrupo++)
				{
						if (_sensoresGruposForMeGet(matrizGruposAplicable, numeroGrupo) == TRUE)
						{
								configuracion = sensoresGruposConfiguracionGet(numeroGrupo);
								if (configuracion.detectoresPresenciaModo == DETECTORES_PRESENCIA_MODO_NIVEL)
								{
										detectoresPresenciaModoNivelEstadoIndividualSet(sensorAddress, evento); // Fija el estado individual de un sensor.
										detectoresPresenciaModoNivelEstadoIndividualGet(numeroGrupo, &estadoIndividual[0]); // Obtiene la matriz de estados individuales.
										estadoGlobal[numeroGrupo] = detectoresPresenciaModoNivelEstadoGlobalGet(sensorAddress, &estadoIndividual[0]); // Obtiene el estado global del grupo.
										sensoresGruposDetectoresPreseciaModoNivelEstadoActualizar(numeroGrupo, evento, estadoGlobal[numeroGrupo]);
								}
								else if (configuracion.detectoresPresenciaModo == DETECTORES_PRESENCIA_MODO_FLANCO)
								{
										sensoresGruposDetectoresPresenciaModoFlancoEstadoActualizar(numeroGrupo, evento.eventoTipo, configuracion.detectoresPresenciaRetardo);
								}
						}
				}
		}

    void sensoresGruposDetectoresPresenciaReiteracionPeriodoCntReinicializar (unsigned char numeroGrupo)
    {
      _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=DETECTORES_PRESENCIA_REITERACION_PERIODO;
    }
    void sensoresGruposDetectoresPresenciaInit(void)
    {
        unsigned char numeroGrupo;
        unsigned char sensorAddress;
        sensoresConfiguracion configuracionSensor;
			
				sensoresGruposConfiguracion		configuracionGrupo;
			
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO;
            if ((_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL)==TRUE) || (_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL_REGULADOR_LUZ)==TRUE))
            {
                _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PRESENCY; // [w] Revisarlo. También los interfaces deberían tener un tiempo desde el arranque en que no den ninguna señal de vacancy.
                _sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]=0xFFFF;
                _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=0xFF;
            }
						
            if ((_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_FLANCO)==TRUE) || (_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL_REGULADOR_LUZ)==TRUE) || (_sensoresGruposAlgunoOperativoGet (numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_FLANCO_REGULADOR_LUZ)==TRUE))
            {
                _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_PRESENCY; // [w] Revisarlo. También los interfaces deberían tener un tiempo desde el arranque en que no den ninguna señal de vacancy.
								_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
                _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]=DETECTORES_PRESENCIA_TIEMPO_PRESENCY_CUANDO_ARRANCA;
                _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=0xFF;
            }   						
						configuracionGrupo=sensoresGruposConfiguracionGet(numeroGrupo);				
						_sensoresGruposDetectoresPresenciaPrevacancyLevel[numeroGrupo]=configuracionGrupo.prevacancyLevel;
        }
        for (sensorAddress=0;sensorAddress<MAX_SENSORES_NORMALINK;sensorAddress++)
        {
            configuracionSensor=sensoresConfiguracionGet(sensorAddress);
            if ((configuracionSensor.tipoSensor==SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL_REGULADOR_LUZ) || (configuracionSensor.tipoSensor==SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL))
            {
                _sensoresGruposDetectoresPresenciaMatrizGruposAsociadosModoNivel[sensorAddress]=configuracionSensor.matrizGrupos;                // [w] Estoy obviando la desactivación.
            }
            else
            {
                _sensoresGruposDetectoresPresenciaMatrizGruposAsociadosModoNivel[sensorAddress]=0;
            }
            _sensoresGruposDetectoresPresenciaModoNivelEstadoIndividual[sensorAddress]=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO; // [w] Revisarlo en relación con lo de arriba.
        }
    }
    void sensoresGruposDetectoresPresenciaDoTask (void)
    {
        unsigned char numeroGrupo;
        sensoresGruposConfiguracion configuracion;
        sensoresMatrizConflictoComportamientos comportamiento;
        if (_sensoresGruposDetectoresPresenciaRtcOverflow==TRUE)
        {
            for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
            {
                // >> FLANCO
                if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
                {
                    configuracion=sensoresGruposConfiguracionGet(numeroGrupo);

                    if (configuracion.detectoresPresenciaModo==DETECTORES_PRESENCIA_MODO_FLANCO)
                    {
                        if (_sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]>0)
                        {
                            _sensoresGruposDetectoresPresenciaRetardoCnt[numeroGrupo]--;
                        }
                        else
                        {
                            sensoresGruposDetectoresPresenciaModoFlancoEstadoActualizar (numeroGrupo, SENSORES_EVENTO_VACANCY, 0xFFFF);
                        }
                    }
                }
                // << FLANCO
                // >> PASO DE PREVACANCY A VACANCY.
                if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_PREVACANCY)
                {
                    if ((_sensoresGruposDetectoresPresenciaPrevacancyPeriodoCnt[numeroGrupo]>0) && (_sensoresGruposDetectoresPresenciaPrevacancyPeriodoCnt[numeroGrupo]!=0xFFFF))
                    {
                        _sensoresGruposDetectoresPresenciaPrevacancyPeriodoCnt[numeroGrupo]--;
                    }
                    else
                    {
                        if (_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]==FALSE)
                        {                            
                            if (_sensoresGruposDetectoresPresenciaPrevacancyPeriodoCnt[numeroGrupo]!=0xFFFF)
                            {
                                sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, DETECTORES_PRESENCIA_ESTADO_VACANCY);
                            }
                            else
                            {
                                sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, DETECTORES_PRESENCIA_ESTADO_PREVACANCY);
                            }
                        }
                        _sensoresGruposDetectoresPresenciaPresencyInhibido[numeroGrupo]=FALSE;
                        _sensoresGruposDetectoresPresenciaEstado[numeroGrupo]=DETECTORES_PRESENCIA_ESTADO_VACANCY;
                        _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=DETECTORES_PRESENCIA_REITERACION_PERIODO;
                    }
                }
                // << PASO DE PREVACANCY A VACANCY.
                // >> ACCION REITERATIVA
                if (_sensoresGruposDetectoresPresenciaEstado[numeroGrupo]==DETECTORES_PRESENCIA_ESTADO_VACANCY)
                {
                    if (_sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]>0)
                    {
                        _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]--;
                    }
                    else
                    {
                        if (_sensoresGruposDetectoresPresenciaVacancyInhibido[numeroGrupo]==FALSE)
                        {
                            if (_sensoresGruposDetectoresPresenciaPrevacancyPeriodoCnt[numeroGrupo]!=0xFFFF)
                            {
                                //sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, DETECTORES_PRESENCIA_ESTADO_VACANCY);
                            }
                            else if (_sensoresGruposAlgunEquipoApagadoGet(numeroGrupo)==FALSE) 
                            {
																//sensoresGruposDetectoresPresenciaAccionSolicitar (numeroGrupo, comportamiento, DETECTORES_PRESENCIA_ESTADO_PREVACANCY);
                            }
                        }
                        _sensoresGruposDetectoresPresenciaReiteracionPeriodoCnt[numeroGrupo]=DETECTORES_PRESENCIA_REITERACION_PERIODO;
                    }
                }
                // << ACCION REITERATIVA
            }
            _sensoresGruposDetectoresPresenciaRtcOverflow=FALSE;
        }
    }
    void sensoresGruposDetectoresPresenciaRtcTick (void)
    {
        _sensoresGruposDetectoresPresenciaRtcOverflow=TRUE;
    }
    void sensoresGruposDetectoresPresenciaSysTick (void)
    {
    }
// FUNCIONES