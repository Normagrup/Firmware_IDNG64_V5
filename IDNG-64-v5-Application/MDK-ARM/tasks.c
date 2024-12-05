/****************************************************************************************************
*                               TAREAS Y FUNCIONES PARA TEST DE MODULOS                             *
*****************************************************************************************************/
/**
 ****************************************************************************************************
 * @file 		tasks.c
 * @date 		01-Junio-2021
 ****************************************************************************************************
 *
 *
 ****************************************************************************************************
 */
#include "tasks.h"
#include "board.h"

unsigned char i2c_tx_buffer[8];
unsigned char i2c_rx_buffer[8];

unsigned char spi_tx_buffer[8];
unsigned char spi_rx_buffer[8];

unsigned char testBTN_count = 0;
unsigned char failBTN_count = 0;

/****************************************************************************************************
*                                    FUNCIONES MANEJO DEL M25P                                      *
*****************************************************************************************************/

void M25P_WriteEnable(void){
	uint8_t WREN_opcode = 0x06; 
	boardSPIM25PSSEnable();
	boardSPI2Write(WREN_opcode);
	boardSPIM25PSSDisable();
}

void M25P_WriteDisable(void){
	uint8_t WRDIS_opcode = 0x04; 
	boardSPIM25PSSEnable();
	boardSPI2Write(WRDIS_opcode);
	boardSPIM25PSSDisable();
}

void M25P_SectorErase(unsigned char addressH, unsigned char addressM, unsigned char addressL){
	unsigned char SE_opcode = 0xD8;
	boardSPIM25PSSEnable();
	boardSPI2Write(SE_opcode);
	boardSPI2Write(addressH);
	boardSPI2Write(addressM);
	boardSPI2Write(addressL);
	boardSPIM25PSSDisable();
}

void M25P_BulkErase(void){
	unsigned char BE_opcode = 0xC7;
	boardSPIM25PSSEnable();
	boardSPI2Write(BE_opcode);
	boardSPIM25PSSDisable();
}
	
void M25P_WriteBytes(unsigned char addressH, unsigned char addressM, unsigned char addressL, unsigned char *buffer, unsigned char sizeofbuffer){
	unsigned char WR_opcode = 0x02;
	boardSPIM25PSSEnable();
	boardSPI2Write(WR_opcode);
	boardSPI2Write(addressH);
	boardSPI2Write(addressM);
	boardSPI2Write(addressL);
	for(uint8_t i=0; i<sizeofbuffer; i++){
		boardSPI2Write(buffer[i]);
	}
	boardSPIM25PSSDisable();
}

void M25P_ReadBytes(unsigned char addressH, unsigned char addressM, unsigned char addressL, unsigned char *buffer, unsigned char sizeofbuffer){
	unsigned char RD_opcode = 0x03;
	boardSPIM25PSSEnable();
	boardSPI2Write(RD_opcode);
	boardSPI2Write(addressH);
	boardSPI2Write(addressM);
	boardSPI2Write(addressL);
	for(uint8_t i=0; i<sizeofbuffer; i++){
		buffer[i] = boardSPI2Read();
	}
	boardSPIM25PSSDisable();
}


/****************************************************************************************************
*                                       GESTION DE BOTONERAS                                        *
*****************************************************************************************************/
void task_GetButtons(void){
	if(boardTestBtnGet() == 1){
		if (testBTN_count == 0){
			fill_buffer(i2c_tx_buffer, sizeof(i2c_tx_buffer));
			boardI2CWrite(i2c_tx_buffer, PAGE2, sizeof(i2c_tx_buffer));
			boardLedVerdeON();
			testBTN_count = 1;
		}
		else{
			boardI2CRead(i2c_rx_buffer, PAGE2, sizeof(i2c_tx_buffer));
			boardLedVerdeOFF();
			testBTN_count = 0;
		}
	}
	
	if(boardResetBtnGet() == 1){
		if (failBTN_count == 0){
			fill_buffer(spi_tx_buffer, sizeof(spi_tx_buffer));
			M25P_WriteEnable();
			M25P_WriteBytes(0x01, 0x00, 0x00, spi_tx_buffer, sizeof(spi_tx_buffer));
			boardLedRojoON();
			failBTN_count = 1;
		}
		else{
			M25P_ReadBytes(0x01, 0x00, 0x00, spi_rx_buffer, sizeof(spi_rx_buffer));
			boardLedRojoOFF();
			failBTN_count = 0;
		}
	}
	while((boardResetBtnGet() != 0) || (boardTestBtnGet() != 0));
}
