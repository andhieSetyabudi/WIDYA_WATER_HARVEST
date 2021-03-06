/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BASIC_LOGIC.h"
#include "HX710B.h"
#include "AHT10.h"
#include "E32_915T30.h"

#include "SGP40.h"

#include "nextion.h"

#include "SerialUSB.h"
#include "HALSerial.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_RTC_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
char * getUUID(char * buff)
{
	union {
	  uint32_t w[3];
	  uint8_t b[12];
	} uid;
	uid.w[0] = HAL_GetUIDw0();
	uid.w[1] = HAL_GetUIDw1();
	uid.w[2] = HAL_GetUIDw2();
	sprintf(buff, "%02X%02X%02X%02X%02X%02X", uid.b[11], uid.b[10] + uid.b[2], uid.b[9], uid.b[8] + uid.b[0], uid.b[7], uid.b[6]);
	return buff;
}

char* ftoa(float f, char * buf, int precision)
{
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;
	if (precision > MAX_PRECISION)		// check precision bounds
		precision = MAX_PRECISION;
	if (f < 0) {		// sign stuff
		f = -f;
		*ptr++ = '-';
	}
	if (precision < 0) {  // negative precision == automatic precision guess
		if (f < 1.0) precision = 4;
		else if (f < 10.0) precision = 3;
		else if (f < 100.0) precision = 2;
		else if (f < 1000.0) precision = 1;
		else precision = 0;
	}
	if (precision)  	// round value according the precision
		f += rounders[precision];
	// integer part...
	intPart = f;
	f -= intPart;
	if (!intPart)
		*ptr++ = '0';
	else
	{
		p = ptr;	// save start pointer
		while (intPart) { // convert (reverse order)
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}
		p1 = p;  // save end pos
		while (p > ptr)	{ // reverse result
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}
		ptr = p1;	// restore end pos
	}
	if (precision) {	// decimal part
		*ptr++ = '.';	// place decimal point
		while (precision--)	 { // convert
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}
	*ptr = 0;	// terminating zero
	return buf;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void b0PopCallback(void *ptr)
{
	char buffer[100] = {0};
    uint16_t len;
    uint16_t number;
    NexObject_var *btn = (NexObject_var *)ptr;
    memset(buffer, 0, sizeof(buffer));

    /* Get the text value of button component [the value is string type]. */
    NexButton.getText(btn, buffer, 100);
    USBSerial.print(buffer);
//    btn->getText(buffer, sizeof(buffer));?

    number = atoi(buffer);
    number += 1;

    memset(buffer, 0, sizeof(buffer));
    itoa(number, buffer, 10);

    /* Set the text value of button component [the value is string type]. */
    NexButton.setText(btn,buffer);
    HAL_Delay(1000);
//    btn->setText(buffer);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_RTC_Init();
  MX_USB_DEVICE_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  PORT_IO LED_pin_ = {
		  .port = LED_GPIO_Port,
		  .pin = LED_Pin,
  };
  PORT_IO SDI_ = {
		  .port = Pres1_SDO_GPIO_Port,
		  .pin  = Pres1_SDO_Pin,
  };
  PORT_IO SCK_ = {
		  .port = Pres1_SCK_GPIO_Port,
		  .pin = Pres1_SCK_Pin,
  };
  HX710B_var pressure_sensor1;
  SGP40_var VOC_sensor;
  SGP40.halt = HAL_Delay;

  USBSerial.begin();
  Serial1.begin(9600);
  delay_microsInit();
  HX710B.halt = delay_microSeconds;
  HX710B.init(&pressure_sensor1,SCK_, SDI_, MODE_DIFF1);
//  HX710B.init(&pressure_sensor1,SDI_, SCK_);

  AHT10.halt = HAL_Delay;
  AHT10.getTick = HAL_GetTick;
  AHT10_var AHT1;
  AHT10.begin(&AHT1, &hi2c1,0x38);

  SGP40.begin(&VOC_sensor, &hi2c1,DFRobot_SGP40_ICC_ADDR, 10000UL);
  char txt[20]="halloooo \n";
  char tmp[64];

  NexObject_var button_nextion;
  NexHardware.nex_serial = &Serial1;
  NexHardware.delay = HAL_Delay;
  NexButton.create(&button_nextion, 0, 1, "b0");
  NexObject_var *nex_listen_list[] =
  {
      &button_nextion,
      NULL
  };
  NexHardware.init();
//  NexButton.setText(&button_nextion, (const char*)"1");
  NexObject.attachPop(&button_nextion,b0PopCallback, &button_nextion);
  uint8_t num=0;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  NexHardware.loop(nex_listen_list);
	  char num_val[10];
	  sprintf(num_val, "nilai %d", num);
	  if(NexButton.setText(&button_nextion, (const char*)num_val) )
		  USBSerial.print((const char*)"success\r\n");
	  num++;
//	  if ( USBSerial.peek() >= (USB_OTG_FS_MAX_PACKET_SIZE/2) )
//	  	  USBSerial.flush();
//	  USBSerial.puts((uint8_t*)txt,strlen(txt));
/*	  int press_adc = 0;
	  if ( HX710B.read(&pressure_sensor1,&press_adc, 1000) == HX710B_OK )
	  {
//		  uint32_t press_offset = 7240000;
		  sprintf(tmp,"value sensor = %i \n\r",press_adc);
		  USBSerial.puts((uint8_t*)tmp, strlen(tmp));
//		  float scale = press_adc * (-0.000018);
//		  sprintf(tmp,"voltage = %i \n\r",scale*1E+6);
//		  USBSerial.puts((uint8_t*)tmp, strlen(tmp));
	  }

	  if( AHT10.getEvent(&AHT1, 100UL) == AHT10_OK )
	  {
		  sprintf(tmp,"AHT ada \n\r");
		  USBSerial.puts((uint8_t*)tmp, strlen(tmp));
		  char val[10];
		  sprintf(tmp, "temperature %s", ftoa(AHT1.temperature, val, 3));
		  USBSerial.puts((uint8_t*)tmp, strlen(tmp));
	  }
	  uint16_t voc_index = 0;
	  if ( SGP40.getVOCIndex(&VOC_sensor, &voc_index, 1000) == SGP40_OK )
	  {
		  sprintf(tmp, "VOC = %i \n\r", voc_index);
		  USBSerial.puts((uint8_t*)tmp, strlen(tmp));
	  }
*/
	  HAL_Delay(5000);
	  digitalToggle(LED_pin_);
//	  while(Serial1.available() > 0)
//	  {
//		  size_t n_ = Serial1.available();
//		  uint8_t tBuf[n_];
//		  Serial1.gets(tBuf,n_);
//		  char buffer_t[n_ + 10];
//		  sprintf( buffer_t, "n = ");
//		  for ( int i=0; i< n_; i++)
//		  {
//			  char cat_[5];
//			  sprintf(cat_, "%02x", tBuf[i]);
//			  strcat(buffer_t, cat_);
//		  }
//		  strcat( buffer_t, (const char*) "\r\n");
//		  USBSerial.puts(buffer_t, strlen(buffer_t));
////		  USBSerial.flush();
////		  USBSerial.puts((uint8_t*)tBuf, strlen(tBuf));
//
//	  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 19200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Pres1_SCK_GPIO_Port, Pres1_SCK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Pres1_SCK_Pin */
  GPIO_InitStruct.Pin = Pres1_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Pres1_SCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Pres1_SDO_Pin */
  GPIO_InitStruct.Pin = Pres1_SDO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Pres1_SDO_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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
