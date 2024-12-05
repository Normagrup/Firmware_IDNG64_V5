/*
 * Tema del paso de los comportamientos como parámetro, así como buscar una estructura más adecuada.
 * Manejo de inhibiciones.
 * Puesta en marcha con un determinado nivel de arco. Hacerlo en sensoresGrupos: sensoresGruposAlgunEquipoApagadoGet(...).
 * Llamada a la función actualizar cuando se cambia el arco de un determinado grupo.
 * Información desde otros tipos de sensores para que actualice el estado.
 *
 * */
 
 
 /*
 
	16/02/2018.
 
	Teniendo en cuenta el diagrama de estados del regulador de luz:
 
	// PARA DETENER EL REGULADOR:           ALGUN EQUIPO DEL GRUPO APAGADO.
	// PARA DETENER EL REGULADOR:           ALGUN EQUIPO DEL GRUPO APAGADO ó DETECTORES PRESENCIA EN VACANCY.
	// PARA DETENER EL REGULADOR:           ALGUN EQUIPO DEL GRUPO APAGADO ó (DETECTORES PRESENCIA EN VACANCY y PULSADORES ACCION TEMPORIZADA DETENIDOS).
	// PARA PONER EN MARCHA REGULADOR:      NINGUN EQUIPO DEL GRUPO APAGADO.
	// PARA PONER EN MARCHA REGULADOR:      NINGUN EQUIPO DEL GRUPO APAGADO Y DETECTORES PRESENCIA EN PRESENCY.
	// PARA PONER EN MARCHA REGULADOR:      NINGUN EQUIPO DEL GRUPO APAGADO Y (DETECTORES PRESENCIA EN PRESENCY ó PULSADORES ACCION TEMPORIZADA CORRIENDO).

	- Por tanto desde donde se notifica un cambio de nivel de luz se llama a la función de actualización.
	- Desde donde cambia el estado de detectores de presencia.
	- Desde donde cambia el estado de pulsadores de acción temporizada.
	
	En los dos casos anteriores (detectores de presencia y pulsadores de acción temporizada) se producirá una doble actualización.
	
	
	Para cada grupo hay un contador de reenvio del nivel de luz.
	



 
 
 
 
 
 
 
 
 
 
 
 */
 
 
 
 
 
 
 
 

#define INC_SENSORES_GRUPOS_REGULADORES_LUZ
    #include "sensoresGruposReguladoresLuz.h"
#undef INC_SENSORES_GRUPOS_REGULADORES_LUZ

#include "mainCfg.h"

#define PERIODO_ACTUALIZACION               				120                   // 120 segundos
#define PERIODO_ACTUALIZACION_INICIAL               5                   	// 5 segundos
#define PERIODO_ACTUALIZACION_ARRANQUE              15                   	// 15 segundos


#define LECTURA_INIT_VALUE                  0xFF


// >> VARIABLES.
		// 16 + 16 + 16 + 16 + 16 = 80 bytes
    sensoresGruposReguladoresLuzEstados     _sensoresGruposReguladoresLuzEstado[MAX_GRUPOS];
    unsigned char                           _sensoresGruposReguladoresLuzLectura[MAX_GRUPOS];
    BOOL                                    _sensoresGruposReguladoresLuzInhibido[MAX_GRUPOS];
    unsigned char                           _sensoresGruposReguladoresLuzPeriodoActualizacionCnt[MAX_GRUPOS];
		unsigned char 													_sensoresGruposReguladoresLuzInhibicionInicial[MAX_GRUPOS];
		
    BOOL                                    _sensoresGruposReguladoresLuzRtcOverflow;
		
		const unsigned char _sensoresGruposReguladoresLuzTransp[256]={1,50,76,91,101,110,116,122,127,131,135,139,142,145,147,150,152,155,157,159,161,162,164,166,167,169,170,172,173,174,175,177,178,179,180,181,182,183,184,185,186,187,188,189,190,190,191,192,193,193,194,195,196,196,197,198,198,199,200,200,201,202,202,203,203,204,204,205,206,206,207,207,208,208,209,209,210,210,211,211,212,212,212,213,213,214,214,215,215,215,216,216,217,217,217,218,218,219,219,219,220,220,220,221,221,222,222,222,223,223,223,224,224,224,225,225,225,225,226,226,226,227,227,227,228,228,228,229,229,229,229,230,230,230,230,231,231,231,232,232,232,232,233,233,233,233,234,234,234,234,235,235,235,235,236,236,236,236,237,237,237,237,237,238,238,238,238,239,239,239,239,239,240,240,240,240,241,241,241,241,241,242,242,242,242,242,243,243,243,243,243,244,244,244,244,244,244,245,245,245,245,245,246,246,246,246,246,246,247,247,247,247,247,248,248,248,248,248,248,249,249,249,249,249,249,250,250,250,250,250,250,251,251,251,251,251,251,251,252,252,252,252,252,252,253,253,253,253,253,253,253,254,254,254,254,255};
		
		extern unsigned char _broadcastAddress[4];
		
		
		
		
// << VARIABLES.
// >> PROTOTIPOS.
    static void sensoresGruposReguladoresLuzAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, unsigned char lectura);
// << PROTOTIPOS.
// >> FUNCIONES.
    static void sensoresGruposReguladoresLuzAccionSolicitar (unsigned char numeroGrupo, sensoresMatrizConflictoComportamientos comportamiento, unsigned char lectura)
    {
        BOOL transmitirOrden=TRUE;
				#ifdef DEBUG_ACTIONS_SEND
					unsigned char debugFrame[DEBUG_ACTIONS_SEND_FRAME_LENGHT];
				#endif				
        // >> A N A L I S I S     C O N F L I C T O S .
            // >> INTERRUPTOR GENERAL.
            // << INTERRUPTOR GENERAL.
            // >> PULSADORES ACCION REGULACION.
                // En principio debería de estar inhibido el regulador de luz y por tanto no trasladar órdenes, pero por si acaso.
                if (sensoresGruposPulsadoresAccionRegulacionEstadoGet(numeroGrupo)!=PULSADORES_ACCION_REGULACION_ESTADO_NO_OPERATIVO)
                {
                    if ((sensoresGruposPulsadoresAccionRegulacionEstadoGet(numeroGrupo)==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_ASCENDENTE) || (sensoresGruposPulsadoresAccionRegulacionEstadoGet(numeroGrupo)==PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_DESCENDENTE))
                    {
                        if (comportamiento.pulsadoresAccionRegulacion==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                        {
                            transmitirOrden=FALSE;
                        }
                    }
                }
            // << PULSADORES ACCION REGULACION.
            // >> DETECTOR PRESENCIA.
                if (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)!=PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO)
                {
                    if (comportamiento.detectoresPresencia==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                        if (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_PRESENCY)
                        {
                        }
                        else if (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_VACANCY)
                        {
                            transmitirOrden=FALSE;
                        }
                    }
                }
            // << DETECTOR PRESENCIA.
            // >> ACCION TEMPORIZADA.
                if (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)!=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO)
                {
                    if (comportamiento.pulsadoresAccionTemporizada==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                        if (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO)
                        {
                        }
                        else if (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO)
                        {
                            transmitirOrden=FALSE;
                        }
                    }
                }
            // << ACCION TEMPORIZADA.
            // >> DETECTORES DE PRESENCIA Y REGULADORES DE LUZ.
                if ((sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)!=PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)!=DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO))
                {
                    if (comportamiento.detectoresPresenciaPulsadoresAccionTemporizada==CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT)
                    {
                        if ((sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_VACANCY) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO))
                        {
													transmitirOrden=FALSE;
                        }
                    }
                }
            // << DETECTORES DE PRESENCIA Y REGULADORES DE LUZ.
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
									debugFrame[5]=DEBUG_ACTIONS_SEND_SUB_OP_REGULADORES_LUZ;
									debugFrame[6]=_sensoresGruposReguladoresLuzTransp[lectura];
									ethSendFrame(0,&debugFrame[0],DEBUG_ACTIONS_SEND_FRAME_LENGHT,&_broadcastAddress[0], DEBUG_ACTIONS_SEND_PORT);	
								#endif  
							_sensoresGruposAccionDAPCSet(numeroGrupo, _sensoresGruposReguladoresLuzTransp[lectura], TRUE);																
            }
        // << E J E C U C I O N .
    }
    sensoresGruposReguladoresLuzEstados sensoresGruposReguladoresLuzEstadoGet (unsigned char numeroGrupo)
    {
        return _sensoresGruposReguladoresLuzEstado[numeroGrupo];
    }
    unsigned char sensoresGruposReguladoresLuzLecturaGet(unsigned char numeroGrupo)
    {
        return _sensoresGruposReguladoresLuzLectura[numeroGrupo];
    }
    void sensoresGruposReguladoresLuzNivelActualEnvia(unsigned char numeroGrupo)
    {
        _sensoresGruposAccionDAPCSet(numeroGrupo, _sensoresGruposReguladoresLuzLectura[numeroGrupo], FALSE);    // Lo envía con prioridad.
    }
    void sensoresGruposReguladoresLuzInhibir (unsigned char numeroGrupo)
    {
        _sensoresGruposReguladoresLuzInhibido[numeroGrupo]=TRUE;
    }
    void sensoresGruposReguladoresLuzInhibicionBorrar (unsigned char numeroGrupo)
    {
        _sensoresGruposReguladoresLuzInhibido[numeroGrupo]=FALSE;
    }
    void sensoresGruposReguladoresLuzEstadoActualizar (UINT16 matrizGrupos)
    {
        /* Se llama a esta función externamente cuando se produce un cambio de nivel de luz que puede dar lugar
        * a que empiece a funcionar el regulador de luz pasándole la matriz de grupos como argumento.
        * Se llama también periódicamente.
        */

        // El estado del regulador de luz está supeditado a los detectores de presencia y los pulsadores de acción temporizada si existen.
        // Se llama cuando SUCEDE ALGO QUE PUEDA MODIFICAR ESTA SITUACION.
        // PARA DETENER EL REGULADOR:           ALGUN EQUIPO DEL GRUPO APAGADO.
        // PARA DETENER EL REGULADOR:           ALGUN EQUIPO DEL GRUPO APAGADO ó DETECTORES PRESENCIA EN VACANCY.
        // PARA DETENER EL REGULADOR:           ALGUN EQUIPO DEL GRUPO APAGADO ó (DETECTORES PRESENCIA EN VACANCY y PULSADORES ACCION TEMPORIZADA DETENIDOS).
        // PARA PONER EN MARCHA REGULADOR:      NINGUN EQUIPO DEL GRUPO APAGADO.
        // PARA PONER EN MARCHA REGULADOR:      NINGUN EQUIPO DEL GRUPO APAGADO Y DETECTORES PRESENCIA EN PRESENCY.
        // PARA PONER EN MARCHA REGULADOR:      NINGUN EQUIPO DEL GRUPO APAGADO Y (DETECTORES PRESENCIA EN PRESENCY ó PULSADORES ACCION TEMPORIZADA CORRIENDO).
        // ¿SI ESTÁN INHIBIDOS NO SON TENIDOS EN CUENTA?.
        unsigned char numeroGrupo;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if (_sensoresGruposForMeGet(matrizGrupos,numeroGrupo)==TRUE)
            {
                if (_sensoresGruposReguladoresLuzEstado[numeroGrupo]!=REGULADORES_LUZ_ESTADO_NO_OPERATIVO)
                {
                    // >> CONDICIONES DE DETENCIÓN. FALTA INHIBICIONES.
									
									if (_sensoresGruposReguladoresLuzEstado[numeroGrupo]!=REGULADORES_LUZ_ESTADO_DETENIDO)
									{										
                        if ((_sensoresGruposAlgunEquipoApagadoGet(numeroGrupo)==TRUE) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO))
                        {
                            _sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_DETENIDO;
                        }
                        else if (((_sensoresGruposAlgunEquipoApagadoGet(numeroGrupo)==TRUE) || (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_VACANCY)) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO))
                        {
                            _sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_DETENIDO;
                        }
                        else if (((_sensoresGruposAlgunEquipoApagadoGet(numeroGrupo)==TRUE) || (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO)) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO))
                        {
                            _sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_DETENIDO;
                        }
                        else if ((_sensoresGruposAlgunEquipoApagadoGet(numeroGrupo)==TRUE) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_VACANCY) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO))
                        {
                            _sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_DETENIDO;
                        }
										}
                    // >> CONDICIONES DE DETENCIÓN. FALTA INHIBICIONES.

                    // >> CONDICIONES DE PUESTA EN MARCHA. FALTA INHIBICIONES.
                  
											if (_sensoresGruposReguladoresLuzEstado[numeroGrupo]!=REGULADORES_LUZ_ESTADO_FUNCIONANDO)
											{													

												if ((_sensoresGruposAlgunEquipoApagadoGet(numeroGrupo)==FALSE) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO))
                        {
													_sensoresGruposReguladoresLuzPeriodoActualizacionCnt[numeroGrupo]=PERIODO_ACTUALIZACION_INICIAL;													
													_sensoresGruposReguladoresLuzInhibido[numeroGrupo]=FALSE;
													_sensoresGruposReguladoresLuzInhibicionInicial[numeroGrupo]=TRUE;													
													_sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_FUNCIONANDO;
                        }
                        else if ((_sensoresGruposAlgunEquipoApagadoGet(numeroGrupo)==FALSE) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_PRESENCY) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO))
                        {
													_sensoresGruposReguladoresLuzPeriodoActualizacionCnt[numeroGrupo]=PERIODO_ACTUALIZACION_INICIAL;
													_sensoresGruposReguladoresLuzInhibido[numeroGrupo]=FALSE;
													_sensoresGruposReguladoresLuzInhibicionInicial[numeroGrupo]=TRUE;													
													_sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_FUNCIONANDO;
                        }
                        else if ((_sensoresGruposAlgunEquipoApagadoGet(numeroGrupo)==FALSE) && (sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO) && (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO))
                        {
													_sensoresGruposReguladoresLuzPeriodoActualizacionCnt[numeroGrupo]=PERIODO_ACTUALIZACION_INICIAL;
													_sensoresGruposReguladoresLuzInhibido[numeroGrupo]=FALSE;
													_sensoresGruposReguladoresLuzInhibicionInicial[numeroGrupo]=TRUE;													
													_sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_FUNCIONANDO;
                        }
                        else if ((_sensoresGruposAlgunEquipoApagadoGet(numeroGrupo)==FALSE) && ((sensoresGruposDetectoresPresenciaEstadoGet(numeroGrupo)==DETECTORES_PRESENCIA_ESTADO_PRESENCY) || (sensoresGruposPulsadoresAccionTemporizadaEstadoGet(numeroGrupo)==PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO)))
                        {
													_sensoresGruposReguladoresLuzPeriodoActualizacionCnt[numeroGrupo]=PERIODO_ACTUALIZACION_INICIAL;
													_sensoresGruposReguladoresLuzInhibido[numeroGrupo]=FALSE;
													_sensoresGruposReguladoresLuzInhibicionInicial[numeroGrupo]=TRUE;													
													_sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_FUNCIONANDO;
                        }
											}
                    // << CONDICIONES DE PUESTA EN MARCHA. FALTA INHIBICIONES.
                }
            }
        }
    }
    void sensoresGruposReguladoresLuzNotificar (UINT16 matrizGruposAplicable, sensoresEventos evento)
    {
        unsigned char numeroGrupo;
        sensoresGruposConfiguracion configuracion;
        sensoresMatrizConflictoComportamientos comportamiento;
        unsigned char nivelLuzSolicitar;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if (_sensoresGruposForMeGet(matrizGruposAplicable,numeroGrupo)==TRUE)
            {
								_sensoresGruposReguladoresLuzLectura[numeroGrupo]=evento.lectura;                               // Guarda en una variable global la lectura del sensor.
								sensoresGruposReguladoresLuzEstadoActualizar (0xFFFF);
                if (_sensoresGruposReguladoresLuzEstado[numeroGrupo]==REGULADORES_LUZ_ESTADO_FUNCIONANDO)
                {   
										if ((_sensoresGruposReguladoresLuzInhibido[numeroGrupo]==FALSE) && (_sensoresGruposReguladoresLuzInhibicionInicial[numeroGrupo]==FALSE))
                    {
                        if (configuracion.seleccionComportamientoConflictos<SENSORES_MAX_CONFLICTOS_ALTERNATIVAS)
                        {
                            comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                            comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
                        }
                        // En función de configuracion.reguladoresLuzGanancia; // Guardar del 0 al 255 donde 0 es 0.05 y 255 es 12.8, es decir es multiplicar el numero por 0.05 y sumarle 0.05
                        // y reguladoresLuzOffset corregimos la lectura que nos llega desde evento.lectura.
                        nivelLuzSolicitar=evento.lectura;   // de momento:
                        sensoresGruposReguladoresLuzAccionSolicitar(numeroGrupo, comportamiento, nivelLuzSolicitar);												
                    }
                }
            }
        }
    }
    void sensoresGruposReguladoresLuzInit (void)
    {
        unsigned char numeroGrupo;
        for (numeroGrupo=0;numeroGrupo<MAX_GRUPOS;numeroGrupo++)
        {
            if ((_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_REGULADOR_LUZ)==TRUE) || (_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL_REGULADOR_LUZ)==TRUE) || (_sensoresGruposAlgunoOperativoGet(numeroGrupo, SENSORES_TIPOS_DETECTOR_PRESENCIA_FLANCO_REGULADOR_LUZ)==TRUE))
            {
                _sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_DETENIDO;
								_sensoresGruposReguladoresLuzPeriodoActualizacionCnt[numeroGrupo]=PERIODO_ACTUALIZACION_ARRANQUE;
                _sensoresGruposReguladoresLuzLectura[numeroGrupo]=LECTURA_INIT_VALUE;
                _sensoresGruposReguladoresLuzInhibido[numeroGrupo]=FALSE;
            }
						else
						{
							_sensoresGruposReguladoresLuzEstado[numeroGrupo]=REGULADORES_LUZ_ESTADO_NO_OPERATIVO;
						}
            // Si debe funcionar lo hará, ya que el regulador revisa su estado periódicamente ó cuando se detecta algún cambio.
        }

    }
    void sensoresGruposReguladoresLuzDoTask (void)
    {
      unsigned char n;  			
			sensoresMatrizConflictoComportamientos comportamiento;			
			//Periódicamente actualiza el estado de todos los grupos.
        if (_sensoresGruposReguladoresLuzRtcOverflow==TRUE)
        {          
						for (n=0;n<16;n++)
						{									
							if (_sensoresGruposReguladoresLuzEstado[n]==REGULADORES_LUZ_ESTADO_FUNCIONANDO)
							{							
								if (_sensoresGruposReguladoresLuzPeriodoActualizacionCnt[n]>0)
								{
									_sensoresGruposReguladoresLuzPeriodoActualizacionCnt[n]--;
								}
								else
								{							
									_sensoresGruposReguladoresLuzInhibicionInicial[n]=FALSE;
									if (_sensoresGruposReguladoresLuzInhibido[n]==FALSE)
									{
										comportamiento.detectoresPresencia=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
										comportamiento.detectoresPresenciaPulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
										comportamiento.pulsadoresAccionRegulacion=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
										comportamiento.pulsadoresAccionTemporizada=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;
										comportamiento.reguladoresLuz=CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT;		
										sensoresGruposReguladoresLuzAccionSolicitar (n, comportamiento, _sensoresGruposReguladoresLuzLectura[n]);
									}								
									_sensoresGruposReguladoresLuzPeriodoActualizacionCnt[n]=PERIODO_ACTUALIZACION;										
								}
							}
						}
            _sensoresGruposReguladoresLuzRtcOverflow=FALSE;
        }
    }
    void sensoresGruposReguladoresLuzRtcTick (void)
    {
        _sensoresGruposReguladoresLuzRtcOverflow=TRUE;
    }
    void sensoresGruposReguladoresLuzSysTick (void)
    {
    }
// FUNCIONES.
