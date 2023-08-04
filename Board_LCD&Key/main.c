/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD.h"
#include "stdio.h"

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
 ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void create_menu(void){
	LCD_Puts(0, 0,  "1.Sin");
	LCD_Puts(6, 0,  "2.Squ");
	LCD_Puts(12, 0, "3.Tri");
	LCD_Puts(0, 1,  "4.|S|");
	LCD_Puts(6, 1,  "5.Stp");
	LCD_Puts(12, 1, "6.Sw");
}

void keypad_config(void){
	
	GPIO_InitTypeDef PinConfing;
	PinConfing.Pin = GPIO_PIN_0;
	PinConfing.Mode = GPIO_MODE_INPUT;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_1;
	PinConfing.Mode = GPIO_MODE_INPUT;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_2;
	PinConfing.Mode = GPIO_MODE_INPUT;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_3;
	PinConfing.Mode = GPIO_MODE_INPUT;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_4;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_5;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_6;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &PinConfing);
}

int column_check(void){
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 0)
		return 1;
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == 0)
		return 4;
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 0)
		return 7;
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) == 0)
		return 42; /* star */
		
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 0)
		return 2;
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == 0)
		return 5;
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 0)
		return 8;
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) == 0)
		return 0; 
	
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 0)
		return 3;
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == 0)
		return 6;
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == 0)
		return 9;
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3) == 0)
		return 35;  /* Hashtag */ 
	
	return 100;
}

ADC_HandleTypeDef poten_config(void){
	GPIO_InitTypeDef PinConfig;
	PinConfig.Pin = GPIO_PIN_0;
	PinConfig.Mode = GPIO_MODE_ANALOG;
	PinConfig.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &PinConfig);
	
	__HAL_RCC_ADC1_CLK_ENABLE();
	ADC_HandleTypeDef HADC;
	HADC.Instance = ADC1;
	HADC.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	HADC.Init.Resolution = ADC_RESOLUTION_12B;
	HADC.Init.ScanConvMode = DISABLE;
	HADC.Init.ContinuousConvMode = ENABLE;
	HADC.Init.DiscontinuousConvMode = DISABLE;
	HADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	HADC.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	HADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	HADC.Init.NbrOfConversion = 1;
	HADC.Init.DMAContinuousRequests = DISABLE;
	HADC.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&HADC) != HAL_OK){
		while(1);
	}
	ADC_ChannelConfTypeDef ConfigS;
	ConfigS.Channel = ADC_CHANNEL_0;
	ConfigS.Rank = 1;
	ConfigS.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	HAL_ADC_Start(&HADC);
	if (HAL_ADC_ConfigChannel(&HADC, &ConfigS) != HAL_OK){
		while(1);
	}
	return HADC;
}

uint16_t poten_input(ADC_HandleTypeDef HADC){
	if(HAL_ADC_Start (&HADC) != HAL_OK){
		while(1);
	}
	if(HAL_ADC_PollForConversion(&HADC, HAL_MAX_DELAY) != HAL_OK){
		while(1);
	}
	uint16_t read_ADC = HAL_ADC_GetValue(&HADC);
	HAL_ADC_Stop(&HADC);
	return read_ADC;
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
	__HAL_RCC_GPIOA_CLK_ENABLE();    /* enable clock for port A */
	__HAL_RCC_GPIOB_CLK_ENABLE();		 /* enable clock for port B */
	__HAL_RCC_GPIOC_CLK_ENABLE();    /* enable clock for port C */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();															 /* initialize LCD */ 
	LCD_Puts(4, 0, "Welcome!");              /* write 'Welcome!' in column 4 and row 0 */ 
	LCD_Puts(0, 1, "992430 14 & 48");		     /* write 'Eftekhari' in column 0 and row 1 */
	HAL_Delay(3000);												 /* wait for 5000ms (5 sec) */
	LCD_Clear();
	create_menu();
	HAL_Delay(3000);
		
	uint8_t input_key;
	int poten_time = 0;
	int poten_freq = 0;
	ADC_HandleTypeDef HADC;
	HADC = poten_config();
	keypad_config();
	char str[10];
	uint8_t tx_buffer[3];
	uint8_t ack[0];
	
	
	GPIO_InitTypeDef PinConfing;
	PinConfing.Pin = GPIO_PIN_0;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &PinConfing);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		LCD_Clear();
		create_menu();
		input_key = 100;
		
		while(input_key > 6) {
			input_key = column_check();
			if (input_key <= 6) {
				sprintf(str, "%d" , input_key);
				LCD_Clear();
				LCD_Puts(0,0, str);
				HAL_Delay(1000);
				tx_buffer[0] = input_key;
			}		
		}
		while(column_check() != 35){ // time
			LCD_Clear();
			poten_time = poten_input(HADC);
			tx_buffer[1] = (poten_time / 4095) * 255;
			poten_time = (poten_time * 5 * 1900 / 4095) + 500;
			sprintf(str, "%d", poten_time);
			LCD_Puts(0, 0, "Time is ");
			LCD_Puts(0, 1, str);
			LCD_Puts(6, 1, "ms");
			HAL_Delay(200);
		}
		HAL_Delay(200);
		while(column_check() != 35){ // freq
			LCD_Clear();
			poten_freq = poten_input(HADC);
			tx_buffer[2] = (poten_freq / 4095) * 255;
			poten_freq = (poten_freq * 5 * 200 / 4095);
			sprintf(str, "%d", poten_freq);
			LCD_Puts(0, 0, "Freq is ");
			LCD_Puts(0, 1, str);
			LCD_Puts(6, 1, "Hz");
			HAL_Delay(200);
		}
		
		HAL_SPI_Transmit(&hspi1, tx_buffer, 3, HAL_MAX_DELAY);
		
    /* USER CODE BEGIN 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
