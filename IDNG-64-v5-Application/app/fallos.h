
#define FALLOS_ESTADO_INDETERMINADO		0x00
#define FALLOS_ESTADO_NO_MARCADO			0x01
#define FALLOS_ESTADO_DETECTADO				0x02
#define FALLOS_ESTADO_NO_DETECTADO		0x04
#define FALLOS_ESTADO_MARCADO					0x05



void fallosFalloResetSet(void);
void fallosDoTask (void);
void fallosProcesar (unsigned char address, unsigned char deviceType, unsigned char failureOrStatusRegister);
void fallosComunicacion(unsigned char address, unsigned char direction);
