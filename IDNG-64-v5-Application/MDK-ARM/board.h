/****************************************************************************************************
*                        CONFIGURACIONES DE A GPIO Y MODULOS -> STM32G070CB                         *
*****************************************************************************************************/
/**
 ****************************************************************************************************
 * @file 		board.h
 * @date 		20-Mayo-2021
 ****************************************************************************************************
 *
 *
 ****************************************************************************************************
 */
#include "stm32g0xx_hal.h"
#include "main.h"

//SPI DEVICES:
#define W5100		0							//Mascara para la comunicacion con el modulo ethernet [SPI2]		
#define M25P		1							//Mascara para la comunicacion con la memoria flash [SPI2]

//COMANDOS M25P40 (FLASH MEMORY) SPI2
#define WR_ENABLE			0x06		//Habilitacion de operaciones de escritura
#define WR_DISABLE		0x04		//Deshabilitacion de opreaciones de escritura
#define RD_ID					0x9F		//Lectura de identificacion
#define RD_STREG			0x05		//Lectura del registro STATUS
#define WR_STREG			0x01		//Escritura del registro STATUS
#define RD_DATAB			0x03		//Lectura de bytes de datos
#define RD_DATABHS		0x0B		//Lecturade bytes de datos amayor velocidad
#define PAGE_PROG			0x02		//Programacion de una pagina
#define SEC_ERASE			0xD8		//Borrado de un sector
#define BLK_ERASE			0xC7		//Borrado total de la memoria
#define DP_PWRDWN			0xB9		//Deep Power-Down
#define REL_DP_PWRDWN	0xAB		//Regreso del Deep Power-Down

//INFO I2C:
#define MEM_PAGESIZE 	0x08		//Tamaño de la página de la memoria EEPROM
#define MEM_MAX_WADDR 0x7F		//Ultima dirección de escritura de EEPROM
#define MEM_MAC_ADDR	0xFA		//Direccion de inicio del almacenamiento de la direccion MAC
#define MEM_MAC_SIZE	6
#define MEM_MAX_ADDR 	0xFF		//Ultima direccion de memoria
#define PAGE0	0*MEM_PAGESIZE	//Direccion de inicio de la pagina 0
#define PAGE1	1*MEM_PAGESIZE	//Direccion de inicio de la pagina 1
#define PAGE2 2*MEM_PAGESIZE	//Direccion de inicio de la pagina 2
#define PAGE3 3*MEM_PAGESIZE	//Direccion de inicio de la pagina 3
#define PAGE4 4*MEM_PAGESIZE	//Direccion de inicio de la pagina 4
#define PAGE5 5*MEM_PAGESIZE	//Direccion de inicio de la pagina 5


//LLENADO Y BORRADO DE BUFFERS SPI E I2C ************************************************************
#define RAND_BUFFER
void fill_buffer(uint8_t *buffer, uint8_t size);
void erase_buffer(uint8_t *buffer, uint8_t size);

//INICIALIZACION DE MODULOS Y GPIO ******************************************************************
void boardInit(void);

//MANEJO DE LA GPIO *********************************************************************************
void boardLedVerdeON(void);
void boardLedVerdeOFF(void);
void boardLedRojoON(void);
void boardLedRojoOFF(void);
unsigned char boardTestBtnGet(void);
unsigned char boardResetBtnGet(void);

//SERIAL PORT INTERFACE *****************************************************************************
void boardSPIW5100SSEnable(void);
void boardSPIW5100SSDisable(void);
void boardSPIM25PSSEnable(void);
void boardSPIM25PSSDisable(void);
void boardSPI1Write(unsigned char data);
uint8_t boardSPI1Read (void);
void boardSPI2Write(unsigned char data);
uint8_t boardSPI2Read (void);

//I2C ***********************************************************************************************
void boardI2CWrite(uint8_t *tx_buffer, uint8_t StartAddr, uint16_t SizeOfBuffer);
void boardI2CRead(uint8_t *rx_buffer, uint8_t StartAddr, uint16_t SizeOfBuffer);



void board_i2c_write_ip_address (unsigned char *ipAddress);
void board_i2c_write_submask_address (unsigned char *submaskAddress);
void board_i2c_write_bootloader_status(unsigned char value);
unsigned char board_i2c_read_bootloader_status (void);
