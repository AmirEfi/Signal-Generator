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
#define DAC_RESOLUTION 255

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
 SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void DAC_output_config(void){
	GPIO_InitTypeDef PinConfing;
	PinConfing.Pin = GPIO_PIN_0;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_1;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_2;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_3;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_4;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_5;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_6;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &PinConfing);
	
	PinConfing.Pin = GPIO_PIN_7;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &PinConfing);
	
}
int sin_generator(int freq, int duration, const double * LUT_sin){
	int num = 256;
	int maxt = num*(duration/1000)*freq;
	int counter = maxt;
	float delayt = 0.5;
	while (counter > 0){
		for(int i =0;i<num;++i){
			GPIOB->ODR=LUT_sin[i]; 
			counter--;
			if (counter<=0){
				return 0 ;
			}
			HAL_Delay(delayt);
		}
	}
return 0;
}

int sin_abc_generator(int freq, int duration, const double * LUT_sin){ 
  int num = 385;
  int numh = 192;
  int maxt =num*(duration/1000)*freq;
  int counter = maxt;
  double delayt = 0.5;

  while (counter>0) {
		for(int i=0;i<128;i++){
			GPIOB->ODR=LUT_sin[i];
			counter--; 
			if(0>=counter){
				return 0 ;
			}
			HAL_Delay(delayt);
		} 
  }
return 0; 
}

int square_generator(int freq,int duration){
	float delayt = 1.0;
	int counter=(2*duration*freq)/1000;
	while (counter>0){      
    GPIOB->ODR=0xFF;
		HAL_Delay(delayt);
		counter--;
		if (counter <= 0){
			return 0;
		}  			  
		GPIOB->ODR=0X00;
		HAL_Delay(delayt);
		counter--;
		if (counter <= 0){
			return 0;
		}
	}
return 0;			
}

int triangle_generator(int freq , int duration){
	int maxt = 255*(duration/1000)*freq;
	float delayt = 1.0;
	float delayt2 = 2.0;
	int counter = maxt;
	while(counter>0){
		for(int i = 0; i < DAC_RESOLUTION/2; i++) {          
			counter--;
			if (counter<=0){
				return 0;
			}
			GPIOB->ODR=i;         
      HAL_Delay(delayt);
		}	
		for(int i = DAC_RESOLUTION/2; i>=0 ; i--) {
			counter--;
			if (counter<=0){
				return 0;
			}
			GPIOB->ODR=i;
      HAL_Delay(delayt);
		}	
		HAL_Delay(delayt2);			
	}
return 0;
}

int sawtooth_generator(int freq , int duration){
	int maxt =(DAC_RESOLUTION/2) * (duration/1000)*freq;
	float delayt = 1.0;
	float delayt2 = 2.0;
	int counter=maxt;
	while (counter>0){
		for(int i = 0; i < DAC_RESOLUTION/2; i++) {
			counter--;
			if (counter<=0){
				return 0;
			}  			
      GPIOB->ODR=i;
      HAL_Delay(delayt);
		}
		HAL_Delay(delayt2);
	}
return 0;
}

int step_generator(int freq , int duration){
		int maxj = 5000;
		int maxt = DAC_RESOLUTION*(duration/1000)*freq*maxj;
		float delayt = 10.0;
		int counter = maxt;
		int stp=10;
		while (counter>0){
			
			for(int i = 0; i <= DAC_RESOLUTION/2; i+=stp) {					
				for(int j=0;j<maxj; j++){           
					GPIOB->ODR=i;
					counter--;
					if (counter<=0){
						return 0;
					}
				}
				HAL_Delay(delayt);
			}
			
			for(int i = DAC_RESOLUTION/2; i >=0 ; i-=stp) {									
				for(int j=0;j<maxj; j++){
					GPIOB->ODR=i;
					counter--;
					if (counter<=0){
						return 0;
					}			
				}
				HAL_Delay(delayt);
			}
			HAL_Delay(delayt);			
		}
return 0;
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
	DAC_output_config();
	//const uint16_t LUT_sin[512]={128,129,131,132,134,135,137,138,140,142,143,145,146,148,149,151,152,154,155,157,159,160,162,163,165,166,168,169,171,172,173,175,176,178,179,181,182,184,185,186,188,189,190,192,193,195,196,197,198,200,201,202,204,205,206,207,209,210,211,212,213,214,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,235,236,237,238,239,239,240,241,242,242,243,244,244,245,245,246,247,247,248,248,249,249,250,250,250,251,251,252,252,252,253,253,253,253,254,254,254,254,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,254,254,254,254,254,253,253,253,252,252,252,251,251,251,250,250,249,249,248,248,247,247,246,246,245,244,244,243,243,242,241,240,240,239,238,237,237,236,235,234,233,232,231,231,230,229,228,227,226,225,224,223,222,221,219,218,217,216,215,214,213,212,210,209,208,207,205,204,203,202,200,199,198,196,195,194,192,191,190,188,187,186,184,183,181,180,179,177,176,174,173,171,170,168,167,165,164,162,161,159,158,156,155,153,152,150,149,147,145,144,142,141,139,138,136,135,133,131,130,128,127,125,124,122,120,119,117,116,114,113,111,110,108,106,105,103,102,100,99,97,96,94,93,91,90,88,87,85,84,82,81,79,78,76,75,74,72,71,69,68,67,65,64,63,61,60,59,57,56,55,53,52,51,50,48,47,46,45,43,42,41,40,39,38,37,36,34,33,32,31,30,29,28,27,26,25,24,24,23,22,21,20,19,18,18,17,16,15,15,14,13,12,12,11,11,10,9,9,8,8,7,7,6,6,5,5,4,4,4,3,3,3,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,3,3,3,4,4,5,5,5,6,6,7,7,8,8,9,10,10,11,11,12,13,13,14,15,16,16,17,18,19,20,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,41,42,43,44,45,46,48,49,50,51,53,54,55,57,58,59,60,62,63,65,66,67,69,70,71,73,74,76,77,79,80,82,83,84,86,87,89,90,92,93,95,96,98,100,101,103,104,106,107,109,110,112,113,115,117,118,120,121,123,124,126,127};
	const double LUT_sin[256] = {0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,0x98,0x9b,0x9e,0xa2,0xa5,0xa7,0xaa,0xad,0xb0,0xb3,0xb6,0xb9,0xbc,0xbe,0xc1,0xc4,0xc6,0xc9,0xcb,0xce,0xd0,0xd3,0xd5,0xd7,0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xeb,0xed,0xee,0xf0,0xf1,0xf3,0xf4,0xf5,0xf6,0xf8,0xf9,0xfa,0xfa,0xfb,0xfc,0xfd,0xfd,0xfe,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfe,0xfd,0xfd,0xfc,0xfb,0xfa,0xfa,0xf9,0xf8,0xf6,0xf5,0xf4,0xf3,0xf1,0xf0,0xee,0xed,0xeb,0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,0xda,0xd7,0xd5,0xd3,0xd0,0xce,0xcb,0xc9,0xc6,0xc4,0xc1,0xbe,0xbc,0xb9,0xb6,0xb3,0xb0,0xad,0xaa,0xa7,0xa5,0xa2,0x9e,0x9b,0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x67,0x64,0x61,0x5d,0x5a,0x58,0x55,0x52,0x4f,0x4c,0x49,0x46,0x43,0x41,0x3e,0x3b,0x39,0x36,0x34,0x31,0x2f,0x2c,0x2a,0x28,0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,0x15,0x14,0x12,0x11,0xf,0xe,0xc,0xb,0xa,0x9,0x7,0x6,0x5,0x5,0x4,0x3,0x2,0x2,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x2,0x2,0x3,0x4,0x5,0x5,0x6,0x7,0x9,0xa,0xb,0xc,0xe,0xf,0x11,0x12,0x14,0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,0x25,0x28,0x2a,0x2c,0x2f,0x31,0x34,0x36,0x39,0x3b,0x3e,0x41,0x43,0x46,0x49,0x4c,0x4f,0x52,0x55,0x58,0x5a,0x5d,0x61,0x64,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c};
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	TIM5->PSC   = 16000-1;
	TIM5->CNT   = 0;
	TIM5->CR1   |= (1<<3) ;
	
	uint8_t rc_buffer[3];
	uint8_t numWave = 0;
	int freq = 0;
	int duration = 0;
	uint8_t ack[1];
		
	GPIO_InitTypeDef PinConfing;
	PinConfing.Pin = GPIO_PIN_0;
	PinConfing.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfing.Speed = GPIO_SPEED_FREQ_HIGH;
	PinConfing.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &PinConfing);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		HAL_SPI_Receive(&hspi1, rc_buffer, 3, HAL_MAX_DELAY);
		numWave = rc_buffer[0];
		duration = (rc_buffer[1] * 4095) / 255;
		duration = (duration * 5 * 1900 / 4095) + 500;
		freq = (rc_buffer[2] * 4095) / 255;
		freq = (freq * 5 * 200 / 4095);
		
		TIM5->CNT   = 0;
		TIM5->CR1   |= 1;
		TIM5->ARR   = (5000-1)/3;
		
		switch(numWave) {
			case 1:
				sin_generator(1000, 8000, LUT_sin);
				ack[0] = 1;
				break;
			case 2:
				square_generator(2000, 9000);
				ack[0] = 1;
				break;
			case 3:
				triangle_generator(2000, 9000);
				ack[0] = 1;
				break;
			case 4:
				sin_abc_generator(2000, 9000, LUT_sin);
				ack[0] = 1;
				break;
			case 5:
				step_generator(2000, 9000);
				ack[0] = 1;
				break;
			case 6:
				sawtooth_generator(2000, 9000);
				ack[0] = 1;
				break;
			default:
				break;
		}
		GPIOC->ODR = ack[0];
		HAL_Delay(1000);
		ack[0] = 0;
		GPIOC->ODR = ack[0];
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
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_INPUT;
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
