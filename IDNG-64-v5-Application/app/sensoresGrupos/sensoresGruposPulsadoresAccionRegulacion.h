#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_H
        // Propias.
            #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_ESTADOS_E
                #include "sensoresGruposPulsadoresAccionRegulacion.h"
            #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_ESTADOS_E
            #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_PRT
                #include "sensoresGruposPulsadoresAccionRegulacion.h"
            #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_PRT
        // Propias.
        // SensoresGrupos.
            #define INC_SENSORES_GRUPOS_DESC
                #include "sensoresGrupos.h"
            #undef INC_SENSORES_GRUPOS_DESC
        // SensoresGrupos.
        #define INC_SENSORES_ACCIONES_E
            #include "sensores.h"
        #undef INC_SENSORES_ACCIONES_E
        #define INC_SENSORES_EVENTOS_S
            #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S
            #include "sensoresConfig.h"
            #include "dataTypes.h"
            #include "daliSensores.h"
    #endif
#endif

#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_ESTADOS_E
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_ESTADOS_E_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_ESTADOS_E_H
    typedef enum sensoresGruposPulsadoresAccionRegulacionEstados_e
    {
        PULSADORES_ACCION_REGULACION_ESTADO_NO_OPERATIVO,
        PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_ASCENDENTE,
        PULSADORES_ACCION_REGULACION_ESTADO_CORRIENDO_DESCENDENTE,
        PULSADORES_ACCION_REGULACION_ESTADO_DETENIDO,
    }sensoresGruposPulsadoresAccionRegulacionEstados;
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_PRT
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_PRT_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_PRT_H
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_ESTADOS_E
            #include "sensoresGruposPulsadoresAccionRegulacion.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_REGULACION_ESTADOS_E
        #define INC_SENSORES_ORIGEN_NOTIFICACION_S
            #include "sensores.h"
        #undef INC_SENSORES_ORIGEN_NOTIFICACION_S
        #define INC_SENSORES_EVENTOS_S
            #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S
            #include "dataTypes.h"
        sensoresGruposPulsadoresAccionRegulacionEstados sensoresGruposPulsadoresAccionRegulacionEstadoGet (unsigned char numeroGrupo);
        void sensoresGruposPulsadoresAccionRegulacionNotificar (sensoresOrigenNotificacion origen, UINT16 matrizGruposAplicable, sensoresEventos evento);
        void sensoresGruposPulsadoresAccionRegulacionInit (void);
        void sensoresGruposPulsadoresAccionRegulacionDoTask (void);
        void sensoresGruposPulsadoresAccionRegulacionRtcTick (void);
        void sensoresGruposPulsadoresAccionRegulacionSysTick (void);
    #endif
#endif




