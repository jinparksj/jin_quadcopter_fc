/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f4xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t uart6_rx_flag = 0;
uint8_t uart6_rx_data = 0;

uint8_t uart5_rx_flag = 0;
uint8_t uart5_rx_data = 0;

uint8_t uart4_rx_flag = 0;
uint8_t uart4_rx_data = 0;

uint8_t m8n_rx_buf[36];
uint8_t m8n_rx_cplt_flag = 0;

uint8_t ibus_rx_buf[32];
uint8_t ibus_rx_cplt_flag = 0;

uint8_t uart1_rx_data = 0;

uint8_t tim7_1ms_flag = 0;
uint8_t tim7_20ms_flag = 0;
uint8_t tim7_100ms_flag = 0;
uint8_t tim7_1000ms_flag = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
	/* USER CODE BEGIN NonMaskableInt_IRQn 0 */

	/* USER CODE END NonMaskableInt_IRQn 0 */
	/* USER CODE BEGIN NonMaskableInt_IRQn 1 */

	/* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
	/* USER CODE BEGIN HardFault_IRQn 0 */

	/* USER CODE END HardFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_HardFault_IRQn 0 */
		/* USER CODE END W1_HardFault_IRQn 0 */
	}
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
	/* USER CODE BEGIN MemoryManagement_IRQn 0 */

	/* USER CODE END MemoryManagement_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
		/* USER CODE END W1_MemoryManagement_IRQn 0 */
	}
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
	/* USER CODE BEGIN BusFault_IRQn 0 */

	/* USER CODE END BusFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_BusFault_IRQn 0 */
		/* USER CODE END W1_BusFault_IRQn 0 */
	}
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
	/* USER CODE BEGIN UsageFault_IRQn 0 */

	/* USER CODE END UsageFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_UsageFault_IRQn 0 */
		/* USER CODE END W1_UsageFault_IRQn 0 */
	}
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
	/* USER CODE BEGIN SVCall_IRQn 0 */

	/* USER CODE END SVCall_IRQn 0 */
	/* USER CODE BEGIN SVCall_IRQn 1 */

	/* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
	/* USER CODE BEGIN DebugMonitor_IRQn 0 */

	/* USER CODE END DebugMonitor_IRQn 0 */
	/* USER CODE BEGIN DebugMonitor_IRQn 1 */

	/* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
	/* USER CODE BEGIN PendSV_IRQn 0 */

	/* USER CODE END PendSV_IRQn 0 */
	/* USER CODE BEGIN PendSV_IRQn 1 */

	/* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
	/* USER CODE BEGIN SysTick_IRQn 0 */

	/* USER CODE END SysTick_IRQn 0 */
	HAL_IncTick();
	/* USER CODE BEGIN SysTick_IRQn 1 */

	/* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void)
{
	/* USER CODE BEGIN USART1_IRQn 0 */

	/* USER CODE END USART1_IRQn 0 */
	HAL_UART_IRQHandler(&huart1);
	/* USER CODE BEGIN USART1_IRQn 1 */

	/* USER CODE END USART1_IRQn 1 */
}

/**
 * @brief This function handles UART4 global interrupt.
 */
void UART4_IRQHandler(void)
{
	/* USER CODE BEGIN UART4_IRQn 0 */
	static unsigned char cnt = 0;

	if (LL_USART_IsActiveFlag_RXNE(UART4)) {
		LL_USART_ClearFlag_RXNE(UART4);
		uart4_rx_data = LL_USART_ReceiveData8(UART4);
		uart4_rx_flag = 1;

		//DEBUG code
		//		LL_USART_TransmitData8(USART6, uart4_rx_data);

		//		cnt++;
		//		if (cnt == 35) {
		//			m8n_rx_cplt_flag = 1;
		//			cnt = 0;
		//		}

		switch (cnt) {
		case 0:
			if (uart4_rx_data == 0xb5) {
				m8n_rx_buf[cnt] = uart4_rx_data;
				cnt++;
			}
			break;
		case 1:
			if (uart4_rx_data == 0x62) {
				m8n_rx_buf[cnt] = uart4_rx_data;
				cnt++;
			} else {
				cnt = 0;
			}
			break;
		case 35:
			m8n_rx_buf[cnt] = uart4_rx_data;
			cnt = 0;
			m8n_rx_cplt_flag = 1;
			break;
		default:
			m8n_rx_buf[cnt] = uart4_rx_data;
			cnt++;
			break;
		}
	}

	/* USER CODE END UART4_IRQn 0 */
	/* USER CODE BEGIN UART4_IRQn 1 */

	/* USER CODE END UART4_IRQn 1 */
}

/**
 * @brief This function handles UART5 global interrupt.
 */
void UART5_IRQHandler(void)
{
	/* USER CODE BEGIN UART5_IRQn 0 */
	static unsigned char cnt = 0;

	if (LL_USART_IsActiveFlag_RXNE(UART5)) {
		LL_USART_ClearFlag_RXNE(UART5);
		uart5_rx_data = LL_USART_ReceiveData8(UART5);
		uart5_rx_flag = 1;

		switch (cnt) {
		case 0:
			if (uart5_rx_data == 0x20) {
				ibus_rx_buf[cnt] = uart5_rx_data;
				cnt++;
			}
			break;
		case 1:
			if (uart5_rx_data == 0x40) {
				ibus_rx_buf[cnt] = uart5_rx_data;
				cnt++;
			} else {
				cnt = 0;
			}
			break;
		case 31:
			ibus_rx_buf[cnt] = uart5_rx_data;
			cnt = 0;
			ibus_rx_cplt_flag = 1;
			break;
		default:
			ibus_rx_buf[cnt] = uart5_rx_data;
			cnt++;
			break;
		}

		//		while(!LL_USART_IsActiveFlag_TXE(USART6));
		//		LL_USART_TransmitData8(USART6, uart5_rx_data);
	}
	/* USER CODE END UART5_IRQn 0 */
	/* USER CODE BEGIN UART5_IRQn 1 */

	/* USER CODE END UART5_IRQn 1 */
}

/**
 * @brief This function handles TIM7 global interrupt.
 */
void TIM7_IRQHandler(void)
{
	/* USER CODE BEGIN TIM7_IRQn 0 */
	//1ms period
	static unsigned char tim7_1ms_count = 0;
	static unsigned char tim7_20ms_count = 0;
	static unsigned char tim7_100ms_count = 0;
	static unsigned short tim7_1000ms_count = 0;

	if(LL_TIM_IsActiveFlag_UPDATE(TIM7)) {
		LL_TIM_ClearFlag_UPDATE(TIM7); //Clear flag of TM7

		//1ms - 1000Hz Transmission
		tim7_1ms_count++;
		if (tim7_1ms_count == 1) {
			tim7_1ms_count = 0;
			tim7_1ms_flag = 1;
		}
		//20ms - 50Hz Transmission
		tim7_20ms_count++;
		if (tim7_20ms_count == 20) {
			tim7_20ms_count = 0;
			tim7_20ms_flag = 1;
		}
		//100ms - 10Hz Transmission
		tim7_100ms_count++;
		if (tim7_100ms_count == 100) {
			tim7_100ms_count = 0;
			tim7_100ms_flag = 1;
		}

		//1000ms - 1Hz Transmission
		tim7_1000ms_count++;
		if (tim7_1000ms_count == 1000) {
			tim7_1000ms_count = 0;
			tim7_1000ms_flag = 1;
		}

	}

	/* USER CODE END TIM7_IRQn 0 */
	/* USER CODE BEGIN TIM7_IRQn 1 */

	/* USER CODE END TIM7_IRQn 1 */
}

/**
 * @brief This function handles DMA2 stream0 global interrupt.
 */
void DMA2_Stream0_IRQHandler(void)
{
	/* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

	/* USER CODE END DMA2_Stream0_IRQn 0 */
	HAL_DMA_IRQHandler(&hdma_adc1);
	/* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

	/* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
 * @brief This function handles USART6 global interrupt.
 */
void USART6_IRQHandler(void)
{
	/* USER CODE BEGIN USART6_IRQn 0 */
	if (LL_USART_IsActiveFlag_RXNE(USART6)) {
		LL_USART_ClearFlag_RXNE(USART6);
		uart6_rx_data = LL_USART_ReceiveData8(USART6);
		uart6_rx_flag = 1;

		//		while(!LL_USART_IsActiveFlag_TXE(UART4));
		//		LL_USART_TransmitData8(UART4, uart6_rx_data);
	}

	/* USER CODE END USART6_IRQn 0 */
	/* USER CODE BEGIN USART6_IRQn 1 */

	/* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/