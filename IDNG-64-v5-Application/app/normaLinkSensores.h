#define INC_INTERFACES_PRT
    #include "interfaces.h"
#undef INC_INTERFACES_PRT

#define INC_SENSORES_PRT
    #include "sensores.h"
#undef INC_SENSORES_PRT

void normaLinkSensoresEnviar (interfacesNumeroSerie numeroSerie, interfacesEventos evento);
static void normaLinkSensoresEventoProcesar (interfacesNumeroSerie numeroSerie, interfacesEventos evento);
void normaLinkSensores(unsigned char comL);
void normaLinkSensoresBuscarInterfacesSend (unsigned char *ip, unsigned char p1, unsigned char p2, unsigned char p3, unsigned char p4);
