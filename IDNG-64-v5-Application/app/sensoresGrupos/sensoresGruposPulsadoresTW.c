#define INC_SENSORES_GRUPOS_PULSADORES_TW
    #include "sensoresGruposPulsadoresTW.h"
#undef INC_SENSORES_GRUPOS_PULSADORES_TW

#include "mainCfg.h"

#define PERIODO_RECARGA             PULSADORES_TW_PERIODO_RECARGA
#define PERIODOS_TIMEOUT            PULSADORES_TW_PERIODOS_TIMEOUT
// >> VARIABLES.
    // 16 + 16 + 16 + 1 + 16 + 16 = 81 bytes
		volatile sensoresGruposPulsadoresTWEstados     							 _sensoresGruposPulsadoresTWEstado[MAX_GRUPOS];																												
    volatile unsigned char                                       _sensoresGruposPulsadoresTWPeriodoActualizacionNivelCnt[MAX_GRUPOS];                                //ACTUALIZACION DE NIVEL
    volatile unsigned char                                       _sensoresGruposPulsadoresTWTimeoutCnt[MAX_GRUPOS];                                                  //TIMEOUT
    volatile BOOL                                                _sensoresGruposPulsadoresTWSysTickOverflow;
    sensoresTipoOrigenNotificacion                               _sGPTWTipoOrigenNotif[MAX_GRUPOS];                                                                  //Guarda temporalmente el origen de la notificación.
    unsigned char                                                _sGPTWInterfazAddressOrigenNotif[MAX_GRUPOS];                                                       //Guarda temporalmente la direccion de la interfaz.
// << VARIABLES.

		extern unsigned char _broadcastAddress[4];

// >> PROTOTIPOS.
    static void sensoresGruposPulsadoresTWAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, BOOL sentidoAscendente);
// << PROTOTIPOS.
// >> FUNCIONES.
    static void sensoresGruposPulsadoresTWAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, BOOL sentidoAscendente)
    {
        BOOL transmitirOrden=TRUE;
				#ifdef DEBUG_ACTIONS_SEND
					unsigned char debugFrame[DEBUG_ACTIONS_SEND_FRAME_LENGHT];
				#endif				
				circGruposInhibidosSet(numeroGrupo, 1);				
				// >> E J E C U C I O N .
            if (transmitirOrden==TRUE)
            {
                 #ifdef DEBUG_ACTIONS_SEND
									debugFrame[0]=DEBUG_FRAME_TYPE;
									debugFrame[1]=0xFF;
									debugFrame[2]=0xFF;
									debugFrame[3]=DEBUG_ACTIONS_SEND_OP;
									debugFrame[4]=numeroGrupo;
									debugFrame[5]=DEBUG_ACTIONS_SEND_SUB_OP_PULSADORES_TW;
									debugFrame[6]=sentidoAscendente;
									ethSendFrame(0,&debugFrame[0],DEBUG_ACTIONS_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_ACTIONS_SEND_PORT);	
								#endif	  
							if (sentidoAscendente==TRUE)
							{
									//_sensoresGruposAccionTcStepUpSet(numeroGrupo);
									_sensoresGruposAccionTcStepDownSet(numeroGrupo);
							}
							else
							{
									//_sensoresGruposAccionTcStepDownSet(numeroGrupo);
									_sensoresGruposAccionTcStepUpSet(numeroGrupo);
							}
            }
        // << E J E C U C I O N .
    }
    static void sensoresGruposPulsadoresTWActualizarNivel (unsigned char numeroGrupo)
    {
        /*
         * Periódicamente se llama a esta función cuando hay una regulación en curso, que por un lado ejecuta
         * los comandos correspondientes y por otro lado si el sensor es de ámbito interno, es decir está en su propia interfaz
         * pues hace pollings regularmente con la finalidad de detectar el longRelease.
         * cuando se entra en esta función ya se sabe que ...RegulacionEstado es CORRIENDO_ASCENDENTE ó CORRIENDO_DESCENDENTE.
        */
        sensoresMatrizConflictoComportamientos comportamiento;
        comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
        comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
        comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
        comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
        comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
        if (_sensoresGruposPulsadoresTWEstado[numeroGrupo]==PULSADORES_TW_ESTADO_CORRIENDO_ASCENDENTE)
        {
            sensoresGruposPulsadoresTWAccionSolicitar (numeroGrupo, comportamiento, TRUE);
        }
        if (_sensoresGruposPulsadoresTWEstado[numeroGrupo]==PULSADORES_TW_ESTADO_CORRIENDO_DESCENDENTE)
        {
            sensoresGruposPulsadoresTWAccionSolicitar (numeroGrupo, comportamiento, FALSE);
        }
        // >> Si el sensor es interno.
            if (_sGPTWTipoOrigenNotif[numeroGrupo]==SENSORES_ORIGEN_NOTIFICACION_INTERFAZ)
            {
               _sensoresGruposAccionQuerySensorStatusSet(numeroGrupo,_sGPTWInterfazAddressOrigenNotif[numeroGrupo]);
            }
        // << Si el sensor es interno.
    }
    sensoresGruposPulsadoresTWEstados sensoresGruposPulsadoresTWEstadoGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposPulsadoresTWEstado[numeroGrupo];
    }
    void sensoresGruposPulsadoresTWNotificar (sensoresOrigenNotificacion origen, UINT16 matrizGruposAplicable, sensoresEventos evento)
    {
        unsigned char numeroGrupo;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if (_sensoresGruposForMeGet(matrizGruposAplicable,numeroGrupo)==TRUE)
            {
                // Añadir como condición que haya algún equipo encendido¿?.
                if (_sensoresGruposPulsadoresTWEstado[numeroGrupo]==PULSADORES_TW_ESTADO_DETENIDO)
                {
                    if (evento.eventoTipo==SENSORES_EVENTO_LONG_PRESS)
                    {
                        if (evento.sentidoAscendente==TRUE)
                        {
                            _sensoresGruposPulsadoresTWEstado[numeroGrupo]=PULSADORES_TW_ESTADO_CORRIENDO_ASCENDENTE;
                        }
                        else
                        {
                            _sensoresGruposPulsadoresTWEstado[numeroGrupo]=PULSADORES_TW_ESTADO_CORRIENDO_DESCENDENTE;
                        }
                        _sensoresGruposPulsadoresTWPeriodoActualizacionNivelCnt[numeroGrupo]=PERIODO_RECARGA;                                                            //Periodo de actualización del nivel
                        _sensoresGruposPulsadoresTWTimeoutCnt[numeroGrupo]=PERIODOS_TIMEOUT;												
                        //sensoresGruposPulsadoresTWActualizarNivel(numeroGrupo);
                        _sGPTWTipoOrigenNotif[numeroGrupo]=origen.tipoOrigen;
                        if (origen.tipoOrigen==SENSORES_ORIGEN_NOTIFICACION_INTERFAZ)
                        {
                            _sGPTWInterfazAddressOrigenNotif[numeroGrupo]=origen.interfazAddressOrigen;


                            daliSensoresPollingTmpDisable();
                        }
                        else
                        {
                            _sGPTWInterfazAddressOrigenNotif[numeroGrupo]=0xFF;    // [No aplicable].
                        }
                    }
                    else
                    {
                    }
                }
                else if ((_sensoresGruposPulsadoresTWEstado[numeroGrupo]==PULSADORES_TW_ESTADO_CORRIENDO_ASCENDENTE) || (_sensoresGruposPulsadoresTWEstado[numeroGrupo]==PULSADORES_TW_ESTADO_CORRIENDO_DESCENDENTE))
                {
                    if (evento.eventoTipo==SENSORES_EVENTO_LONG_RELEASE)
                    {
                        _sensoresGruposPulsadoresTWEstado[numeroGrupo]=PULSADORES_TW_ESTADO_DETENIDO;
                        if (origen.tipoOrigen==SENSORES_ORIGEN_NOTIFICACION_INTERFAZ)
                        {
                            daliSensoresPollingEndTmpDisable();
                        }
                    }
                    else
                    {
                    }
                }
            }
        }
    }
    void sensoresGruposPulsadoresTWInit (void)
    {
        unsigned char numeroGrupo;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            //if (_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_ACCION_BASCULANTE_REGULACION)==TRUE)
						if (_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_PULSADOR_TW)==TRUE)
            {
                 _sensoresGruposPulsadoresTWEstado[numeroGrupo]=PULSADORES_TW_ESTADO_DETENIDO;
                //_sGPARITipoOrigenNotif[numeroGrupo]=SENSORES_ORIGEN_NOTIFICACION_INDEFINIDO;              // [No hace falta].
                //_sGPARIAddressOrigenNotif[numeroGrupo]=0;                                                 // [No hace falta].
                //_sensoresGruposPulsadoresTWPeriodoActualizacionNivelCnt[numeroGrupo]=0;     // [No hace falta].
                //_sensoresGruposPulsadoresTWTimeoutCnt[numeroGrupo]=0;                       // [No hace falta].
            }
        }
        //_sensoresGruposPulsadoresTWSysTickOverflow=FALSE;                                   // [No hace falta].
    }
    void sensoresGruposPulsadoresTWDoTask (void)
    {
        unsigned char numeroGrupo=0;
        if (_sensoresGruposPulsadoresTWSysTickOverflow==TRUE)
        {
                for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
                {
                if ((_sensoresGruposPulsadoresTWEstado[numeroGrupo]==PULSADORES_TW_ESTADO_CORRIENDO_ASCENDENTE) || (_sensoresGruposPulsadoresTWEstado[numeroGrupo]==PULSADORES_TW_ESTADO_CORRIENDO_DESCENDENTE))
                {
                    if (_sensoresGruposPulsadoresTWPeriodoActualizacionNivelCnt[numeroGrupo]>0)
                    {
                        _sensoresGruposPulsadoresTWPeriodoActualizacionNivelCnt[numeroGrupo]--;
                    }
                    else
                    {
                        if (_sensoresGruposPulsadoresTWTimeoutCnt[numeroGrupo]>0)
                        {
                           _sensoresGruposPulsadoresTWTimeoutCnt[numeroGrupo]--;
                           _sensoresGruposPulsadoresTWPeriodoActualizacionNivelCnt[numeroGrupo]=PERIODO_RECARGA;
                            sensoresGruposPulsadoresTWActualizarNivel(numeroGrupo);
													//enviar=1;
													_sensoresGruposAccionQuerySensorStatusSet(numeroGrupo,_sGPTWInterfazAddressOrigenNotif[numeroGrupo]);
                       }
                        else
                       {
                            //Timeout
                            _sensoresGruposPulsadoresTWEstado[numeroGrupo]=PULSADORES_TW_ESTADO_DETENIDO;
                            if (_sGPTWTipoOrigenNotif[numeroGrupo]==SENSORES_ORIGEN_NOTIFICACION_INTERFAZ)
                            {
                                daliSensoresPollingEndTmpDisable();
                            }
                        }
                    }
                }
            }
						
						//if (enviar==1)
						//	{
						//
						//	}
            _sensoresGruposPulsadoresTWSysTickOverflow=FALSE;
        }
    }
    void sensoresGruposPulsadoresTWRtcTick (void)
    {
    }
    void sensoresGruposPulsadoresTWSysTick (void)
    {
        _sensoresGruposPulsadoresTWSysTickOverflow=TRUE;
    }
// << FUNCIONES.
