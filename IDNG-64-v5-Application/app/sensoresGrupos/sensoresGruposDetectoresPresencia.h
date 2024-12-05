/*
		C H A N G E S 
		---------------------------------			
			
		-	05/02/2019	SOPORTE FUNCIÓN CORRIDOR.

*/



#ifdef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA
    #ifndef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_H
    #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_H
        // Propias
            #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_MODOS_E
                #include "sensoresGruposDetectoresPresencia.h"
            #undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_MODOS_E
            #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_ESTADOS_E
                #include "sensoresGruposDetectoresPresencia.h"
            #undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_ESTADOS_E
            #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
                #include "sensoresGruposDetectoresPresencia.h"
            #undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
        // Propias
        // SensoresGrupos.
            #define INC_SENSORES_GRUPOS_DESC
                #include "sensoresGrupos.h"
            #undef INC_SENSORES_GRUPOS_DESC
        // SensoresGrupos.
        #define INC_SENSORES_TIPOS_E
            #include "sensores.h"
        #undef INC_SENSORES_TIPOS_E
        #define INC_SENSORES_CONFIGURACION_S
            #include "sensores.h"
        #undef INC_SENSORES_CONFIGURACION_S
        #define INC_SENSORES_PRT
            #include "sensores.h"
        #undef INC_SENSORES_PRT
        #include "sensoresConfig.h"
        #include "dataTypes.h"
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_MODOS_E
    #ifndef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_MODOS_E_H
    #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_MODOS_E_H
        typedef enum sensoresGruposDetectoresPresenciaModos_e
        {
            DETECTORES_PRESENCIA_MODO_INDETERMINADO,
            DETECTORES_PRESENCIA_MODO_NIVEL,
            DETECTORES_PRESENCIA_MODO_FLANCO,
        }sensoresGruposDetectoresPresenciaModos;
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_ESTADOS_E
    #ifndef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_ESTADOS_E_H
    #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_ESTADOS_E_H
        typedef enum sensoresGruposDetectoresPresenciaEstados_e
        {
            DETECTORES_PRESENCIA_ESTADO_NO_OPERATIVO,
            DETECTORES_PRESENCIA_ESTADO_PRESENCY,
						DETECTORES_PRESENCIA_ESTADO_PREVACANCY,
            DETECTORES_PRESENCIA_ESTADO_VACANCY,
					
        }sensoresGruposDetectoresPresenciaEstados;
    #endif
#endif
#ifdef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT
    #ifndef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT_H
    #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_PRT_H
        #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_MODOS_E
            #include "sensoresGruposDetectoresPresencia.h"
        #undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_MODOS_E
        #define INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_ESTADOS_E
            #include "sensoresGruposDetectoresPresencia.h"
        #undef INC_SENSORES_GRUPOS_DETECTORES_PRESENCIA_ESTADOS_E
            #include "dataTypes.h"
        #define INC_SENSORES_EVENTOS_S
            #include "sensores.h"
        #undef INC_SENSORES_EVENTOS_S
        sensoresGruposDetectoresPresenciaEstados sensoresGruposDetectoresPresenciaEstadoGet (unsigned char numeroGrupo);
        UINT16 sensoresGruposDetectoresPresenciaRetardoCntGet (unsigned char numeroGrupo);
        unsigned char sensoresGruposDetectoresPresenciaReiteracionPeriodoCntGet (unsigned char numeroGrupo);
        BOOL sensoresGruposDetectoresPresenciaVacancyInhibicionGet (unsigned char numeroGrupo);
        BOOL sensoresGruposDetectoresPresenciaPresencyInhibicionGet (unsigned char numeroGrupo);
        void sensoresGruposDetectoresPresenciaVacancyInhibir(unsigned char numeroGrupo);
        void sensoresGruposDetectoresPresenciaPresencyInhibir(unsigned char numeroGrupo);
        void sensoresGruposDetectoresPresenciaInhibicionesBorrar (unsigned char numeroGrupo);
        void sensoresGruposDetectoresPresenciaNotificar (unsigned char sensorAddress, UINT16 matrizGruposAplicable, sensoresEventos evento);
        void sensoresGruposDetectoresPresenciaReiteracionPeriodoCntReinicializar (unsigned char numeroGrupo);
        void sensoresGruposDetectoresPresenciaInit(void);
        void sensoresGruposDetectoresPresenciaDoTask (void);
        void sensoresGruposDetectoresPresenciaRtcTick (void);
        void sensoresGruposDetectoresPresenciaSysTick (void);
    #endif
#endif
