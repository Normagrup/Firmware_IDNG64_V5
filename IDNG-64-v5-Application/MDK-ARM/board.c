/****************************************************************************************************
*                                  INTERFAZ PARA MCU -> STM32G070CB                                 *
*****************************************************************************************************/
/**
 ****************************************************************************************************
 * @file 		board.c
 * @date 		20-Mayo-2021
 ****************************************************************************************************
 *
 *
 ****************************************************************************************************
 */
#include "board.h"
#include "tasks.h"
#include "maincfg.h"

#ifndef TIME_S
	typedef struct time_s
	{
			unsigned char hours;
			unsigned char minutes;
			unsigned char seconds;
	} Time;
#define	TIME_S
#endif

#ifndef DATE_S
	typedef struct date_s
	{
		unsigned char day;
		unsigned char weekday;
		unsigned char month;
		unsigned char year;
	} Date;
#define	DATE_S
#endif


//Variables privadas
I2C_HandleTypeDef hi2c1;
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
//UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;
IWDG_HandleTypeDef hiwdg;

/****************************************************************************************************
*                              LLENADO Y BORRADO DE BUFFERS SPI E I2C                               *
*****************************************************************************************************/
void fill_buffer(uint8_t *buffer, uint8_t size){
	for(uint8_t i=0; i<size; i++){
		#ifdef RAND_BUFFER
			buffer[i] = rand()%255;
		#else
			buffer[i] = i;
		#endif
	}
}
void erase_buffer(uint8_t *buffer, uint8_t size){
	for(uint8_t i=0; i<size; i++){
		buffer[i] = 0;
	}
}

/****************************************************************************************************
*                           INICIALIZACION DE LOS MODULOS Y GPIO DEL MCU                            *
*****************************************************************************************************/

void boardRemapSRAM (void)
{
	__HAL_SYSCFG_REMAPMEMORY_SRAM();
}


void boardResetSYSCFG (void)
{
__HAL_RCC_SYSCFG_FORCE_RESET();
}

void boardWdtDoTask(void)
{
	#ifndef NO_WDT  
	HAL_IWDG_Refresh(&hiwdg);
	#endif
}

void boardWdtInit (void)
{
	#ifndef NO_WDT  
	/* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */
	//#else
	#endif
}



void boardInit(void)
{	
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_MEDIUMHIGH);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
	//INICIALIZACION DEL I2C **************************************************************************
	hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10707DBC;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	
  if (HAL_I2C_Init(&hi2c1) != HAL_OK){
    Error_Handler();
  }
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK){
    Error_Handler();
  }
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK){
    Error_Handler();
  }
	
	//INICIALIZACION DEl SPI1 [ATMEGA -> DALI] ********************************************************
	hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;	
	//hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; //¡MODIFICADO!
	//hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; //¡MODIFICADO!
	//hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64; //¡MODIFICADO!
	//hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; //¡MODIFICADO!	
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; //¡MODIFICADO!
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK){
    Error_Handler();
  }
	
	//INICIALIZACION DEL SPI2 [Ethernet(W811) / Memoria Flash(M25P40)] ********************************
	hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK){
    Error_Handler();
  }
	
	//INICIALIZACION DE LA UART ***********************************************************************
	/*
	huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK){
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK){
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK){
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK){
    Error_Handler();
  }*/
	
	//INICIALIZACION DE LA GPIO ***********************************************************************
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, PTEST_MCU_Pin|W811_RESET_Pin, GPIO_PIN_RESET);
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, AVR_BOOT_Pin|SPI1_DALI_SS_Pin, GPIO_PIN_RESET);
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LINK_LED_Pin|SPI2_M25P_SS_Pin|SPI2_W5100_SS_Pin|FAIL_LED_Pin, GPIO_PIN_RESET);
  /*Configure GPIO pin : PTEST_MCU_Pin */
  GPIO_InitStruct.Pin = PTEST_MCU_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PTEST_MCU_GPIO_Port, &GPIO_InitStruct);
  /*Configure GPIO pins : STM_BLD2_Pin STM_BLD_Pin AVR_INT_Pin */
  GPIO_InitStruct.Pin = STM_BLD2_Pin|STM_BLD_Pin|AVR_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*Configure GPIO pin : AVR_BOOT_Pin */
  GPIO_InitStruct.Pin = AVR_BOOT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(AVR_BOOT_GPIO_Port, &GPIO_InitStruct);
  /*Configure GPIO pin : SPI1_DALI_SS_Pin */
  GPIO_InitStruct.Pin = SPI1_DALI_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI1_DALI_SS_GPIO_Port, &GPIO_InitStruct);
  /*Configure GPIO pins : RESET_BTN_Pin SYS_SWO_Pin TEST_BTN_Pin */
  GPIO_InitStruct.Pin = RESET_BTN_Pin|SYS_SWO_Pin|TEST_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*Configure GPIO pins : LINK_LED_Pin SPI2_M25P_SS_Pin SPI2_W5100_SS_Pin FAIL_LED_Pin */
  GPIO_InitStruct.Pin = LINK_LED_Pin|SPI2_M25P_SS_Pin|SPI2_W5100_SS_Pin|FAIL_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*Configure GPIO pin : W811_INT_Pin */
  GPIO_InitStruct.Pin = W811_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(W811_INT_GPIO_Port, &GPIO_InitStruct);
  /*Configure GPIO pin : W811_RESET_Pin */
  GPIO_InitStruct.Pin = W811_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(W811_RESET_GPIO_Port, &GPIO_InitStruct);
  /*Configure GPIO pin : EXT_STM_GPIO_Pin */
  GPIO_InitStruct.Pin = EXT_STM_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EXT_STM_GPIO_GPIO_Port, &GPIO_InitStruct);
	
	/*Inicialización de los pines del SPI*/
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //SPI1 -> SCK
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); //SPI1 -> MOSI
	HAL_GPIO_WritePin(SPI1_DALI_SS_GPIO_Port, SPI1_DALI_SS_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); //SPI2 -> SCK
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); //SPI2 -> MOSI
	HAL_GPIO_WritePin(SPI2_M25P_SS_GPIO_Port, SPI2_M25P_SS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI2_W5100_SS_GPIO_Port, SPI2_W5100_SS_Pin, GPIO_PIN_SET);
	
	



	

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.SubSeconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  //if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  //{
  //  Error_Handler();
  //}
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  //if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  //{
  //  Error_Handler();
  //}
  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_ALL;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */	
	
	boardWdtInit();
	
}




void boardSystick (void)
{
	
	sysTickTick();
}


void boardRTCTick (Date date, Time time)
{		
	rtccTick(date, time);	
}

void boardDateSet (Date date)
{
		RTC_DateTypeDef sDate = {0};
		sDate.Month=date.month;
		sDate.Date=date.day;
		sDate.WeekDay=date.weekday;
		sDate.Year=date.year;
		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}	
}

void boardTimeSet (Time time)
{
		RTC_TimeTypeDef sTime = {0};				
		sTime.Hours = time.hours;
		sTime.Minutes = time.minutes;
		sTime.Seconds = time.seconds;
		sTime.SubSeconds = 0;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
		{
			Error_Handler();
		}
}



/****************************************************************************************************
*                                   FUNCIONES MANEJO DE LA GPIO                                     *
*****************************************************************************************************/
/*LED VERDE / LINK LED*/
void boardLedVerdeON(void){
	HAL_GPIO_WritePin(LINK_LED_GPIO_Port, LINK_LED_Pin, GPIO_PIN_SET);
}
void boardLedVerdeOFF(void){
	HAL_GPIO_WritePin(LINK_LED_GPIO_Port, LINK_LED_Pin, GPIO_PIN_RESET);
}
/*LED ROJO / FAIL LED*/
void boardLedRojoON(void){
	HAL_GPIO_WritePin(FAIL_LED_GPIO_Port, FAIL_LED_Pin, GPIO_PIN_SET);
}
void boardLedRojoOFF(void){
	HAL_GPIO_WritePin(FAIL_LED_GPIO_Port, FAIL_LED_Pin, GPIO_PIN_RESET);
}

void boardAVRBootON (void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);	
}

void boardAVRBootOFF (void)
{	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);	
}

void boardW5100ResetON (void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);			//Reset ON	
}

void boardW5100ResetOFF (void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);				//Reset OFF
}


// DALI ISR
unsigned char boardDALIISRGet(void)
{
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 1) return 1;
	else return 0;
}


/*BOTON TEST*/
unsigned char boardTestBtnGet(void){
	if(HAL_GPIO_ReadPin(TEST_BTN_GPIO_Port, TEST_BTN_Pin) == 0) return 1;
	else return 0;
}
/*BOTON RESET*/
unsigned char boardResetBtnGet(void){
	if(HAL_GPIO_ReadPin(RESET_BTN_GPIO_Port, RESET_BTN_Pin) == 0) return 1;
	else return 0;
}

void boardAVR_SPI_CS_Enable(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void boardAVR_SPI_CS_Disable(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}


/****************************************************************************************************
*                                    FUNCIONES MANEJO DEL SPI                                       *
*****************************************************************************************************/
//Activacion y desactivacion de los Slave Selectors
void boardSPIW5100SSEnable(void){
	HAL_GPIO_WritePin(SPI2_W5100_SS_GPIO_Port, SPI2_W5100_SS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPI2_M25P_SS_GPIO_Port, SPI2_M25P_SS_Pin, GPIO_PIN_SET);
}

void boardSPIW5100SSDisable(void){
	HAL_GPIO_WritePin(SPI2_W5100_SS_GPIO_Port, SPI2_W5100_SS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI2_M25P_SS_GPIO_Port, SPI2_M25P_SS_Pin, GPIO_PIN_SET);
}

void boardSPIM25PSSEnable(void){
	HAL_GPIO_WritePin(SPI2_M25P_SS_GPIO_Port, SPI2_M25P_SS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPI2_W5100_SS_GPIO_Port, SPI2_W5100_SS_Pin, GPIO_PIN_SET);
}

void boardSPIM25PSSDisable(void){
	HAL_GPIO_WritePin(SPI2_W5100_SS_GPIO_Port, SPI2_W5100_SS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI2_M25P_SS_GPIO_Port, SPI2_M25P_SS_Pin, GPIO_PIN_SET);
}

void boardSPI1Write(unsigned char data){
	HAL_SPI_Transmit(&hspi1, &data, 1, 100);
}

//Lectura y escritura del SPI1
uint8_t boardSPI1Read (void){
	uint8_t rx_byte;
	uint8_t tx_byte=0;
	//HAL_SPI_Receive(&hspi1, &rx_byte, 1, 100);
	HAL_SPI_TransmitReceive(&hspi1,&tx_byte,&rx_byte,1,100);
	return rx_byte;
}

//Lectura y escritura del SPI2
void boardSPI2Write(unsigned char data){
	HAL_SPI_Transmit(&hspi2, &data, 1, 100);
}

uint8_t boardSPI2Read (void){
	uint8_t rx_byte;
	uint8_t tx_byte=0;
	HAL_SPI_TransmitReceive(&hspi2,&tx_byte,&rx_byte,1,100);
	//HAL_SPI_Receive(&hspi1, &rx_byte, 1, 100);
	return rx_byte;
}


void board_i2c_write_ip_address (unsigned char *ipAddress)
{
	boardI2CWrite(&ipAddress[0], 0x14, 0x04);
sysTickDelay(20);	
}

void board_i2c_write_submask_address (unsigned char *submaskAddress)
{
	boardI2CWrite(&submaskAddress[0], 0x10, 0x04);
sysTickDelay(20);	
}

void board_i2c_write_bootloader_status(unsigned char value)
{
	boardI2CWrite(&value, 0x00, 0x01);
sysTickDelay(20);	
}

unsigned char board_i2c_read_bootloader_status (void)
{
	unsigned char answer;		
	boardI2CRead(&answer, 0x00, 1);	
	return answer;
}



/****************************************************************************************************
*                                    FUNCIONES MANEJO DEL I2C                                       *
*****************************************************************************************************/
/**	@note		Escritura en I2C1
  * @param  *tx_buffer -> Puntero al buffer que se desea enviar [tamaño variable].
	* @param	StartAddr -> Direccion del comienzo  de escritura.
	*							- INICIO[0x00] - FINAL[0x7F].
	*							- Memoria dividida en paginas de 8 bytes.
	*							- Maximo tamaño de escritura 8 bytes.
  * @param  SizeOfBuffer -> Especificar tamaño del buffer a escribir.
  * @retval None.
  */
void boardI2CWrite(uint8_t *tx_buffer, uint8_t StartAddr, uint16_t SizeOfBuffer){
	if((StartAddr+SizeOfBuffer) > MEM_MAX_WADDR) return;
	uint8_t controlbyte = 0xA0;	// CONTROL BYTE [EEPROM] = 1010 XXX(R/W) [WRITE = 0]
	HAL_I2C_Mem_Write(&hi2c1, controlbyte, StartAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)tx_buffer, SizeOfBuffer, 100);
}

/**	@note		Lectura en I2C1
  * @param  *tx_buffer -> Puntero al buffer en el que se desean recibir los datos [tamaño variable].
	* @param	StartAddr -> Direccion del comienzo  de escritura.
	*							- INICIO[0x00] - FINAL[0xFF].
	*							- MAC ADDRESS -> 0xFA-0xFF.
	*							- Memoria dividida en paginas de 8 bytes.
	*							- Maximo tamaño de lectura SizeOfBuffer.
  * @param  SizeOfBuffer -> Especificar tamaño del buffer a escribir.
  * @retval None.
  */
void boardI2CRead(uint8_t *rx_buffer, uint8_t StartAddr, uint16_t SizeOfBuffer){
	uint8_t controlbyte = 0xA1;	// CONTROL BYTE [EEPROM] = 1010 XXX(R/W) [READ = 1]
	HAL_I2C_Mem_Read(&hi2c1, controlbyte, StartAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&rx_buffer[0], SizeOfBuffer, 100);
}
