#define SPI1_WRITE_OP_CODE 								0xF0
#define SPI1_READ_OP_CODE 								0x0F


void spi1Write (unsigned char address, unsigned char data);
void spi1Send (unsigned char data);
unsigned char spi1Read (unsigned char address);
