#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_H
        // Tiene total visibilidad con sus funciones y las de su padre.
        // Suyas.
            #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_ESTADOS_E
                #include "sensoresGruposPulsadoresAccionTemporizada.h"
            #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_ESTADOS_E
            #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT
                #include "sensoresGruposPulsadoresAccionTemporizada.h"
            #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT
        // SensoresGrupos.
            #define INC_SENSORES_GRUPOS_DESC
                #include "sensoresGrupos.h"
            #undef INC_SENSORES_GRUPOS_DESC
        // SensoresGrupos.
        // Otros
        #define INC_SENSORES_ACCIONES_E
            #include "sensores.h"
        #undef INC_SENSORES_ACCIONES_E
        #define INC_SENSORES_EVENTOS_S
            #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S

        #include "sensoresConfig.h"
        #include "dataTypes.h"
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_ESTADOS_E
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_ESTADOS_E_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_ESTADOS_E_H
        typedef enum sensoresGruposPulsadoresAccionTemporizadaEstados_e
        {
            PULSADORES_ACCION_TEMPORIZADA_ESTADO_NO_OPERATIVO,
            PULSADORES_ACCION_TEMPORIZADA_ESTADO_CORRIENDO,
            PULSADORES_ACCION_TEMPORIZADA_ESTADO_DETENIDO,
        }sensoresGruposPulsadoresAccionTemporizadaEstados;
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT
    #ifndef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT_H
    #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_PRT_H
        #define INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_ESTADOS_E
            #include "sensoresGruposPulsadoresAccionTemporizada.h"
        #undef INC_SENSORES_GRUPOS_PULSADORES_ACCION_TEMPORIZADA_ESTADOS_E
            #include "dataTypes.h"
        sensoresGruposPulsadoresAccionTemporizadaEstados sensoresGruposPulsadoresAccionTemporizadaEstadoGet (unsigned char numeroGrupo);
        UINT16 sensoresGruposPulsadoresAccionTemporizadaRetardoCntGet (unsigned char numeroGrupo);
        unsigned char sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCntGet (unsigned char numeroGrupo);
        void sensoresGruposPulsadoresAccionTemporizadaReiteracionPeriodoCntReinicializar (unsigned char numeroGrupo);
        BOOL sensoresGruposPulsadoresAccionTemporizadaInhibidoGet (unsigned char numeroGrupo);
        void sensoresGruposPulsadoresAccionTemporizadaInhibir (unsigned char numeroGrupo);
        void sensoresGruposPulsadoresAccionTemporizadaNotificar (UINT16 matrizGruposAplicable);
        void sensoresGruposPulsadoresAccionTemporizadaInit (void);
        void sensoresGruposPulsadoresAccionTemporizadaDoTask (void);
        void sensoresGruposPulsadoresAccionTemporizadaRtcTick (void);
        void sensoresGruposPulsadoresAccionTemporizadaSysTick (void);
    #endif
#endif
