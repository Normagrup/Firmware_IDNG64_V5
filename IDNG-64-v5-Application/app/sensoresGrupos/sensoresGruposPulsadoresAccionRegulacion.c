/*  TO.DO
 *
 *  31/08/2017  Warnings.
 *  Pendiente hacer funciones INLINE.
 *  Pendiente pasar en notificar los comporamientos como argumento.
 *  Tanto los comportamientos como las acciones puede hacerse una matriz que las recoja todas, esto también es aplicable a la parte de configuración.
 *  Selección de comportamientos.
 *  Selección de acciones.
 *   // A ver que hacemos con el SYSTICK. ESCALADO ó FLAG COMUN?
    // añadir una especie de evento timeout asimilado a long release para que los estado se cambien siempre desde una única función.
    // añadir sensoresConflictoComportamiento para el combinado de presencia y pulsadores accion temporizada ó contemplar la combinación.
 *  Dependencias. Sólo debería añadirse su propio file? el de su padre?.
 *  Pte revisar todo lo referente a conflictos.
 *  Matriz de comportamientos en RAM en principio uno por grupo, de cara a la función ...ActualizarNivel
 *  problema en el header. tengo que comentar la llamada a ...Notificar.
 *
 */
#define INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION
    #include "sensoresGruposPulsadoresAccionRegulacion.h"
#undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION

#include "mainCfg.h"

#define PERIODO_RECARGA             PULSADORES_ACCION_REGULACION_PERIODO_RECARGA
#define PERIODOS_TIMEOUT            PULSADORES_ACCION_REGULACION_PERIODOS_TIMEOUT
// >> VARIABLES.
    // 16 + 16 + 16 + 1 + 16 + 16 = 81 bytes
		volatile sensoresGruposPulsadoresAccionRegulacionEstados     _sensoresGruposPulsadoresAccionRegulacionEstado[MAX_GRUPOS];																												
    volatile unsigned char                                       _sensoresGruposPulsadoresAccionRegulacionPeriodoActualizacionNivelCnt[MAX_GRUPOS];                                //ACTUALIZACION DE NIVEL
    volatile unsigned char                                       _sensoresGruposPulsadoresAccionRegulacionTimeoutCnt[MAX_GRUPOS];                                                  //TIMEOUT
    volatile BOOL                                                _sensoresGruposPulsadoresAccionRegulacionSysTickOverflow;
    sensoresTipoOrigenNotificacion                               _sGPARTipoOrigenNotif[MAX_GRUPOS];                                                                                //Guarda temporalmente el origen de la notificación.
    unsigned char                                                _sGPARInterfazAddressOrigenNotif[MAX_GRUPOS];                                                                     //Guarda temporalmente la direccion de la interfaz.
// << VARIABLES.

		extern unsigned char _broadcastAddress[4];

// >> PROTOTIPOS.
    static void sensoresGruposPulsadoresAccionRegulacionAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, BOOL sentidoAscendente);
// << PROTOTIPOS.
// >> FUNCIONES.
    static void sensoresGruposPulsadoresAccionRegulacionAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, BOOL sentidoAscendente)
    {
        BOOL transmitirOrden=TRUE;
				#ifdef DEBUG_ACTIONS_SEND
					unsigned char debugFrame[DEBUG_ACTIONS_SEND_FRAME_LENGHT];
				#endif	
        // >> A N A L I S I S     C O N F L I C T O S .
            // >> INTERRUPTOR GENERAL.
            // << INTERRUPTOR GENERAL.
            // >> DETECTOR PRESENCIA.
                if ((sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)!=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO))
                {
                    if (comportamiento.detectoresPresencia==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                      sensoresGruposDetectoresPresenciaReiteracionPeriodoCntReinicializar(numeroGrupo);  
											if (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
                        {
                            // Si los detectores de presencia están en ESTADO_PRESENCY transmite la orden.
                        }
                        else if (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_VACANCY)
                        {
                            // Si los detectores de presencia están en ESTADO_VACANCY bloquea la orden.
                            transmitirOrden=FALSE;
                        }
                    }
                }
            // << DETECTOR PRESENCIA.
            // >> ACCION TEMPORIZADA.
                if ((sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)!=PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO))
                {
                    if (comportamiento.pulsadoresAccionTemporizada==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                        if (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO)
                        {
                            // Si los pulsadores de accion temporizada están en ESTADO_CORRIENDO transmite la orden.
                        }
                        else if (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO)
                        {
                            // Si los pulsadores de accion temporizada están en ESTADO_DETENIDO bloquea la orden.
                            transmitirOrden=FALSE;
                        }
                    }
                }
            // << ACCION TEMPORIZADA.
            // >> REGULADORES DE LUZ.
                // Al contrario que el los pulsadores de Accion fija, aquí en la acción por defecto deberíamos inhibir el regulador de luz si está funcionando.
                if (sensoresGruposReguladoresLuzEstadoGet(numeroGrupo)!=REGULADORES_LUZ_ESTADO_NO_OPERATIVO)
                {
                    if (comportamiento.reguladoresLuz==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                        if (sensoresGruposReguladoresLuzEstadoGet(numeroGrupo)==REGULADORES_LUZ_ESTADO_FUNCIONANDO)
                        {
                            sensoresGruposReguladoresLuzInhibir(numeroGrupo);
                        }
                        else if (sensoresGruposReguladoresLuzEstadoGet(numeroGrupo)==REGULADORES_LUZ_ESTADO_DETENIDO)
                        {
                            // Aquí no sé que hacer por que supuestamente estarían apagadas de modo que no reaccionarían a la orden de todas maneras.
                            // Parace mejor idea en principio transmitir la orden.
                            // Puede haber diferencias en la decisión por ejemplo en el caso de que una única luminaria esté apagada.
                            // Tal y como lo estoy haciendo el resto reaccionarían.
                        }
                    }
                }
            // << REGULADORES DE LUZ.
            // >> DETECTORES DE PRESENCIA Y PULSADORES DE ACCION TEMPORIZADA.
                if ((sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)!=PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)!=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO))
                {
                    if (comportamiento.detectoresPresenciaPulsadoresAccionTemporizada==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                        if ((sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_VACANCY) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO))
                        {
                            // Si los detectores de presencia están en ESTADO_VACANCY y los pulsadores de acción temporizada están en ESTADO_DETENIDO bloquea la orden.
                            transmitirOrden=FALSE;
                        }
                        else
                        {
                            // En caso contrario transmite la orden.
                        }
                    }
                }
            // << DETECTORES DE PRESENCIA Y PULSADORES DE ACCION TEMPORIZADA.
								
								
						circGruposInhibidosSet (numeroGrupo, 0);
								
        // >> A N A L I S I S     C O N F L I C T O S .

        // >> E J E C U C I O N .

            if (transmitirOrden==TRUE)
            {
                 #ifdef DEBUG_ACTIONS_SEND
									debugFrame[0]=DEBUG_FRAME_TYPE;
									debugFrame[1]=0xFF;
									debugFrame[2]=0xFF;
									debugFrame[3]=DEBUG_ACTIONS_SEND_OP;
									debugFrame[4]=numeroGrupo;
									debugFrame[5]=DEBUG_ACTIONS_SEND_SUB_OP_PULSADORES_ACCION_REGULACION;
									debugFrame[6]=sentidoAscendente;
									ethSendFrame(0,&debugFrame[0],DEBUG_ACTIONS_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_ACTIONS_SEND_PORT);	
								#endif	  
							if (sentidoAscendente==TRUE)
                {
                    _sensoresGruposAccionUpSet(numeroGrupo);
                }
                else
                {
                    _sensoresGruposAccionDownSet(numeroGrupo);
                }
            }
        // << E J E C U C I O N .
    }
    static void sensoresGruposPulsadoresAccionRegulacionActualizarNivel (unsigned char numeroGrupo)
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
        if (_sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_ASCENDENTE)
        {
            sensoresGruposPulsadoresAccionRegulacionAccionSolicitar (numeroGrupo, comportamiento, TRUE);
        }
        if (_sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_DESCENDENTE)
        {
            sensoresGruposPulsadoresAccionRegulacionAccionSolicitar (numeroGrupo, comportamiento, FALSE);
        }
        // >> Si el sensor es interno.
            if (_sGPARTipoOrigenNotif[numeroGrupo]==SENSORES_ORIGEN_NOTIFICACION_INTERFAZ)
            {
               _sensoresGruposAccionQuerySensorStatusSet(numeroGrupo,_sGPARInterfazAddressOrigenNotif[numeroGrupo]);
            }
        // << Si el sensor es interno.
    }
    sensoresGruposPulsadoresAccionRegulacionEstados sensoresGruposPulsadoresAccionRegulacionEstadoGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo];
    }
    void sensoresGruposPulsadoresAccionRegulacionNotificar (sensoresOrigenNotificacion origen, UINT16 matrizGruposAplicable, sensoresEventos evento)
    {
        unsigned char numeroGrupo;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if (_sensoresGruposForMeGet(matrizGruposAplicable,numeroGrupo)==TRUE)
            {
                // Añadir como condición que haya algún equipo encendido¿?.
                if (_sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]==PULSADORES_ACCION_REGULACION_ESTADO_DETENIDO)
                {
                    if (evento.eventoTipo==SENSORES_EVENTO_LONG_PRESS)
                    {
                        if (evento.sentidoAscendente==TRUE)
                        {
                            _sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]=PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_ASCENDENTE;
													 _sensoresGruposAccionOnAndStepUpSet(numeroGrupo);
                        }
                        else
                        {
                            _sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]=PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_DESCENDENTE;
                        }
                        _sensoresGruposPulsadoresAccionRegulacionPeriodoActualizacionNivelCnt[numeroGrupo]=PERIODO_RECARGA;                                                            //Periodo de actualización del nivel
                        _sensoresGruposPulsadoresAccionRegulacionTimeoutCnt[numeroGrupo]=PERIODOS_TIMEOUT;												
                        //sensoresGruposPulsadoresAccionRegulacionActualizarNivel(numeroGrupo);
                        _sGPARTipoOrigenNotif[numeroGrupo]=origen.tipoOrigen;
                        if (origen.tipoOrigen==SENSORES_ORIGEN_NOTIFICACION_INTERFAZ)
                        {
                            _sGPARInterfazAddressOrigenNotif[numeroGrupo]=origen.interfazAddressOrigen;


                            daliSensoresPollingTmpDisable();
                        }
                        else
                        {
                            _sGPARInterfazAddressOrigenNotif[numeroGrupo]=0xFF;    // [No aplicable].
                        }
                    }
                    else
                    {
                    }
                }
                else if ((_sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_ASCENDENTE) || (_sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_DESCENDENTE))
                {
                    if (evento.eventoTipo==SENSORES_EVENTO_LONG_RELEASE)
                    {
                        _sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]=PULSADORES_ACCION_REGULACION_ESTADO_DETENIDO;
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
    void sensoresGruposPulsadoresAccionRegulacionInit (void)
    {
        unsigned char numeroGrupo;
				unsigned char enviar;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if ((_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_ACCION_FIJA_REGULACION)==TRUE) || (_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_ACCION_BASCULANTE_REGULACION)==TRUE) || (_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_ACCION_TEMPORIZADA_REGULACION)==TRUE))
            {
                 _sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]=PULSADORES_ACCION_REGULACION_ESTADO_DETENIDO;
                //_sGPARITipoOrigenNotif[numeroGrupo]=SENSORES_ORIGEN_NOTIFICACION_INDEFINIDO;              // [No hace falta].
                //_sGPARIAddressOrigenNotif[numeroGrupo]=0;                                                 // [No hace falta].
                //_sensoresGruposPulsadoresAccionRegulacionPeriodoActualizacionNivelCnt[numeroGrupo]=0;     // [No hace falta].
                //_sensoresGruposPulsadoresAccionRegulacionTimeoutCnt[numeroGrupo]=0;                       // [No hace falta].
            }
        }
        //_sensoresGruposPulsadoresAccionRegulacionSysTickOverflow=FALSE;                                   // [No hace falta].
    }
    void sensoresGruposPulsadoresAccionRegulacionDoTask (void)
    {
        unsigned char numeroGrupo=0;
				unsigned char enviar=0;
        if (_sensoresGruposPulsadoresAccionRegulacionSysTickOverflow==TRUE)
        {
                for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
                {
                if ((_sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_ASCENDENTE) || (_sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_DESCENDENTE))
                {
                    if (_sensoresGruposPulsadoresAccionRegulacionPeriodoActualizacionNivelCnt[numeroGrupo]>0)
                    {
                        _sensoresGruposPulsadoresAccionRegulacionPeriodoActualizacionNivelCnt[numeroGrupo]--;
                    }
                    else
                    {
                        if (_sensoresGruposPulsadoresAccionRegulacionTimeoutCnt[numeroGrupo]>0)
                        {
                           _sensoresGruposPulsadoresAccionRegulacionTimeoutCnt[numeroGrupo]--;
                           _sensoresGruposPulsadoresAccionRegulacionPeriodoActualizacionNivelCnt[numeroGrupo]=PERIODO_RECARGA;
                            sensoresGruposPulsadoresAccionRegulacionActualizarNivel(numeroGrupo);
													//enviar=1;
													_sensoresGruposAccionQuerySensorStatusSet(numeroGrupo,_sGPARInterfazAddressOrigenNotif[numeroGrupo]);
                       }
                        else
                       {
                            //Timeout
                            _sensoresGruposPulsadoresAccionRegulacionEstado[numeroGrupo]=PULSADORES_ACCION_REGULACION_ESTADO_DETENIDO;
                            if (_sGPARTipoOrigenNotif[numeroGrupo]==SENSORES_ORIGEN_NOTIFICACION_INTERFAZ)
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
            _sensoresGruposPulsadoresAccionRegulacionSysTickOverflow=FALSE;
        }
    }
    void sensoresGruposPulsadoresAccionRegulacionRtcTick (void)
    {
    }
    void sensoresGruposPulsadoresAccionRegulacionSysTick (void)
    {
        _sensoresGruposPulsadoresAccionRegulacionSysTickOverflow=TRUE;
    }
// << FUNCIONES.
