/****************************************************************************************************
*                                      PROGRAMA PRINCIPAL "MAIN"                                    *
*****************************************************************************************************/
/**
 ****************************************************************************************************
 * @file 		main.c
 * @date 		20-Mayo-2021
 ****************************************************************************************************
 *
 *
 ****************************************************************************************************
 */
 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
	

#include "main.h"
#include "board.h"
#include "tasks.h"

int main(void)
{
 	HAL_Init();	
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	boardInit();		
	sysTickDelay(100);		
	configInit();
	rtccInit();
	embeddedBootloaderInit();
	macInit();
	ethTransceiverInit();		
	historicoInit();
	testInit();
	tempInit();
	daliAccessHandlerAllowAllModules ();
	sensoresInit();	
	eBootloaderTransceiver_Check();	
	circInit();
  
  while (1){
		
		wdtDoTask();
		ethDoTask();	
		ethTransceiverDoTask();
		
		daliDoTask();		
	
		daliAddressingDoTask();		
		polling_DoTask();		
		botoneraDoTask();
		ethICMPDoTask();	
		fallosDoTask();
		tempDoTask();
		testDoTask();
		storageDoTask();
		daliUtilsDoTask();	
		sensoresDoTask ();
		sensorScanningDoTask ();
		sensorStoringDoTask ();
		iEDDoTask();
		daliLoopDoTask ();
		circDoTask();
		groupsDoTask();
	
  }
}



// MANEJADOR DE ERRORES:
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
