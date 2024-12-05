#include "spi1.h"

void spi1Write (unsigned char address, unsigned char data)
{
	boardAVR_SPI_CS_Enable();
	boardSPI1Write(SPI1_WRITE_OP_CODE);		
	boardSPI1Write(address);			
	boardSPI1Write(data);			
	boardAVR_SPI_CS_Disable();	
}

unsigned char spi1Read(unsigned char address)
{
    unsigned char answer;
		volatile unsigned short int n;
		boardAVR_SPI_CS_Enable();
		boardSPI1Write(SPI1_READ_OP_CODE);		
		boardSPI1Write(address);					
		for (n=0;n<200;n++)	// 60uS Delay
		{
		}
		answer=boardSPI1Read();
		boardAVR_SPI_CS_Disable();	
    return answer;		
}