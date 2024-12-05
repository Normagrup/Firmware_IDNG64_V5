#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_H
        // Propias.
            #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_PRT
                #include "sensoresGruposPulsadoresAccionFija.h"
            #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_PRT
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
        #include "sensoresConfig.h"                             // Lo necesito para SENSORES_MAX_ACCIONES_ALTERNATIVAS y SENSORES_MAX_ACCIONES
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_PRT
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_PRT_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_FIJA_PRT_H
        #include "dataTypes.h"
        #define INC_SENSORES_ACCIONES_E
            #include "sensores.h"
        #undef INC_SENSORES_ACCIONES_E

        #include "sensoresConfig.h"

        void sensoresGruposPulsadoresAccionFijaNotificar (UINT16 matrizGruposAplicable,  sensoresAcciones accion[SENSORES_MAX_ACCIONES_ALTERNATIVAS][SENSORES_MAX_ACCIONES]);
        void sensoresGruposPulsadoresAccionFijaInit (void);
        void sensoresGruposPulsadoresAccionFijaDoTask (void);
        void sensoresGruposPulsadoresAccionFijaRtcTick (void);
        void sensoresGruposPulsadoresAccionFijaSysTick (void);
    #endif
#endif




