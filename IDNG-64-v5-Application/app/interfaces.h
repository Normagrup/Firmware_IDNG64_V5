    #ifdef INTERFACES_DEF
        #ifndef INTERFACES_DEF_H
            #define INTERFACES_DEF_H
            #define INTERFACES_MAX  16  // N<=64
        #endif
    #endif

    #ifdef INC_INTERFACES_NUMERO_SERIE_S
        #ifndef INC_INTERFACES_NUMERO_SERIE_S_H
        #define INC_INTERFACES_NUMERO_SERIE_S_H
            #include "dataTypes.h"
            typedef struct interfacesNumeroSerie_s
            {
                /*	261217
								UINT16 ejercicio;
                unsigned char mes;
                unsigned char ordenTrabajo[3];
                unsigned char id[3];
								*/
							
								unsigned char byte[4];
            } interfacesNumeroSerie;
        #endif
    #endif
    #ifdef INC_INTERFACES_MODOS_INTERFAZ_E
        #ifndef INC_INTERFACES_MODOS_INTERFAZ_E_H
        #define INC_INTERFACES_MODOS_INTERFAZ_E_H
            typedef enum interfacesModosInterfaz_e
            {
                INTERFACES_MODO_INTERFAZ_NO_OPERATIVO,                            // Todos
								INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE,  	    // IDNEAD
								INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_DESCENDENTE,       // IDNEAD
								INTERFACES_MODO_INTERFAZ_PULSADOR_SHORT_LONG,                     // IDNEAD
                INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO,               // IDNEAD
                INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL,                // IDNEAD
                INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_EXT,       // IDNEAD
                INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_EXT,        // IDNEAD
                INTERFACES_MODO_INTERFAZ_REG_EXT,			                            // IDNEAD
								INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_INT,       // IDNEAD
                INTERFACES_MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_INT,        // IDNEAD
                INTERFACES_MODO_INTERFAZ_REG_INT,			                            // IDNEAD
							
							
								/*
									#define MODO_INTERFAZ_NO_OPERATIVO													0x00	// 0	//0b000000/000
									#define MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_ASCENDENTE			0x08	// 1	//0b000001/000
									#define MODO_INTERFAZ_PULSADOR_SHORT_FLANCO_DESCENDENTE			0x10	// 2	//0b000010/000
									#define MODO_INTERFAZ_PULSADOR_SHORT_LONG										0x18	// 3	//0b000011/000
									#define MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO							0x20	// 4	//0b000100/000
									#define MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL							0x28	// 5	//0b000101/000
									#define MODO_INTERFAZ_DETECTOR_PRESENCIA_FLANCO_REG_LUZ			0x30	// 6	//0b000110/000
									#define MODO_INTERFAZ_DETECTOR_PRESENCIA_NIVEL_REG_LUZ			0x38	// 7	//0b000111/000
									#define MODO_INTERFAZ_REG_LUZ																0x40	// 8	//0b001000/000
								*/							
							
							
							
							
							
            }interfacesModosInterfaz;
        #endif
    #endif
    #ifdef INC_INTERFACES_TIPO_INTERFAZ_E
        #ifndef INC_INTERFACES_TIPO_INTERFAZ_E_H
        #define INC_INTERFACES_TIPO_INTERFAZ_E_H
            typedef enum interfacesTipoInterfaz_e
            {
                INTERFACES_TIPO_INTERFAZ_NO_OPERATIVO,
                INTERFACES_TIPO_INTERFAZ_IDNGEAD,
								INTERFACES_TIPO_INTERFAZ_IDNG4P,
            }interfacesTiposInterfaz;
        #endif
    #endif
    #ifdef  INC_INTERFACES_EVENTOS_TIPOS_E
        #ifndef  INC_INTERFACES_EVENTOS_TIPOS_E_H
        #define INC_INTERFACES_EVENTOS_TIPOS_E_H
            typedef enum interfacesEventosTipos_e
            {
                INTERFACES_EVENTO_NINGUNO,
                INTERFACES_EVENTO_SHORT_PRESS,
                INTERFACES_EVENTO_LONG_PRESS,
                INTERFACES_EVENTO_LONG_RELEASE,
                INTERFACES_EVENTO_PRESENCY,
                INTERFACES_EVENTO_VACANCY,
                INTERFACES_EVENTO_LIGHT_REPORT,
                INTERFACES_EVENTO_KEEP_ALIVE,
            } interfacesEventosTipos;
        #endif
    #endif
    #ifdef INC_INTERFACES_EVENTOS_S
        #ifndef INC_INTERFACES_EVENTOS_S_H
        #define INC_INTERFACES_EVENTOS_S_H
            #include "dataTypes.h"
            #define INC_INTERFACES_EVENTOS_TIPOS_E
                #include "interfaces.h"
            #undef INC_INTERFACES_EVENTOS_TIPOS_E

            typedef struct interfacesEventos_s
            {
                // En realidad se situaría en un nivel entre la interfaz y el sensor. Lo coloco en la interfaz por que en realidad sale de la interfaz y llegaría a todos los sensores.
                // También podría hacer una estructura con el evento y serían los parámetros del evento.

                interfacesEventosTipos  eventoTipo;
                BOOL posicionEncender;
                BOOL sentidoAscendente;
                unsigned char lectura;                  // Normalmente Nivel de Luz.
								unsigned char canal;

            } interfacesEventos;
        #endif
    #endif
    #ifdef INC_INTERFACES_CONFIGURACION_S
        #ifndef INC_INTERFACES_CONFIGURACION_S_H
        #define INC_INTERFACES_CONFIGURACION_S_H
            #include "dataTypes.h"

            #define INC_INTERFACES_TIPO_INTERFAZ_E
                #include "interfaces.h"
            #undef INC_INTERFACES_TIPO_INTERFAZ_E

            #define INC_INTERFACES_MODOS_INTERFAZ_E
                #include "interfaces.h"
            #undef INC_INTERFACES_MODOS_INTERFAZ_E

            #define INC_INTERFACES_NUMERO_SERIE_S
                #include "interfaces.h"
            #undef INC_INTERFACES_NUMERO_SERIE_S

            typedef struct interfacesConfiguracion_s
            {
                // Esta es la estructura para guardar en flash. Yo en la
                // estructura en RAM añadiría un flag para inhibido.
                BOOL                        instalado;
                BOOL                        desactivado;
								// Canal desactivado.
							
                interfacesTiposInterfaz     tipoInterfaz;
                interfacesModosInterfaz     modoInterfaz;				// EN OTROS PODRÍA SER UNO POR CANAL.
                interfacesNumeroSerie       numeroSerie;
            } interfacesConfiguracion;
            #define INTERFACES_CONFIGURACION_S_SIZE     13
        #endif
    #endif

    #ifdef INC_INTERFACES_PRT
        #ifndef INC_INTERFACES_PRT_H
        #define INC_INTERFACES_PRT_H

            #define INC_INTERFACES_CONFIGURACION_S
                #include "interfaces.h"
            #undef INC_INTERFACES_CONFIGURACION_S

            #define INC_INTERFACES_MODOS_INTERFAZ_E
                #include "interfaces.h"
            #undef INC_INTERFACES_MODOS_INTERFAZ_E

            #define INC_INTERFACES_EVENTOS_S
                #include "interfaces.h"
            #undef INC_INTERFACES_EVENTOS_S
						
						BOOL interfacesBuscar (unsigned char *ip);
            void interfacesDoTask(void);
            void interfacesSysTick (void);
            void interfacesRtcTick (void);
						unsigned char interfacesInstaladosCntGet(void);
            interfacesConfiguracion interfacesConfiguracionGet (unsigned char interfazAddress);
						BOOL interfacesConfiguracionSet (unsigned char interfazAddress, unsigned char *cfg, const unsigned char *interfacesCfgIp, unsigned char packetId_1, unsigned char packetId_2);
            void interfacesInit (void);
            void interfacesNotificar (unsigned char interfazAddress, unsigned char registroEstado, unsigned char lectura);            
						void interfacesStore (unsigned char interfazAddress, unsigned char registroEstado, unsigned char lectura);

        #endif
    #endif
