// DISTRIBUCIÓN DE BANCOS DE MEMORIA

















void m25pWriteEnable (void);
void m25pWriteDisable (void);
void m25pWriteBytes (unsigned char startAddrH, unsigned char startAddrM, unsigned char startAddrL, unsigned char *buffer, unsigned int size);
void m25pReadBytes (unsigned char startAddrH, unsigned char startAddrM, unsigned char startAddrL, unsigned char *buffer, unsigned int size);
unsigned char m25pStatusRead (void);
void m25pSectorErase (unsigned char startAddrH, unsigned char startAddrM, unsigned char startAddrL);
