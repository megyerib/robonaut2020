/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

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
#define TOF_CS2_Pin GPIO_PIN_13
#define TOF_CS2_GPIO_Port GPIOC
#define TOF_CS3_Pin GPIO_PIN_14
#define TOF_CS3_GPIO_Port GPIOC
#define TOF_CS1_Pin GPIO_PIN_15
#define TOF_CS1_GPIO_Port GPIOC
#define SRV_EN_Pin GPIO_PIN_0
#define SRV_EN_GPIO_Port GPIOC
#define INERT_INT2_Pin GPIO_PIN_3
#define INERT_INT2_GPIO_Port GPIOC
#define SERVO_SENSOR_Pin GPIO_PIN_5
#define SERVO_SENSOR_GPIO_Port GPIOA
#define REM_CH1_Pin GPIO_PIN_6
#define REM_CH1_GPIO_Port GPIOA
#define REM_CH2_Pin GPIO_PIN_7
#define REM_CH2_GPIO_Port GPIOA
#define REM_CH3_Pin GPIO_PIN_0
#define REM_CH3_GPIO_Port GPIOB
#define REM_CH4_Pin GPIO_PIN_1
#define REM_CH4_GPIO_Port GPIOB
#define SERVO_FRONT_Pin GPIO_PIN_14
#define SERVO_FRONT_GPIO_Port GPIOB
#define SERVO_REAR_Pin GPIO_PIN_15
#define SERVO_REAR_GPIO_Port GPIOB
#define INERT_SDA_Pin GPIO_PIN_9
#define INERT_SDA_GPIO_Port GPIOC
#define INERT_SCL_Pin GPIO_PIN_8
#define INERT_SCL_GPIO_Port GPIOA
#define INERT_INT1_Pin GPIO_PIN_15
#define INERT_INT1_GPIO_Port GPIOA
#define FREE3_LED_Pin GPIO_PIN_3
#define FREE3_LED_GPIO_Port GPIOB
#define FREE2_Pin GPIO_PIN_4
#define FREE2_GPIO_Port GPIOB
#define FREE1_Pin GPIO_PIN_5
#define FREE1_GPIO_Port GPIOB
#define TOF_SCL_Pin GPIO_PIN_6
#define TOF_SCL_GPIO_Port GPIOB
#define TOF_SDA_Pin GPIO_PIN_7
#define TOF_SDA_GPIO_Port GPIOB
#define ENC_A_Pin GPIO_PIN_8
#define ENC_A_GPIO_Port GPIOB
#define ENC_B_Pin GPIO_PIN_9
#define ENC_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
