/*  TO.DO
 *
 *  31/08/2017 0 Warnings.
 *  Pendiente hacer funciones INLINE.
 *  Pendiente pasar en notificar los comporamientos como argumento.
 *  Tanto los comportamientos como las acciones puede hacerse una matriz que las recoja todas, esto también es aplicable a la parte de configuración.
 *  Selección de comportamientos.
 *  Selección de acciones.
 *
 *  HEADERS¿?
 *
 */
// NOTAS 3/07/2017, hoy 31/08 no sé que quieren decir.
// FUTURO:  MÁS ACCIONES EN LA PULSACION TEMPORIZADA, SERÁN COMUNES A LOS DETECTORES DE PRESENCIA.
// GESTIONAR EL PARO/PUESTA EN MARCHA DE LA REGULACIÓN DE LUZ.
#define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA
    #include "sensoresGruposPulsadoresAccionTemporizada.h"
#undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA

#include "mainCfg.h"

// >> DEFINICIONES
#define PERIODO_REITERACION_PROVISIONAL     DETECTORES_PRESENCIA_REITERACION_PERIODO


// << DEFINICIONES
// >> VARIABLES
		// 16 + 32 + 16 + 16 + 1  = 81 bytes
    sensoresGruposPulsadoresAccionTemporizadaEstados _sensoresGruposPulsadoresAccionTemporizadaEstado[MAX_GRUPOS];
    UINT16 _sensoresGruposPulsadoresAccionTemporizadaRetardoCnt[MAX_GRUPOS];
    unsigned char _sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCnt[MAX_GRUPOS];
    BOOL _sensoresGruposPulsadoresAccionTemporizadaInhibido[MAX_GRUPOS];
    BOOL _sensoresGruposPulsadoresAccionTemporizadaRtcOverflow;
// << VARIABLES
		
		extern unsigned char _broadcastAddress[4];

// >> FUNCIONES
    static void sensoresGruposPulsadoresAccionTemporizadaAccionSolicitar(unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, BOOL corriendo)
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
            // >> REGULADORES DE LUZ.
                if (sensoresGruposReguladoresLuzEstadoGet(numeroGrupo)!=REGULADORES_LUZ_ESTADO_NO_OPERATIVO)
                {
                    if (comportamiento.reguladoresLuz==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
												sensoresGruposReguladoresLuzEstadoActualizar ((1<<numeroGrupo));											
                    }
                }

            // << REGULADORES DE LUZ.
            // >> DETECTORES DE PRESENCIA Y PULSADORES DE ACCION TEMPORIZADA.
                
								if (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)!=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO)
								{
									if (comportamiento.detectoresPresenciaPulsadoresAccionTemporizada==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
									{
										if ((corriendo==FALSE) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_PRESENCY))
										{
											transmitirOrden=FALSE;
										}
									}
								}
								
								/*
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
								*/
            // >> DETECTORES DE PRESENCIA Y PULSADORES DE ACCION TEMPORIZADA.
        // << A N A L I S I S     C O N F L I C T O S .
        // >> E J E C U C I O N.
            if (transmitirOrden==TRUE)
            {
                 #ifdef DEBUG_ACTIONS_SEND
									debugFrame[0]=DEBUG_FRAME_TYPE;
									debugFrame[1]=0xFF;
									debugFrame[2]=0xFF;
									debugFrame[3]=DEBUG_ACTIONS_SEND_OP;
									debugFrame[4]=numeroGrupo;
									debugFrame[5]=DEBUG_ACTIONS_SEND_SUB_OP_PULSADORES_ACCION_TEMPORIZADA;
									debugFrame[6]=corriendo;
									ethSendFrame(0,&debugFrame[0],DEBUG_ACTIONS_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_ACTIONS_SEND_PORT);	
								#endif	  
								if (corriendo==TRUE)
                {
                    //_sensoresGruposAccionRecallMaxLevelSet(numeroGrupo);
										_sensoresGruposOnActionCall(numeroGrupo);
										
									
                }
                else
                {
                    //_sensoresGruposAccionOffSet(numeroGrupo);
										_sensoresGruposOffActionCall(numeroGrupo);
                }
            }
        // << E J E C U C I O N.
    }
    sensoresGruposPulsadoresAccionTemporizadaEstados sensoresGruposPulsadoresAccionTemporizadaEstadoGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposPulsadoresAccionTemporizadaEstado[numeroGrupo];
    }
    UINT16 sensoresGruposPulsadoresAccionTemporizadaRetardoCntGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposPulsadoresAccionTemporizadaRetardoCnt[numeroGrupo];
    }
    unsigned char sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCntGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCnt[numeroGrupo];
    }
    void sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCntReinicializar (unsigned char numeroGrupo)
    {
        _sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCnt[numeroGrupo]=PULSADORES_ACCION_TEMPORIZADA_REITERACION_PERIODO;;
    }
    BOOL sensoresGruposPulsadoresAccionTemporizadaInhibidoGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposPulsadoresAccionTemporizadaInhibido[numeroGrupo];
    }
    void sensoresGruposPulsadoresAccionTemporizadaInhibir (unsigned char numeroGrupo)
    {
        _sensoresGruposPulsadoresAccionTemporizadaInhibido[numeroGrupo]=TRUE;
    }
    void sensoresGruposPulsadoresAccionTemporizadaNotificar (UINT16 matrizGruposAplicable)
    {
        unsigned char numeroGrupo;
        sensoresMatrizConflictoComportamientos comportamiento;
        sensoresGruposConfiguracion configuracion;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if (_sensoresGruposForMeGet(matrizGruposAplicable,numeroGrupo)==TRUE)
            {
                configuracion=sensoresGruposConfiguracionGet(numeroGrupo);
                if (_sensoresGruposPulsadoresAccionTemporizadaEstado[numeroGrupo]!=PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO)
                {
                    comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                    sensoresGruposPulsadoresAccionTemporizadaAccionSolicitar(numeroGrupo, comportamiento, TRUE);
                    _sensoresGruposPulsadoresAccionTemporizadaRetardoCnt[numeroGrupo]=configuracion.pulsadoresAccionTemporizadaRetardo;
                    _sensoresGruposPulsadoresAccionTemporizadaInhibido[numeroGrupo]=FALSE;
                    _sensoresGruposPulsadoresAccionTemporizadaEstado[numeroGrupo]=PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO;
                }
            }
        }
    }
    void sensoresGruposPulsadoresAccionTemporizadaInit (void)
    {
        sensoresGruposConfiguracion configuracion;
        unsigned char numeroGrupo;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
					if ((_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_ACCION_TEMPORIZADA)==TRUE) || (_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_ACCION_TEMPORIZADA_REGULACION)==TRUE))
					{
						_sensoresGruposPulsadoresAccionTemporizadaEstado[numeroGrupo]=PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO;
					}
					else
					{
						_sensoresGruposPulsadoresAccionTemporizadaEstado[numeroGrupo]=PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO;
					}
           _sensoresGruposPulsadoresAccionTemporizadaRetardoCnt[numeroGrupo]=configuracion.pulsadoresAccionTemporizadaRetardo;
           _sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCnt[numeroGrupo]=0;
           _sensoresGruposPulsadoresAccionTemporizadaInhibido[numeroGrupo]=FALSE;
        }
    }
    void sensoresGruposPulsadoresAccionTemporizadaDoTask (void)
    {
        unsigned char numeroGrupo;
        sensoresMatrizConflictoComportamientos comportamiento;
        if (_sensoresGruposPulsadoresAccionTemporizadaRtcOverflow==TRUE)
        {
            for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
            {
                if (_sensoresGruposPulsadoresAccionTemporizadaEstado[numeroGrupo]==PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO)
                {
                    if (_sensoresGruposPulsadoresAccionTemporizadaRetardoCnt[numeroGrupo]>0)
                    {
                        _sensoresGruposPulsadoresAccionTemporizadaRetardoCnt[numeroGrupo]--;
                    }
                    else
                    {
                        _sensoresGruposPulsadoresAccionTemporizadaEstado[numeroGrupo]=PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO;
                        if (_sensoresGruposPulsadoresAccionTemporizadaInhibido[numeroGrupo]==FALSE)
                        {
                            comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            sensoresGruposPulsadoresAccionTemporizadaAccionSolicitar(numeroGrupo, comportamiento, FALSE);
                        }
                    }
                }
                else if (_sensoresGruposPulsadoresAccionTemporizadaEstado[numeroGrupo]==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO)
                {
                   if (_sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCnt[numeroGrupo]>0)
                   {
                       _sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCnt[numeroGrupo]--;
                   }
                   else
                   {
                       if (_sensoresGruposPulsadoresAccionTemporizadaInhibido[numeroGrupo]==FALSE)
                       {
                           comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                           comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                           comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                           comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                           comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                           //sensoresGruposPulsadoresAccionTemporizadaAccionSolicitar(numeroGrupo, comportamiento, FALSE);
                       }
                       _sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCnt[numeroGrupo]=PULSADORES_ACCION_TEMPORIZADA_REITERACION_PERIODO;;
                   }
                }
            }
            _sensoresGruposPulsadoresAccionTemporizadaRtcOverflow=FALSE;
        }
    }
    void sensoresGruposPulsadoresAccionTemporizadaRtcTick (void)
    {
        _sensoresGruposPulsadoresAccionTemporizadaRtcOverflow=TRUE;
    }
    void sensoresGruposPulsadoresAccionTemporizadaSysTick (void)
    {
    }
// << FUNCIONES
