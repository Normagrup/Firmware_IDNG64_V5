    #ifdef INC_SENSORES_ACCIONES_E
        #ifndef INC_SENSORES_ACCIONES_E_H
        #define INC_SENSORES_ACCIONES_E_H
            typedef enum sensoresAcciones_e
            {
                ACCION_NOP,
                ACCION_RECALL_MAX_LEVEL,
                ACCION_OFF,
								ACCION_GOTO_SCENE_0,
								ACCION_GOTO_SCENE_1,
								ACCION_GOTO_SCENE_2,
								ACCION_GOTO_SCENE_3,
								ACCION_GOTO_SCENE_4,
								ACCION_GOTO_SCENE_5,
								ACCION_GOTO_SCENE_6,
								ACCION_GOTO_SCENE_7,
								ACCION_GOTO_SCENE_8,
								ACCION_GOTO_SCENE_9,
								ACCION_GOTO_SCENE_10,
								ACCION_GOTO_SCENE_11,
								ACCION_GOTO_SCENE_12,
								ACCION_GOTO_SCENE_13,
								ACCION_GOTO_SCENE_14,
								ACCION_GOTO_SCENE_15,														
								ACCION_DESINHIBIR_CIRCADIANO,
								ACCION_INHIBIR_CIRCADIANO,
            }sensoresAcciones;
        #endif
    #endif
    #ifdef INC_SENSORES_TIPOS_E
        #ifndef INC_SENSORES_TIPOS_E_H
        #define INC_SENSORES_TIPOS_E_H
            typedef enum sensoresTipos_e
            {
                SENSORES_TIPOS_NINGUNO,
                SENSORES_TIPOS_ACCION_FIJA,
                SENSORES_TIPOS_ACCION_BASCULANTE,
                SENSORES_TIPOS_ACCION_TEMPORIZADA,
                SENSORES_TIPOS_ACCION_FIJA_REGULACION,
                SENSORES_TIPOS_ACCION_BASCULANTE_REGULACION,
                SENSORES_TIPOS_ACCION_TEMPORIZADA_REGULACION,
                SENSORES_TIPOS_DETECTOR_PRESENCIA_FLANCO,
                SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL,
                SENSORES_TIPOS_REGULADOR_LUZ,
                SENSORES_TIPOS_DETECTOR_PRESENCIA_FLANCO_REGULADOR_LUZ,
                SENSORES_TIPOS_DETECTOR_PRESENCIA_NIVEL_REGULADOR_LUZ,
								SENSORES_TIPOS_PULSADOR_TW,
            } sensoresTipos;
        #endif
    #endif
    #ifdef INC_SENSORES_CONFLICTO_COMPORTAMIENTOS_E
        #ifndef INC_SENSORES_CONFLICTO_COMPORTAMIENTOS_E_H
        #define INC_SENSORES_CONFLICTO_COMPORTAMIENTOS_E_H
            typedef enum sensoresConflictoComportamientos_e
            {
                CONFLICTO_COMPORTAMIENTO_MODO_DEFAULT,
                CONFLICTO_COMPORTAMIENTO_MODO_1,
                CONFLICTO_COMPORTAMIENTO_MODO_2,
                CONFLICTO_COMPORTAMIENTO_MODO_3,
                CONFLICTO_COMPORTAMIENTO_MODO_4,
            } sensoresConflictoComportamientos;
        #endif
    #endif
    #ifdef INC_SENSORES_CONFIGURACION_S
        #ifndef INC_SENSORES_CONFIGURACION_S_H
        #define INC_SENSORES_CONFIGURACION_S_H
            #include "dataTypes.h"
            #define INC_SENSORES_TIPOS_E
                #include "sensores.h"
            #undef INC_SENSORES_TIPOS_E
            #define INC_INTERFACES_NUMERO_SERIE_S
                #include "interfaces.h"
            #undef INC_INTERFACES_NUMERO_SERIE_S
            #define INC_SENSORES_ACCIONES_E
                #include "sensores.h"
            #undef INC_SENSORES_ACCIONES_E
            #define INC_SENSORES_CONFLICTO_COMPORTAMIENTOS_E
                #include "sensores.h"
            #undef INC_SENSORES_CONFLICTO_COMPORTAMIENTOS_E
            #include "sensoresConfig.h"     // SENSORES_MAX...
            typedef struct sensoresConfiguracion_s
            {
                // añadir inhibir, matrizGruposInhibición en RAM.
                BOOL                                    instalado;                                                                          //
                BOOL                                    desactivado;                                                                        //
                sensoresTipos                           tipoSensor;                                                                         //
                UINT16                                  matrizGrupos;                                                                       //
                UINT16                                  matrizDesactivacionGrupos;                                                          //
								interfacesNumeroSerie                   interfazNumeroSerie;                                                                //
                unsigned char                           interfazCanal;                                                                      //
                sensoresAcciones                        accion[SENSORES_MAX_ACCIONES_ALTERNATIVAS][SENSORES_MAX_ACCIONES];                  // Puede organizarse mejor. Se usa para los de accion fija. Después se usará para el resto salvo para presencia y temporizados que son por grupo. Respecto a regulación puedo guardaar si son UP/DOWN o STEP UP/DOWN.
                sensoresConflictoComportamientos        comportamiento[SENSORES_MAX_CONFLICTOS_ALTERNATIVAS][SENSORES_MAX_CONFLICTOS];      // Puede organizarse mejor.
            } sensoresConfiguracion;
            #define SENSORES_CONFIGURACION_S_SIZE       45
        #endif
    #endif
    #ifdef INC_SENSORES_EVENTOS_TIPOS_E
        #ifndef INC_SENSORES_EVENTOS_TIPOS_E_H
        #define INC_SENSORES_EVENTOS_TIPOS_E_H
            typedef enum sensoresEventosTipos_e
            {
                // El orden de las primeras posiciones debe corresponderte con interfacesEventos.
                SENSORES_EVENTO_NINGUNO,
                SENSORES_EVENTO_SHORT_PRESS,
                SENSORES_EVENTO_LONG_PRESS,
                SENSORES_EVENTO_LONG_RELEASE,
                SENSORES_EVENTO_PRESENCY,
                SENSORES_EVENTO_VACANCY,
                SENSORES_EVENTO_LIGHT_REPORT,
                SENSORES_EVENTO_KEEP_ALIVE,
            } sensoresEventosTipos;
        #endif
    #endif
    #ifdef INC_SENSORES_EVENTOS_S
        #ifndef INC_SENSORES_EVENTOS_S_H
        #define INC_SENSORES_EVENTOS_S_H
            #include "dataTypes.h"
            #define INC_SENSORES_EVENTOS_TIPOS_E
                #include "sensores.h"
            #undef INC_SENSORES_EVENTOS_TIPOS_E
            typedef struct sensoresEventos_s
            {
                sensoresEventosTipos  eventoTipo;
                BOOL posicionEncender;
                BOOL sentidoAscendente;
                unsigned char lectura;                  // Normalmente Nivel de Luz.
            } sensoresEventos;
        #endif
    #endif
    #ifdef INC_SENSORES_TIPO_ORIGEN_NOTIFICACION_E
        #ifndef INC_SENSORES_TIPO_ORIGEN_NOTIFICACION_E_H
        #define INC_SENSORES_TIPO_ORIGEN_NOTIFICACION_E_H
            typedef enum sensoresTipoOrigenNotificacion_e
            {
                // Posteriormente habría que discernir entre INTERFACES_DALI y UART
                SENSORES_ORIGEN_NOTIFICACION_INDEFINIDO,
                SENSORES_ORIGEN_NOTIFICACION_INTERFAZ,
                SENSORES_ORIGEN_NOTIFICACION_NORMALINK,
            }sensoresTipoOrigenNotificacion;
        #endif
    #endif
    #ifdef INC_SENSORES_ORIGEN_NOTIFICACION_S
        #ifndef INC_SENSORES_ORIGEN_NOTIFICACION_S_H
        #define INC_SENSORES_ORIGEN_NOTIFICACION_S_H
            #define INC_SENSORES_TIPO_ORIGEN_NOTIFICACION_E
                #include "sensores.h"
            #undef INC_SENSORES_TIPO_ORIGEN_NOTIFICACION_E
            #define INC_INTERFACES_NUMERO_SERIE_S
                #include "interfaces.h"
            #undef  INC_INTERFACES_NUMERO_SERIE_S
            typedef struct sensoresOrigenNotificacion_s
            {
                sensoresTipoOrigenNotificacion  tipoOrigen;
                interfacesNumeroSerie           interfazNumeroSerie;        // Uso en ORIGEN_NOTIFICACION_INTERFAZ y ORIGEN_NOTIFICACION_NORMALINK.
                unsigned char                   interfazCanal;              // Uso en ORIGEN_NOTIFICACION_INTERFAZ y ORIGEN_NOTIFICACION_NORMALINK.
                unsigned char                   interfazAddressOrigen;      // Uso exclusivo en ORIGEN_NOTIFICACION_INTERFAZ, porque cuando recibo el LONG_PRESS tengo que esperar por el LONG_RELEASE ó ¿acaso puedo restringirlo a la parte de interfaces?.
            }sensoresOrigenNotificacion;
        #endif
    #endif
    #ifdef INC_SENSORES_PRT
        #ifndef INC_SENSORES_PRT_H
        #define INC_SENSORES_PRT_H
            #include "dataTypes.h"
            #define INC_SENSORES_CONFIGURACION_S
                #include "sensores.h"
            #undef INC_SENSORES_CONFIGURACION_S
            #define INC_INTERFACES_EVENTOS_S
                #include "interfaces.h"
            #undef INC_INTERFACES_EVENTOS_S
            #define INC_SENSORES_ORIGEN_NOTIFICACION_S
                #include "sensores.h"
            #undef INC_SENSORES_ORIGEN_NOTIFICACION_S
            void sensoresNotificar (sensoresOrigenNotificacion origenNotificacion, interfacesEventos evento);
            BOOL _sensoresDALIrecallMaxLevelEnviar (unsigned char numeroGrupo);
            BOOL _sensoresDALIoffEnviar (unsigned char numeroGrupo);
            BOOL _sensoresDALIupEnviar (unsigned char numeroGrupo);
            BOOL _sensoresDALIdownEnviar (unsigned char numeroGrupo);
            BOOL _sensoresDALIcomandoArcoDirectoEnviar(unsigned char numeroGrupo, unsigned char nivel);
            BOOL _sensoresDALIquerySensorStatusEnviar(unsigned char sensorAddress);
						BOOL _sensoresDALIRestEnviar(unsigned char numeroGrupo);
						BOOL _sensoresDALIRelightEnviar(unsigned char numeroGrupo);
						BOOL _sensoresDALIInhibitEnviar(unsigned char numeroGrupo);						
            unsigned char sensorAddressGet (unsigned char sensorAddressOrigen, unsigned char *answer);
            sensoresConfiguracion sensoresConfiguracionGet (unsigned char sensorAddress);
						BOOL sensoresConfiguracionSet (unsigned char accion, unsigned char *cfg);
						unsigned char sensoresMatrizGruposToGrupoGet (UINT16 matrizGrupos);		
						UINT16 sensoresGruposToMatrizGruposGet (unsigned char grupo);
						BOOL sensoresIniciarConfiguracion(void);
						BOOL sensoresFinalizarConfiguracion(void);						
						BOOL sensoresPeticionNiveles(unsigned char *numeroSerie, unsigned char *valores);	
						
						
        #endif
    #endif
