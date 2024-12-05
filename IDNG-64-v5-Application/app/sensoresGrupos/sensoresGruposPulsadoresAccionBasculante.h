#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_H
        // Propios.
            #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_PRT
                #include "sensoresGruposPulsadoresAccionBasculante.h"
            #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_PRT
        // Propios.
        // SensoresGrupos.
            #define INC_SENSORES_GRUPOS_DESC
                #include "sensoresGrupos.h"
            #undef INC_SENSORES_GRUPOS_DESC
        // SensoresGrupos.
        #define INC_SENSORES_EVENTOS_S
            #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S



        #include "dataTypes.h"
    #endif
#endif



#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_PRT
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_PRT_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_BASCULANTE_PRT_H
        #include "dataTypes.h"
        #define INC_SENSORES_EVENTOS_S
            #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S            
        #define INC_SENSORES_ACCIONES_E
            #include "sensores.h"
        #undef INC_SENSORES_ACCIONES_E

            #include "sensoresConfig.h"


        void sensoresGruposPulsadoresAccionBasculanteNotificar (UINT16 matrizGruposAplicable, sensoresEventos evento, sensoresAcciones accion[SENSORES_MAX_ACCIONES_ALTERNATIVAS][SENSORES_MAX_ACCIONES]);
        void sensoresGruposPulsadoresAccionBasculanteInit (void);
        void sensoresGruposPulsadoresAccionBasculanteDoTask (void);
        void sensoresGruposPulsadoresAccionBasculanteRtcTick (void);
        void sensoresGruposPulsadoresAccionBasculanteSysTick (void);
    #endif
#endif
