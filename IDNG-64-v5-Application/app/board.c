#include "board.h"



void board_AvrBootClear(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2); 	
}

void board_AvrBootSet(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2); 	
}
		
		
void board_LedRojoOn(void)
{	
	GPIO_SetBits(GPIOC, GPIO_Pin_1); 
}

void board_LedRojoOff(void)
{	
	GPIO_ResetBits(GPIOC, GPIO_Pin_1); 
}

void board_LedVerdeOn(void)
{	
	GPIO_SetBits(GPIOC, GPIO_Pin_2); 
}

void board_LedVerdeOff(void)
{	
	GPIO_ResetBits(GPIOC, GPIO_Pin_2); 
}

				



void boardInit (void)
{
	SPI_InitTypeDef spi2con;
	GPIO_InitTypeDef spi2;
	GPIO_InitTypeDef spi2nss;
	GPIO_InitTypeDef spi2nssm;
	GPIO_InitTypeDef	w811en;
	GPIO_InitTypeDef	w811rst;
	SPI_InitTypeDef spicon;
	GPIO_InitTypeDef demo;
	GPIO_InitTypeDef demo2;
	GPIO_InitTypeDef avrInt;
	GPIO_InitTypeDef i2c;
	GPIO_InitTypeDef botonTest;
	GPIO_InitTypeDef botonRst;
	GPIO_InitTypeDef ledLink;
	GPIO_InitTypeDef ledFail;
	
	GPIO_InitTypeDef i1;
	GPIO_InitTypeDef i2;
	GPIO_InitTypeDef avrBoot;
	
	
	
	
	
	I2C_InitTypeDef I2C_InitStructure;		
	RCC_DeInit();		
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	// INICIALIZACIÓN DEL SPI2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	spi2.GPIO_Mode = GPIO_Mode_AF;
	spi2.GPIO_Speed = GPIO_Speed_Level_3;
	spi2.GPIO_OType = GPIO_OType_PP;
	spi2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	spi2.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB,&spi2);	
	spi2nss.GPIO_Mode = GPIO_Mode_OUT;
	spi2nss.GPIO_Speed = GPIO_Speed_Level_3;
	spi2nss.GPIO_OType = GPIO_OType_PP;
	spi2nss.GPIO_PuPd = GPIO_PuPd_NOPULL;
	spi2nss.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOB,&spi2nss);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);	
	spi2nssm.GPIO_Mode = GPIO_Mode_OUT;
	spi2nssm.GPIO_Speed = GPIO_Speed_Level_3;
	spi2nssm.GPIO_OType = GPIO_OType_PP;
	spi2nssm.GPIO_PuPd = GPIO_PuPd_NOPULL;
	spi2nssm.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&spi2nssm);	
	GPIO_SetBits(GPIOB, GPIO_Pin_11);	
	avrInt.GPIO_Mode = GPIO_Mode_IN;
	avrInt.GPIO_PuPd = GPIO_PuPd_NOPULL;
	avrInt.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA,&avrInt);
	SPI_I2S_DeInit(SPI2);
	spi2con.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // Initially Tx
	spi2con.SPI_Mode	= SPI_Mode_Master;
	spi2con.SPI_DataSize	= SPI_DataSize_8b;
	spi2con.SPI_CPOL	= SPI_CPOL_Low ;
	spi2con.SPI_CPHA	= SPI_CPHA_1Edge;
	spi2con.SPI_NSS	= SPI_NSS_Soft ;		
	//spi2con.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_16;		
	//spi2con.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_8;		
	spi2con.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_2;		
	spi2con.SPI_FirstBit	= SPI_FirstBit_MSB;
	spi2con.SPI_CRCPolynomial	= SPI_CRCLength_8b;
	SPI_Init(SPI2, &spi2con);
	SPI_RxFIFOThresholdConfig (SPI2, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(SPI2, ENABLE); 			
	// INICIALIZACIÓN DEL SPI UTILIZADO PARA LA COMUNICACIÓN DALI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);			
	botonTest.GPIO_Mode = GPIO_Mode_IN;
	botonTest.GPIO_PuPd = GPIO_PuPd_NOPULL;
	botonTest.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &botonTest);	
	botonRst.GPIO_Mode = GPIO_Mode_IN;
	botonRst.GPIO_PuPd = GPIO_PuPd_NOPULL;
	botonRst.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOC, &botonRst);	
	
	ledLink.GPIO_Mode = GPIO_Mode_OUT;
	ledLink.GPIO_OType = GPIO_OType_PP;
	ledLink.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC, &ledLink);	
	
	ledFail.GPIO_Mode = GPIO_Mode_OUT;
	ledFail.GPIO_OType = GPIO_OType_PP;
	ledFail.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &ledFail);	
	
	
	
	// GPIO settings
	demo.GPIO_Mode = GPIO_Mode_AF;
	demo.GPIO_Speed = GPIO_Speed_Level_3;
	demo.GPIO_OType = GPIO_OType_PP;
	demo.GPIO_PuPd = GPIO_PuPd_NOPULL;
	demo.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //SPI 1 ports enabled yet spi not initialized
	GPIO_Init(GPIOA, &demo);
	demo2.GPIO_Mode = GPIO_Mode_OUT;
	demo2.GPIO_Speed = GPIO_Speed_Level_3;
	demo2.GPIO_OType = GPIO_OType_PP;
	demo2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	demo2.GPIO_Pin = GPIO_Pin_4; //SPI 1 ports enabled yet spi not initialized
	GPIO_Init(GPIOA, &demo2);
	//GPIO_PinAFConfig(GPIOA,GPIO_PinSource5 |GPIO_PinSource6 |GPIO_PinSource7|GPIO_PinSource4 , GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5 |GPIO_PinSource6 |GPIO_PinSource7 , GPIO_AF_0);
	
	w811rst.GPIO_Pin = GPIO_Pin_7;
	w811rst.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOC, &w811rst);	
	GPIO_ResetBits(GPIOC, GPIO_Pin_7); 
	
	//w811en.GPIO_Pin = GPIO_Pin_7;
	//w811en.GPIO_Mode = GPIO_Mode_OUT;
	//GPIO_Init(GPIOC, &w811en);	
	//GPIO_ResetBits(GPIOC, GPIO_Pin_7); 
	SPI_I2S_DeInit(SPI1);
	spicon.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // Initially Tx
	spicon.SPI_Mode	= SPI_Mode_Master;
	spicon.SPI_DataSize	= SPI_DataSize_8b;
	spicon.SPI_CPOL	= SPI_CPOL_Low ;
	spicon.SPI_CPHA	= SPI_CPHA_1Edge;
	spicon.SPI_NSS	= SPI_NSS_Soft ;
	spicon.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_16;
	spicon.SPI_FirstBit	= SPI_FirstBit_MSB;
	spicon.SPI_CRCPolynomial	= SPI_CRCLength_8b;
	SPI_Init(SPI1, &spicon);
	SPI_RxFIFOThresholdConfig (SPI1, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(SPI1, ENABLE); 
	GPIO_SetBits(GPIOA, GPIO_Pin_4); // CS high					
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);		
	
	
	RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_1);	
	
	
	i1.GPIO_Mode = GPIO_Mode_IN;
	i1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	i1.GPIO_Pin = GPIO_Pin_14;
	//GPIO_Init(GPIOA, &i1);	
	
	i2.GPIO_Mode = GPIO_Mode_IN;
	i2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	i2.GPIO_Pin = GPIO_Pin_15;
	//GPIO_Init(GPIOA, &i2);	
	
	
	avrBoot.GPIO_Mode = GPIO_Mode_OUT;
	avrBoot.GPIO_PuPd = GPIO_PuPd_UP;
	avrBoot.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &avrBoot);	
	
	
	board_AvrBootSet();
	
	
	
	// GPIO settings
	i2c.GPIO_Pin = GPIO_Pin_6;
	i2c.GPIO_Mode = GPIO_Mode_AF;
	i2c.GPIO_OType = GPIO_OType_OD;
	i2c.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB, &i2c);		
	i2c.GPIO_Pin = GPIO_Pin_7;
	i2c.GPIO_Mode = GPIO_Mode_AF;
	i2c.GPIO_OType = GPIO_OType_OD;
	i2c.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB, &i2c);			
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 0x00;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00; 
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_Timing = 0xA0120227;
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);			
	if (SysTick_Config(SystemCoreClock / 1000))
  { 
    while (1);
  }
	
}
