/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PTEST_MCU_Pin GPIO_PIN_13
#define PTEST_MCU_GPIO_Port GPIOC
#define OSC_EXT_IN_Pin GPIO_PIN_14
#define OSC_EXT_IN_GPIO_Port GPIOC
#define OSC_EXT_OUT_Pin GPIO_PIN_15
#define OSC_EXT_OUT_GPIO_Port GPIOC
#define STM_BLD2_Pin GPIO_PIN_0
#define STM_BLD2_GPIO_Port GPIOA
#define STM_BLD_Pin GPIO_PIN_1
#define STM_BLD_GPIO_Port GPIOA
#define AVR_BOOT_Pin GPIO_PIN_2
#define AVR_BOOT_GPIO_Port GPIOA
#define AVR_INT_Pin GPIO_PIN_3
#define AVR_INT_GPIO_Port GPIOA
#define SPI1_DALI_SS_Pin GPIO_PIN_4
#define SPI1_DALI_SS_GPIO_Port GPIOA
#define RESET_BTN_Pin GPIO_PIN_0
#define RESET_BTN_GPIO_Port GPIOB
#define LINK_LED_Pin GPIO_PIN_1
#define LINK_LED_GPIO_Port GPIOB
#define SPI2_M25P_SS_Pin GPIO_PIN_11
#define SPI2_M25P_SS_GPIO_Port GPIOB
#define SPI2_W5100_SS_Pin GPIO_PIN_12
#define SPI2_W5100_SS_GPIO_Port GPIOB
#define W811_INT_Pin GPIO_PIN_6
#define W811_INT_GPIO_Port GPIOC
#define W811_RESET_Pin GPIO_PIN_7
#define W811_RESET_GPIO_Port GPIOC
#define EXT_STM_GPIO_Pin GPIO_PIN_2
#define EXT_STM_GPIO_GPIO_Port GPIOD
#define SYS_SWO_Pin GPIO_PIN_3
#define SYS_SWO_GPIO_Port GPIOB
#define FAIL_LED_Pin GPIO_PIN_8
#define FAIL_LED_GPIO_Port GPIOB
#define TEST_BTN_Pin GPIO_PIN_9
#define TEST_BTN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
