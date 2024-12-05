#ifdef INC_SENSORES_GRUPOS_PULSADORES_TW
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_TW_H
    #define INC_SENSORES_GRUPOS_PULSADORES_TW_H
        // Propias.
            #define INC_SENSORES_GRUPOS_PULSADORES_TW_ESTADOS_E
                #include "sensoresGruposPulsadoresTW.h"
            #undef INC_SENSORES_GRUPOS_PULSADORES_TW_ESTADOS_E
            #define INC_SENSORES_GRUPOS_PULSADORES_TW_PRT
                #include "sensoresGruposPulsadoresTW.h"
            #undef INC_SENSORES_GRUPOS_PULSADORES_TW_PRT
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

#ifdef INC_SENSORES_GRUPOS_PULSADORES_TW_ESTADOS_E
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_TW_ESTADOS_E_H
    #define INC_SENSORES_GRUPOS_PULSADORES_TW_ESTADOS_E_H
    typedef enum sensoresGruposPulsadoresTWEstados_e
    {
        PULSADORES_TW_ESTADO_NO_OPERATIVO,
        PULSADORES_TW_ESTADO_CORRIENDO_ASCENDENTE,
        PULSADORES_TW_ESTADO_CORRIENDO_DESCENDENTE,
        PULSADORES_TW_ESTADO_DETENIDO,
    }sensoresGruposPulsadoresTWEstados;
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_PULSADORES_TW_PRT
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_TW_PRT_H
    #define INC_SENSORES_GRUPOS_PULSADORES_TW_PRT_H
        #define INC_SENSORES_GRUPOS_PULSADORES_TW_ESTADOS_E
            #include "sensoresGruposPulsadoresTW.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_TW_ESTADOS_E
        #define INC_SENSORES_ORIGEN_NOTIFICACION_S
            #include "sensores.h"
        #undef INC_SENSORES_ORIGEN_NOTIFICACION_S
        #define INC_SENSORES_EVENTOS_S
            #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S
            #include "dataTypes.h"
        sensoresGruposPulsadoresTWEstados sensoresGruposPulsadoresTWEstadoGet (unsigned char numeroGrupo);
        void sensoresGruposPulsadoresTWNotificar (sensoresOrigenNotificacion origen, UINT16 matrizGruposAplicable, sensoresEventos evento);
        void sensoresGruposPulsadoresTWInit (void);
        void sensoresGruposPulsadoresTWDoTask (void);
        void sensoresGruposPulsadoresTWRtcTick (void);
        void sensoresGruposPulsadoresTWSysTick (void);
    #endif
#endif




