#ifdef INC_SENSORES_GRUPOS_REGULADORES_LUZ
    #ifndef INC_SENSORES_GRUPOS_REGULADORES_LUZ_H
    #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_H
        // Propias.
            #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_PROPIAS
                #include "sensoresGruposReguladoresLuz.h"
            #undef INC_SENSORES_GRUPOS_REGULADORES_LUZ_PROPIAS
        // Propias.
        // SensoresGrupos.
            #define INC_SENSORES_GRUPOS_DESC
                #include "sensoresGrupos.h"
            #undef INC_SENSORES_GRUPOS_DESC
        // SensoresGrupos.
            #include "dataTypes.h"
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_REGULADORES_LUZ_PROPIAS
    #ifndef INC_SENSORES_GRUPOS_REGULADORES_LUZ_PROPIAS_H
    #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_PROPIAS_H
        #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_ESTADOS_E
            #include "sensoresGruposReguladoresLuz.h"
        #undef INC_SENSORES_GRUPOS_REGULADORES_LUZ_ESTADOS_E
        #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT
            #include "sensoresGruposReguladoresLuz.h"
        #undef INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_REGULADORES_LUZ_ESTADOS_E
    #ifndef INC_SENSORES_GRUPOS_REGULADORES_LUZ_ESTADOS_E_H
    #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_ESTADOS_E_H
        typedef enum sensoresGruposReguladoresLuzEstados_e
        {
            REGULADORES_LUZ_ESTADO_NO_OPERATIVO,
            REGULADORES_LUZ_ESTADO_FUNCIONANDO,
            REGULADORES_LUZ_ESTADO_DETENIDO,
        }sensoresGruposReguladoresLuzEstados;
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT
    #ifndef INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT_H
    #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_PRT_H
        #define INC_SENSORES_GRUPOS_REGULADORES_LUZ_ESTADOS_E
            #include "sensoresGruposReguladoresLuz.h"
        #undef INC_SENSORES_GRUPOS_REGULADORES_LUZ_ESTADOS_E
        #define INC_SENSORES_EVENTOS_S
            #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S
            #include "dataTypes.h"
        sensoresGruposReguladoresLuzEstados     sensoresGruposReguladoresLuzEstadoGet (unsigned char numeroGrupo);
        unsigned char sensoresGruposReguladoresLuzLecturaGet(unsigned char numeroGrupo);
        void sensoresGruposReguladoresLuzNivelActualEnvia(unsigned char numeroGrupo);
        void sensoresGruposReguladoresLuzInhibir (unsigned char numeroGrupo);
        void sensoresGruposReguladoresLuzInhibicionBorrar (unsigned char numeroGrupo);
        void sensoresGruposReguladoresLuzEstadoActualizar (UINT16 matrizGrupos);
        void sensoresGruposReguladoresLuzNotificar (UINT16 matrizGruposAplicable, sensoresEventos evento);
        void sensoresGruposReguladoresLuzInit (void);
        void sensoresGruposReguladoresLuzDoTask (void);
        void sensoresGruposReguladoresLuzRtcTick (void);
        void sensoresGruposReguladoresLuzSysTick (void);
    #endif
#endif
