/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BNO080.h"
#include "Quaternion.h"
#include "ICM20602.h"
#include "LPS22HH.h"
#include "M8N.h"
#include "FS_iA6B.h"
#include "AT24C08.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int _write(int file, char *p, int len) {
	for (int i = 0; i < len; i++) {
		while (!LL_USART_IsActiveFlag_TXE(USART6))
			;
		LL_USART_TransmitData8(USART6, *(p + i));
	}
	return len;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t uart6_rx_flag;
extern uint8_t uart6_rx_data;
extern uint8_t m8n_rx_buf[36];
extern uint8_t m8n_rx_cplt_flag;
extern uint8_t ibus_rx_buf[32];
extern uint8_t ibus_rx_cplt_flag;
extern uint8_t uart1_rx_data;
uint8_t telemetry_tx_buf[40];
extern uint8_t tim7_20ms_flag;
extern uint8_t tim7_100ms_flag;
float batteryVolt;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int Is_iBus_Throttle_Min(void);
void ESC_Calibration(void);
int Is_iBus_Received(void);
void BNO080_Calibration(void);
void Encode_Msg_AHRS(unsigned char *telemetry_tx_buf);
void Encode_Msg_GPS(unsigned char *telemetry_tx_buf);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	float q[4];
	float quatRadianAccuracy;
	unsigned char buf_read[16] = {1};
	unsigned char buf_write[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	unsigned short adcVal;

	short gyro_x_offset = 8;
	short gyro_y_offset = -23;
	short gyro_z_offset = -2;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM3_Init();
	MX_USART6_UART_Init();
	MX_SPI2_Init();
	MX_SPI1_Init();
	MX_SPI3_Init();
	MX_UART4_Init();
	MX_UART5_Init();
	MX_TIM5_Init();
	MX_I2C1_Init();
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	MX_TIM7_Init();
	/* USER CODE BEGIN 2 */
	//TIM3 Initialization
	LL_TIM_EnableCounter(TIM3);

	//UART4, 5, 6 Initialization
	LL_USART_EnableIT_RXNE(USART6);
	LL_USART_EnableIT_RXNE(UART4);
	LL_USART_EnableIT_RXNE(UART5);

	//9DOF Initialization
	BNO080_Initialization();
	BNO080_enableRotationVector(2500); //400Hz, maximum value describing in datasheet

	//6DOF Initialization
	ICM20602_Initialization();
	LPS22HH_Initialization();
	M8N_Initialization();

	//TIM5 Initialization
	LL_TIM_EnableCounter(TIM5);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH2);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH4);

	HAL_ADC_Start_DMA(&hadc1, &adcVal, 1);

	//UART1 - HAL Rx Interrupt
	HAL_UART_Receive_IT(&huart1, &uart1_rx_data, 1);

	//TIM7 Initialization for FC <-> GCS
	LL_TIM_EnableCounter(TIM7);
	LL_TIM_EnableIT_UPDATE(TIM7);

	//ICM20602 DC BIAS OFFSET CALIBRATION
	ICM20602_Writebyte(0x13, (gyro_x_offset * -2) >> 8);
	ICM20602_Writebyte(0x14, (gyro_x_offset * -2));

	ICM20602_Writebyte(0x15, (gyro_y_offset * -2) >> 8);
	ICM20602_Writebyte(0x16, (gyro_y_offset * -2));

	ICM20602_Writebyte(0x17, (gyro_z_offset * -2) >> 8);
	ICM20602_Writebyte(0x18, (gyro_z_offset * -2));

	//	adcVal = ADC1 -> DR;
	//	//FS-i6 PACKET CHECK
	//	while(Is_iBus_Received() == 0) {
	//		LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//		TIM3->PSC = 3000;
	//		HAL_Delay(200);
	//		LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//		HAL_Delay(200);
	//	}
	//
	//
	//	//CALIBRATION BASED ON SwC
	//	if(iBus.SwC == 2000) {
	//		//ESC CALIBRATION
	//		LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//		TIM3->PSC = 1500;
	//		HAL_Delay(200);
	//		TIM3->PSC = 4000;
	//		HAL_Delay(200);
	//		TIM3->PSC = 1500;
	//		HAL_Delay(200);
	//		TIM3->PSC = 4000;
	//		HAL_Delay(200);
	//		LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//
	//		ESC_Calibration();
	//		while(iBus.SwC != 1000) {
	//			Is_iBus_Received();
	//			LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//			TIM3->PSC = 1500;
	//			HAL_Delay(200);
	//			TIM3->PSC = 6000;
	//			HAL_Delay(200);
	//			LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//		}
	//	}
	//	else if (iBus.SwC == 1500) {
	//		//BNO080 CALIBRATION
	//		LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//		TIM3->PSC = 1500;
	//		HAL_Delay(200);
	//		TIM3->PSC = 4000;
	//		HAL_Delay(200);
	//		TIM3->PSC = 1500;
	//		HAL_Delay(200);
	//		TIM3->PSC = 4000;
	//		HAL_Delay(200);
	//		LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//
	//		BNO080_Calibration();
	//		while(iBus.SwC != 1000) {
	//			Is_iBus_Received();
	//			LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//			TIM3->PSC = 1500;
	//			HAL_Delay(200);
	//			TIM3->PSC = 6000;
	//			HAL_Delay(200);
	//			LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//		}
	//	}
	//
	//
	//
	//	while(Is_iBus_Throttle_Min() == 0) {
	//		LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//		TIM3->PSC = 1500;
	//		HAL_Delay(100);
	//		LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	//		HAL_Delay(100);
	//	}

	//	//EEPROM Write
	//	EP_PIDGain_Write(0, 1.1, 2.2, 3.3);
	//	float p = 0.0, i = 0.0, d = 0.0;
	//
	//	//EEPROM Read
	//	EP_PIDGain_Read(0, &p, &i, &d);
	//	printf("%f %f %f", p, i, d);

	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	TIM3->PSC = 6000;
	HAL_Delay(200);
	TIM3->PSC = 4000;
	HAL_Delay(100);
	TIM3->PSC = 4000;
	HAL_Delay(100);
	TIM3->PSC = 6000;
	HAL_Delay(200);
	//  TIM3 -> CCR4 = TIM3 -> ARR / 2;
	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

	printf("Starts\n");

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (tim7_20ms_flag == 1 && tim7_100ms_flag != 1) {
			//20ms - 50Hz
			tim7_20ms_flag = 0;
			Encode_Msg_AHRS(&telemetry_tx_buf[0]);
			//Transmit
			HAL_UART_Transmit_IT(&huart1, &telemetry_tx_buf[0], 20);
		}
		else if (tim7_100ms_flag == 1 && tim7_20ms_flag == 1) {
			//100ms - 10Hz
			tim7_100ms_flag = 0;
			//20ms - 50Hz
			tim7_20ms_flag = 0;
			//AHRS
			Encode_Msg_AHRS(&telemetry_tx_buf[0]);
			//GPS
			Encode_Msg_GPS(&telemetry_tx_buf[20]);
			//Transmit
			HAL_UART_Transmit_IT(&huart1, &telemetry_tx_buf[0], 40);
		}

		//Battery Part
		batteryVolt = adcVal * 0.003619f;
		//		printf("%d\t%.2f\n", adcVal, batteryVolt);
		if (batteryVolt < 10.0f) {
			LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
			TIM3->PSC = 1500;
		}
		else {
			LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
		}


		//BNO080 - 9DOF
		if (BNO080_dataAvailable() == 1) {
			LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_0);

			q[0] = BNO080_getQuatI();
			q[1] = BNO080_getQuatJ();
			q[2] = BNO080_getQuatK();
			q[3] = BNO080_getQuatReal();
			quatRadianAccuracy = BNO080_getQuatRadianAccuracy();

			Quaternion_Update(&q[0]);
			//			printf("%.2f\t%.2f\n", BNO080_Roll, BNO080_Pitch);
			//			printf("%.2f\n", BNO080_Yaw);
		}

		//ICM20602 - 6DOF
		if (ICM20602_DataReady() == 1) {
			LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_1);
			ICM20602_Get3AxisGyroRawData(&ICM20602.gyro_x_raw);
			ICM20602.gyro_x = ICM20602.gyro_x_raw * 2000.f / 32768.f;
			ICM20602.gyro_y = ICM20602.gyro_y_raw * 2000.f / 32768.f;
			ICM20602.gyro_z = ICM20602.gyro_z_raw * 2000.f / 32768.f;
			//		printf("%d,%d,%d\n", ICM20602.gyro_x_raw ,ICM20602.gyro_y_raw, ICM20602.gyro_z_raw);
			//		printf("%d,%d,%d\n", (int)(ICM20602.gyro_x * 100), (int)(ICM20602.gyro_y * 100), (int)(ICM20602.gyro_z * 100));
		}

		//LPS22HH Barometer
		if(LPS22HH_DataReady() == 1) {
			LPS22HH_GetPressure(&LPS22HH.pressure_raw);
			LPS22HH_GetTemperature(&LPS22HH.temperature_raw);
			LPS22HH.baroAlt = getAltitude2(LPS22HH.pressure_raw/4096.f, LPS22HH.temperature_raw / 100.f);
#define X 0.99f
			LPS22HH.baroAltFilt = LPS22HH.baroAltFilt * X + LPS22HH.baroAlt * (1.0f -X);
			//			printf("%d,%d\n", (int)(LPS22HH.baroAlt * 100), (int)(LPS22HH.baroAltFilt * 100));
		}
		//M8N GPS
		if(m8n_rx_cplt_flag == 1) {
			m8n_rx_cplt_flag = 0;
			if (M8N_UBX_CHKSUM_Check(&m8n_rx_buf[0], 36) == 1) {
				LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_2);
				M8N_UBX_NAV_POSLLH_Parsing(&m8n_rx_buf[0], &posllh);

				//				printf("LAT: %ld\tLON: %ld\tHeight: %ld\n", posllh.lat, posllh.lon, posllh.height);
			}
		}
		//Controller
		if (ibus_rx_cplt_flag == 1) {
			ibus_rx_cplt_flag = 0;
			if (iBus_Check_CHECKSUM(&ibus_rx_buf[0], 32) == 1) {
				LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_2);

				iBus_Parsing(&ibus_rx_buf[0], &iBus);
				if (iBus_isActiveFailSafe(&iBus) == 1) {
					LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
				}
				else {
					LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
				}
				//				printf("%d\t%d\t%d\t%d\t%d\t%d\n", iBus.RH, iBus.RV, iBus.LV, iBus.LH, iBus.SwA, iBus.SwC);
				//				HAL_Delay(100);
			}
		}


		//		TIM5 -> CCR1 = 10500 + (iBus.LV - 1000) * 1.5;
		//		TIM5 -> CCR2 = 10500 + (iBus.LV - 1000) * 1.5;
		//		TIM5 -> CCR3 = 10500 + (iBus.LV - 1000) * 1.5;
		//		TIM5 -> CCR4 = 10500 + (iBus.LV - 1000) * 1.5;
		//		tim5_ccr4 += 10;
		//		if (tim5_ccr4 > 21000) tim5_ccr4 = 10500;
		//		TIM5->CCR4 = tim5_ccr4;
		//		HAL_Delay(1);

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
int Is_iBus_Throttle_Min(void) {
	if (ibus_rx_cplt_flag == 1) {
		ibus_rx_cplt_flag = 0;
		if (iBus_Check_CHECKSUM(&ibus_rx_buf[0], 32) == 1) {
			iBus_Parsing(&ibus_rx_buf[0], &iBus);
			if(iBus.LV < 1010) return 1;

		}
	}
	return 0;
}

void ESC_Calibration(void) {
	TIM5 -> CCR1 = 21000;
	TIM5 -> CCR2 = 21000;
	TIM5 -> CCR3 = 21000;
	TIM5 -> CCR4 = 21000;
	HAL_Delay(7000);
	TIM5 -> CCR1 = 10500;
	TIM5 -> CCR2 = 10500;
	TIM5 -> CCR3 = 10500;
	TIM5 -> CCR4 = 10500;
	HAL_Delay(8000);
}

int Is_iBus_Received(void) {
	if (ibus_rx_cplt_flag == 1) {
		ibus_rx_cplt_flag = 0;
		if (iBus_Check_CHECKSUM(&ibus_rx_buf[0], 32) == 1) {
			iBus_Parsing(&ibus_rx_buf[0], &iBus);
			return 1;
		}
	}
	return 0;
}


void BNO080_Calibration(void)
{
	//Resets BNO080 to disable All output
	BNO080_Initialization();

	//BNO080/BNO085 Configuration
	//Enable dynamic calibration for accelerometer, gyroscope, and magnetometer
	//Enable Game Rotation Vector output
	//Enable Magnetic Field output
	BNO080_calibrateAll(); //Turn on cal for Accel, Gyro, and Mag
	BNO080_enableGameRotationVector(20000); //Send data update every 20ms (50Hz)
	BNO080_enableMagnetometer(20000); //Send data update every 20ms (50Hz)

	//Once magnetic field is 2 or 3, run the Save DCD Now command
	printf("Calibrating BNO080. Pull up FS-i6 SWC to end calibration and save to flash\n");
	printf("Output in form x, y, z, in uTesla\n\n");

	//while loop for calibration procedure
	//Iterates until iBus.SwC is mid point (1500)
	//Calibration procedure should be done while this loop is in iteration.
	while(iBus.SwC == 1500)
	{
		if(BNO080_dataAvailable() == 1)
		{
			//Observing the status bit of the magnetic field output
			float x = BNO080_getMagX();
			float y = BNO080_getMagY();
			float z = BNO080_getMagZ();
			unsigned char accuracy = BNO080_getMagAccuracy();

			float quatI = BNO080_getQuatI();
			float quatJ = BNO080_getQuatJ();
			float quatK = BNO080_getQuatK();
			float quatReal = BNO080_getQuatReal();
			unsigned char sensorAccuracy = BNO080_getQuatAccuracy();

			printf("%f,%f,%f,", x, y, z);
			if (accuracy == 0) printf("Unreliable\t");
			else if (accuracy == 1) printf("Low\t");
			else if (accuracy == 2) printf("Medium\t");
			else if (accuracy == 3) printf("High\t");

			printf("\t%f,%f,%f,%f,", quatI, quatI, quatI, quatReal);
			if (sensorAccuracy == 0) printf("Unreliable\n");
			else if (sensorAccuracy == 1) printf("Low\n");
			else if (sensorAccuracy == 2) printf("Medium\n");
			else if (sensorAccuracy == 3) printf("High\n");

			//Turn the LED and buzzer on when both accuracy and sensorAccuracy is high
			if(accuracy == 3 && sensorAccuracy == 3)
			{
				LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2);
				TIM3->PSC = 65000; //Very low frequency
				LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
			}
			else
			{
				LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2);
				LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
			}
		}

		Is_iBus_Received(); //Refreshes iBus Data for iBus.SwC
		HAL_Delay(100);
	}

	//Ends the loop when iBus.SwC is not mid point
	//Turn the LED and buzzer off
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2);
	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

	//Saves the current dynamic calibration data (DCD) to memory
	//Sends command to get the latest calibration status
	BNO080_saveCalibration();
	BNO080_requestCalibrationStatus();

	//Wait for calibration response, timeout if no response
	int counter = 100;
	while(1)
	{
		if(--counter == 0) break;
		if(BNO080_dataAvailable())
		{
			//The IMU can report many different things. We must wait
			//for the ME Calibration Response Status byte to go to zero
			if(BNO080_calibrationComplete() == 1)
			{
				printf("\nCalibration data successfully stored\n");
				LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
				TIM3->PSC = 2000;
				HAL_Delay(300);
				TIM3->PSC = 1500;
				HAL_Delay(300);
				LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
				HAL_Delay(1000);
				break;
			}
		}
		HAL_Delay(10);
	}
	if(counter == 0)
	{
		printf("\nCalibration data failed to store. Please try again.\n");
		LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
		TIM3->PSC = 1500;
		HAL_Delay(300);
		TIM3->PSC = 2000;
		HAL_Delay(300);
		LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
		HAL_Delay(1000);
	}

	//BNO080_endCalibration(); //Turns off all calibration
	//In general, calibration should be left on at all times. The BNO080
	//auto-calibrates and auto-records cal data roughly every 5 minutes

	//Resets BNO080 to disable Game Rotation Vector and Magnetometer
	//Enables Rotation Vector
	BNO080_Initialization();
	BNO080_enableRotationVector(2500); //Send data update every 2.5ms (400Hz)
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//HAL Only, LL cannot be supported
	//HAL UART Receive Call back function
	if (huart->Instance == USART1) {
		HAL_UART_Receive_IT(&huart1, &uart1_rx_data, 1);
		//		HAL_UART_Transmit_IT(&huart1, &uart1_rx_data, 1);
	}
}

void Encode_Msg_AHRS(unsigned char *telemetry_tx_buf) {
	//Sync char
	telemetry_tx_buf[0] = 0x46;
	telemetry_tx_buf[1] = 0x43;
	//AHRS
	telemetry_tx_buf[2] = 0x10;
	//Current Roll
	telemetry_tx_buf[3] = (short)(BNO080_Roll * 100);
	telemetry_tx_buf[4] = ((short)(BNO080_Roll * 100)) >> 8;
	//Current Pitch
	telemetry_tx_buf[5] = (short)(BNO080_Pitch * 100);
	telemetry_tx_buf[6] = ((short)(BNO080_Pitch * 100)) >> 8;
	//Current Yaw
	telemetry_tx_buf[7] = (unsigned short)(BNO080_Yaw * 100);
	telemetry_tx_buf[8] = ((unsigned short)(BNO080_Yaw * 100)) >> 8;
	//Current Altitude
	telemetry_tx_buf[9] = (short)(LPS22HH.baroAltFilt * 10);
	telemetry_tx_buf[10] = ((short)(LPS22HH.baroAltFilt * 100)) >> 8;

	//target Roll at Controller
	telemetry_tx_buf[11] = (short)((iBus.RH - 1500) * 0.1f * 100); //-50 ~ +50
	telemetry_tx_buf[12] = ((short)((iBus.RH - 1500) * 0.1f * 100)) >> 8;

	//target Pitch at Controller
	telemetry_tx_buf[13] = (short)((iBus.RV - 1500) * 0.1f * 100); //-50 ~ +50
	telemetry_tx_buf[14] = ((short)((iBus.RV - 1500) * 0.1f * 100)) >> 8;

	//target Yaw at Controller
	telemetry_tx_buf[15] = (unsigned short)((iBus.LH - 1000) * 0.36f * 100); //0 ~ 360
	telemetry_tx_buf[16] = ((unsigned short)((iBus.LH - 1000) * 0.36f * 100)) >> 8;

	telemetry_tx_buf[17] = 0x00;
	telemetry_tx_buf[18] = 0x00;

	//Checksum
	telemetry_tx_buf[19] = 0xff;
	for(int i = 0; i < 19; i++) {
		telemetry_tx_buf[19] -= telemetry_tx_buf[i];
	}
}

void Encode_Msg_GPS(unsigned char *telemetry_tx_buf) {
	//Sync char
	telemetry_tx_buf[0] = 0x46;
	telemetry_tx_buf[1] = 0x43;
	//0x11 - GPS
	telemetry_tx_buf[2] = 0x11;

	//Latitude
	telemetry_tx_buf[3] = posllh.lat & 0xff;
	telemetry_tx_buf[4] = posllh.lat >> 8;
	telemetry_tx_buf[5] = posllh.lat >> 16;
	telemetry_tx_buf[6] = posllh.lat >> 24;

	//Longitude
	telemetry_tx_buf[7] = posllh.lon & 0xff;
	telemetry_tx_buf[8] = posllh.lon >> 8;
	telemetry_tx_buf[9] = posllh.lon >> 16;
	telemetry_tx_buf[10] = posllh.lon >> 24;

	//Battery Voltage
	telemetry_tx_buf[11] = (unsigned short)(batteryVolt * 100);
	telemetry_tx_buf[12] = ((unsigned short)(batteryVolt * 100)) >> 8;

	//SwA SwC
	telemetry_tx_buf[13] = (iBus.SwA == 1000)? 0 : 1;
	telemetry_tx_buf[14] = (iBus.SwC == 1000)? 0 : (iBus.SwC == 1500)? 1 : 2;

	//Fail-Safe Mode
	telemetry_tx_buf[15] = iBus_isActiveFailSafe(&iBus); // Failsafe active true / inactive false

	//0x00
	telemetry_tx_buf[16] = 0x00;
	telemetry_tx_buf[17] = 0x00;
	telemetry_tx_buf[18] = 0x00;

	//Checksum
	telemetry_tx_buf[19] = 0xff;
	for(int i = 0; i < 19; i++) {
		telemetry_tx_buf[19] -= telemetry_tx_buf[i];
	}
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
