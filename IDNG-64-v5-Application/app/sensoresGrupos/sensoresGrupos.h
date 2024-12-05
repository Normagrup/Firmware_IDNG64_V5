/*

05/02/2019	SOPORTE FUNCIÓN CORRIDOR.

*/



#ifdef INC_SENSORES_GRUPOS
    #ifndef INC_SENSORES_GRUPOS_H
    #define INC_SENSORES_GRUPOS_H
        // Propias.
            #define INC_SENSORES_GRUPOS_CONFIGURACION_S
                #include "sensoresGrupos.h"
            #undef INC_SENSORES_GRUPOS_CONFIGURACION_S
            #define INC_SENSORES_GRUPOS_MATRIZ_CONFLICTO_COMPORTAMIENTOS_S
                #include "sensoresGrupos.h"
            #undef INC_SENSORES_GRUPOS_MATRIZ_CONFLICTO_COMPORTAMIENTOS_S
            #define INC_SENSORES_GRUPOS_NOTIFICACION_PARAMETROS_S
                #include "sensoresGrupos.h"
            #undef INC_SENSORES_GRUPOS_NOTIFICACION_PARAMETROS_S
            #define INC_SENSORES_GRUPOS_PRT
                #include "sensoresGrupos.h"
            #undef INC_SENSORES_GRUPOS_PRT
        // Propias.
        #define INC_SENSORES_TIPOS_E
            #include "sensores.h"
        #undef INC_SENSORES_TIPOS_E
        #define INC_SENSORES_ORIGEN_NOTIFICACION_S
            #include "sensores.h"
        #undef  INC_SENSORES_ORIGEN_NOTIFICACION_S
        #define INC_SENSORES_CONFIGURACION_S
              #include "sensores.h"
        #undef INC_SENSORES_CONFIGURACION_S
        #define INC_SENSORES_EVENTOS_S
              #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S
        #define INC_SENSORES_PRT
              #include "sensores.h"
        #undef  INC_SENSORES_PRT
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_PRT
            #include "sensoresGruposPulsadoresAccionFija.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_PRT
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_PRT
            #include "sensoresGruposPulsadoresAccionBasculante.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_PRT
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_PRT
            #include "sensoresGruposPulsadoresAccionRegulacion.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_PRT
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT
            #include "sensoresGruposPulsadoresAccionTemporizada.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT
        #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT
            #include "sensoresGruposReguladoresLuz.h"
        #undef INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT
        #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
            #include "sensoresGruposDetectoresPresencia.h"
        #undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
				
				#define INC_SENSORES_GRUPOS_PULSADORES_TW_PRT
            #include "sensoresGruposPulsadoresTW.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_TW_PRT
				
            #include "macros.h"
    #endif
#endif

#ifdef INC_SENSORES_GRUPOS_PROPIAS
    #ifndef INC_SENSORES_GRUPOS_PROPIAS_H
    #define INC_SENSORES_GRUPOS_PROPIAS_H
        #define INC_SENSORES_GRUPOS_CONFIGURACION_S
            #include "sensoresGrupos.h"
        #undef INC_SENSORES_GRUPOS_CONFIGURACION_S
        #define INC_SENSORES_GRUPOS_MATRIZ_CONFLICTO_COMPORTAMIENTOS_S
            #include "sensoresGrupos.h"
        #undef INC_SENSORES_GRUPOS_MATRIZ_CONFLICTO_COMPORTAMIENTOS_S
        #define INC_SENSORES_GRUPOS_NOTIFICACION_PARAMETROS_S
            #include "sensoresGrupos.h"
        #undef INC_SENSORES_GRUPOS_NOTIFICACION_PARAMETROS_S
        #define INC_SENSORES_GRUPOS_PRT
            #include "sensoresGrupos.h"
        #undef INC_SENSORES_GRUPOS_PRT
    #endif
#endif

#ifdef INC_SENSORES_GRUPOS_DESC
    #ifndef INC_SENSORES_GRUPOS_DESC_H
    #define INC_SENSORES_GRUPOS_DESC_H
        #define INC_SENSORES_GRUPOS_PROPIAS
            #include "sensoresGrupos.h"
        #undef INC_SENSORES_GRUPOS_PROPIAS
        #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
            #include "sensoresGruposDetectoresPresencia.h"
        #undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_PRT
            #include "sensoresGruposPulsadoresAccionBasculante.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_PRT
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_PRT
            #include "sensoresGruposPulsadoresAccionFija.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_PRT
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_PRT
            #include "sensoresGruposPulsadoresAccionRegulacion.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_PRT
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT
            #include "sensoresGruposPulsadoresAccionTemporizada.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT
        #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT
            #include "sensoresGruposReguladoresLuz.h"
        #undef INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_CONFIGURACION_S
    #ifndef INC_SENSORES_GRUPOS_CONFIGURACION_S_H
    #define INC_SENSORES_GRUPOS_CONFIGURACION_S_H
        #include "dataTypes.h"
        #define INC_SENSORES_ACCIONES_E
            #include "sensores.h"
        #undef INC_SENSORES_ACCIONES_E
        #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
            #include "sensoresGruposDetectoresPresencia.h"
        #undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
        typedef struct sensoresGruposConfiguracion_s
        {
            sensoresGruposDetectoresPresenciaModos  detectoresPresenciaModo;                            // Lo lee 1 vez por segundo...
            UINT16                                  detectoresPresenciaRetardo;
            UINT16                                  pulsadoresAccionTemporizadaRetardo;
            unsigned char                           seleccionAccion;
            unsigned char                           seleccionComportamientoConflictos;   
						unsigned char														prevacancyLevel;
						UINT16																	prevacancyPeriodoCnt;
						unsigned char														onAction;
						unsigned char														offAction;					
        }sensoresGruposConfiguracion;
        #define SENSORES_GRUPOS_CONFIGURACION_S_SIZE    12
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_MATRIZ_CONFLICTO_COMPORTAMIENTOS_S
    #ifndef INC_SENSORES_GRUPOS_MATRIZ_CONFLICTO_COMPORTAMIENTOS_S_H
    #define INC_SENSORES_GRUPOS_MATRIZ_CONFLICTO_COMPORTAMIENTOS_S_H
        #define INC_SENSORES_CONFLICTO_COMPORTAMIENTOS_E
            #include "sensores.h"
        #undef INC_SENSORES_CONFLICTO_COMPORTAMIENTOS_E
        typedef struct sensoresMatrizConflictoComportamientos_s
        {
            sensoresConflictoComportamientos    detectoresPresencia;
            sensoresConflictoComportamientos    reguladoresLuz;
            sensoresConflictoComportamientos    pulsadoresAccionTemporizada;
            sensoresConflictoComportamientos    detectoresPresenciaPulsadoresAccionTemporizada;
            sensoresConflictoComportamientos    pulsadoresAccionRegulacion;
        }sensoresMatrizConflictoComportamientos;
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_NOTIFICACION_PARAMETROS_S
    #ifndef INC_SENSORES_GRUPOS_NOTIFICACION_PARAMETROS_S_H
    #define INC_SENSORES_GRUPOS_NOTIFICACION_PARAMETROS_S_H
        #include "dataTypes.h"
        #define INC_SENSORES_ACCIONES_E
            #include "sensores.h"
        #undef INC_SENSORES_ACCIONES_E
        typedef struct sensoresGruposNotificacionParametros_s
        {
            BOOL posicionEncender;
            BOOL sentidoAscendente;
            unsigned char lectura;
            sensoresAcciones accion;
            UINT16 retardoPulsadoresAccionTemporizada;
            UINT16 retardoDetectoresPresencia;
        } sensoresGruposNotificacionParametros;
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_PRT
    #ifndef INC_SENSORES_GRUPOS_PRT_H
    #define INC_SENSORES_GRUPOS_PRT_H
        #include "dataTypes.h"
        #define INC_SENSORES_TIPOS_E
            #include "sensores.h"
        #undef INC_SENSORES_TIPOS_E
        #define INC_SENSORES_ORIGEN_NOTIFICACION_S
            #include "sensores.h"
        #undef INC_SENSORES_ORIGEN_NOTIFICACION_S
        #define INC_SENSORES_CONFIGURACION_S
            #include "sensores.h"
        #undef INC_SENSORES_CONFIGURACION_S
        #define INC_SENSORES_EVENTOS_S
            #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S
        #define INC_SENSORES_GRUPOS_CONFIGURACION_S
            #include "sensoresGrupos.h"
        #undef INC_SENSORES_GRUPOS_CONFIGURACION_S
        BOOL _sensoresGruposAlgunoOperativoGet (unsigned char numeroGrupo, sensoresTipos tipo);
        BOOL _sensoresGruposDesactivadoGet(UINT16 matrizGrupos, unsigned char numeroGrupo);
        BOOL _sensoresGruposForMeGet(UINT16 matrizGrupos, unsigned char numeroGrupo);
        BOOL _sensoresGruposAlgunEquipoApagadoGet(unsigned char numeroGrupo);
        BOOL _sensoresGruposPulsadoresDetectoresPresenciaVacancyGet (unsigned char numeroGrupo);
        void _sensoresGruposAccionRecallMaxLevelSet(unsigned char numeroGrupo);
        void _sensoresGruposAccionOffSet(unsigned char numeroGrupo);
        void _sensoresGruposAccionDAPCSet(unsigned char numeroGrupo, unsigned char nivel, BOOL origenReguladoresLuz);
        void _sensoresGruposAccionUpSet(unsigned char numeroGrupo);
        void _sensoresGruposAccionDownSet(unsigned char numeroGrupo);								
				void _sensoresGruposAccionTcStepUpSet(unsigned char numeroGrupo);
				void _sensoresGruposAccionTcStepDownSet(unsigned char numeroGrupo);
				
				void _sensoresGruposAccionRestSet(unsigned char numeroGrupo);
				void _sensoresGruposAccionRelightSet(unsigned char numeroGrupo);
				void _sensoresGruposAccionInhibitSet(unsigned char numeroGrupo);
		
				
				void _sensoresGruposOnActionCall(unsigned char numeroGrupo);
				void _sensoresGruposOffActionCall(unsigned char numeroGrupo);
				
				
				void _sensoresGruposAccionGoToSceneSet(unsigned char numeroGrupo, unsigned char scene);				
        void _sensoresGruposAccionQuerySensorStatusSet(unsigned char numeroGrupo, unsigned char sensorAddress);
        void sensoresGruposInit (void);
        void sensoresGruposRtcTick (void);
        void sensoresGruposSysTick (void);
        void sensoresGruposNotificar (unsigned char sensorAddress, sensoresOrigenNotificacion origenNotificacion, sensoresConfiguracion configuracionSensor, sensoresEventos evento);
        void sensoresGruposDoTask (void);
        sensoresGruposConfiguracion sensoresGruposConfiguracionGet(unsigned char numeroGrupo);
				BOOL sensoresGruposConfiguracionSet(unsigned char numeroGrupo, unsigned char *cfg);
    #endif
#endif

