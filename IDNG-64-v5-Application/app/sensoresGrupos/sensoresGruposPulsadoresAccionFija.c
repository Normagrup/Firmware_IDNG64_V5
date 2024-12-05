/*  TO.DO
 *
 *  2/09/2017
 *  Pueden hacerse funciones INLINE para organizar mejor el código.
 *  Por ahora lo dejo así pero habría que pasar los conflictos como argumento.
 *  Tanto los comportamientos como las acciones puede hacerse una matriz que las recoja todas, esto también es aplicable a la parte de configuración.
 *  Acciones distintas de RECALL_MAX_LEVEL y OFF.
 *  Si en la segunda acción se pretende que no sea off la cosa se complica con los reguladores de luz.
 *
 */
#define INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA
    #include "sensoresGruposPulsadoresAccionFija.h"
#undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA

#include "mainCfg.h"

	extern unsigned char _broadcastAddress[4];

// PROTOTIPOS STATIC.
    static void sensoresGruposPulsadoresAccionFijaAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, sensoresAcciones accion);
// PROTOTIPOS STATIC.
// FUNCIONES.
    static void sensoresGruposPulsadoresAccionFijaAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, sensoresAcciones accion)
    {
        BOOL transmitirOrden=TRUE;
				#ifdef DEBUG_ACTIONS_SEND
					unsigned char debugFrame[DEBUG_ACTIONS_SEND_FRAME_LENGHT];
				#endif	
        // >> A N A L I S I S     C O N F L I C T O S .
            // >> INTERRUPTOR GENERAL.
            // << INTERRUPTOR GENERAL.
            // >> PULSADORES ACCION REGULACION.
                if (sensoresGruposPulsadoresAccionRegulacionEstadoGet(numeroGrupo)!=PULSADORES_ACCION_REGULACION_ESTADO_NO_OPERATIVO)
                {
                    if (comportamiento.pulsadoresAccionRegulacion==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                        if ((sensoresGruposPulsadoresAccionRegulacionEstadoGet(numeroGrupo)==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_ASCENDENTE) || (sensoresGruposPulsadoresAccionRegulacionEstadoGet(numeroGrupo)==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_DESCENDENTE))
                        {
                        }
                    }
                }
            // << PULSADORES ACCION REGULACION.
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
            if (sensoresGruposReguladoresLuzEstadoGet(numeroGrupo)!=REGULADORES_LUZ_ESTADO_NO_OPERATIVO)
                {
																					sensoresGruposReguladoresLuzEstadoActualizar ((1<<numeroGrupo));	
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
									debugFrame[5]=DEBUG_ACTIONS_SEND_SUB_OP_PULSADORES_ACCION_FIJA;
									debugFrame[6]=accion;										// 
									ethSendFrame(0,&debugFrame[0],DEBUG_ACTIONS_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_ACTIONS_SEND_PORT);	
								#endif	
								if (accion==ACCION_RECALL_MAX_LEVEL)
                {
                    // [RecallMaxLevelSet].
                    _sensoresGruposAccionRecallMaxLevelSet(numeroGrupo);
                }
                else if (accion==ACCION_OFF)
                {
                    // [Off].
                    _sensoresGruposAccionOffSet(numeroGrupo);
                }							
								else if ((accion>=ACCION_GOTO_SCENE_0) && (accion<=ACCION_GOTO_SCENE_2))
								{									
										if (accion==ACCION_GOTO_SCENE_0)
										{
											_sensoresGruposAccionRestSet(numeroGrupo);
										}
										else if (accion==ACCION_GOTO_SCENE_1)
										{
											_sensoresGruposAccionRelightSet(numeroGrupo);
										}
										else if (accion==ACCION_GOTO_SCENE_2)
										{
											_sensoresGruposAccionRelightSet(numeroGrupo);
										}
								}							
								else if ((accion>=ACCION_GOTO_SCENE_3) && (accion<=ACCION_GOTO_SCENE_15))
								{
										// [Goto SCENE].									
										circGruposInhibidosSet(numeroGrupo, 0);
										circGruposInhibidosSet(numeroGrupo, 1);									
										_sensoresGruposAccionGoToSceneSet(numeroGrupo,(accion-ACCION_GOTO_SCENE_0));
								}		
								else if (accion==ACCION_DESINHIBIR_CIRCADIANO)
								{
									circGruposInhibidosClear(numeroGrupo, 0);
									circGruposInhibidosClear(numeroGrupo, 1);
								}
								else if (accion==ACCION_INHIBIR_CIRCADIANO)
								{
									circGruposInhibidosSet(numeroGrupo, 0);
									circGruposInhibidosSet(numeroGrupo, 1);
								}							
                else
                {
                    // [No hay accion asociada].
                }
            }
        // << E J E C U C I O N .
    }
    void sensoresGruposPulsadoresAccionFijaNotificar (UINT16 matrizGruposAplicable,  sensoresAcciones accion[SENSORES_MAX_ACCIONES_ALTERNATIVAS][SENSORES_MAX_ACCIONES])
    {
        // [Accion fija].
        unsigned char numeroGrupo;
        sensoresGruposConfiguracion configuracion;
        sensoresMatrizConflictoComportamientos comportamiento;
        sensoresAcciones accionSolicitar;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if (_sensoresGruposForMeGet(matrizGruposAplicable,numeroGrupo)==TRUE)
            {
                // [Aplicable a este grupo].
                configuracion=sensoresGruposConfiguracionGet(numeroGrupo);
                accionSolicitar=accion[configuracion.seleccionAccion][0];
                if (configuracion.seleccionComportamientoConflictos<SENSORES_MAX_CONFLICTOS_ALTERNATIVAS)
                {
                    comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                }
                sensoresGruposPulsadoresAccionFijaAccionSolicitar(numeroGrupo, comportamiento, accionSolicitar);
                // << ACCIONES.
            }
            else
            {
                // [No Aplicable a este grupo].
            }
        }
    }
    void sensoresGruposPulsadoresAccionFijaInit (void)
    {
        // [Init].
        // Intencionadamente en blanco.
    }
    void sensoresGruposPulsadoresAccionFijaDoTask (void)
    {
        // [DoTask].
        // Intencionadamente en blanco.
    }
    void sensoresGruposPulsadoresAccionFijaRtcTick (void)
    {
        // [RtcTick].
        // Intencionadamente en blanco.
    }
    void sensoresGruposPulsadoresAccionFijaSysTick (void)
    {
        // [SysTick].
        // Intencionadamente en blanco.
    }
// FUNCIONES.
