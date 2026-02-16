/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ethernet_task.h"
#include "debug_uart.h"
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

UART_HandleTypeDef huart3;

/* Definitions for EthernetTask */
osThreadId_t ethernetTaskHandle;
const osThreadAttr_t ethernetTask_attributes = {
  .name = "EthernetTask",
  .stack_size = 8192,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
void EthernetTask(void *argument);

/* USER CODE BEGIN PFP */

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

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();
  SCB->CCR &= ~SCB_CCR_UNALIGN_TRP_Msk;
  __DSB();
  __ISB();

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */
  /* Тест UART перед запуском RTOS */
  uint8_t msg[] = "\r\n=== SYSTEM START ===\r\n";
  HAL_UART_Transmit(&huart3, msg, sizeof(msg)-1, 100);

  DebugUART_Print("CPU Clock: %lu Hz\r\n", SystemCoreClock);
  DebugUART_Print("Starting FreeRTOS...\r\n");

  /* Тест светодиода - PB0 (зеленый LED) */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Мигнем LED 3 раза */
  for(int i = 0; i < 3; i++) {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
      HAL_Delay(100);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
      HAL_Delay(100);
  }
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  ethernetTaskHandle = osThreadNew(EthernetTask, NULL, &ethernetTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  DebugUART_Print("Starting scheduler...\r\n");
  osKernelStart();

  /* Infinite loop */
  while (1) {}
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

  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* === OSC: HSI 64 MHz, PLL for SYSCLK disabled (как у тебя) === */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    Error_Handler();

  /* === CORE CLOCK: SYSCLK = HSI (64 MHz) === */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    Error_Handler();

  /* ============================================================
   * ВОТ ЭТО ГЛАВНАЯ ДОБАВКА: ETH1 kernel clock от PLL3Q = 50MHz
   * HSI = 64MHz
   * PLL3M=4 => 16MHz
   * PLL3N=100 => 1600MHz VCO
   * PLL3Q=32 => 50MHz (RMII-friendly)
   * ============================================================ */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ETH1;
  PeriphClkInitStruct.Eth1ClockSelection   = RCC_ETH1CLKSOURCE_PLL3;

  PeriphClkInitStruct.PLL3.PLL3M = 4;
  PeriphClkInitStruct.PLL3.PLL3N = 100;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 32;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3RGE    = RCC_PLL3VCIRANGE_2;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    Error_Handler();
}


/**
  * @brief USART3 Initialization Function
  */
static void MX_USART3_UART_Init(void)
{
  /* USER CODE BEGIN USART3_Init 0 */
  /* Включаем тактирование USART3 */
  __HAL_RCC_USART3_CLK_ENABLE();
  /* USER CODE END USART3_Init 0 */

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;

  if (HAL_UART_Init(&huart3) != HAL_OK)
    Error_Handler();

  /* USER CODE BEGIN USART3_Init 2 */
  uint8_t test_msg[] = "\r\n=== USART3 INIT OK (PD8/PD9) ===\r\n";
  HAL_UART_Transmit(&huart3, test_msg, sizeof(test_msg)-1, 100);
  /* USER CODE END USART3_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* НАСТРАИВАЕМ USART3 НА PD8(TX)/PD9(RX) */

  /* Включаем тактирование GPIOD */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* PD8 - USART3_TX */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* PD9 - USART3_RX */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* ЗАКОММЕНТИРУЙТЕ СГЕНЕРИРОВАННЫЙ БЛОК USART3, ЕСЛИ ОН ЕСТЬ */
  /*
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    HAL_Delay(100);
  }
}

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disable MPU */
  HAL_MPU_Disable();

  /* Region 0: FLASH 0x08000000, 1MB, executable, cacheable */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress      = 0x08000000;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_1MB;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;   /* <-- ВАЖНО: разрешаем выполнение */
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Region 1: RAM_D1 0x24000000, 512KB (покрывает 320KB), executable, cacheable */
  MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress      = 0x24000000;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Region 2: RAM_D2 0x30000000, 32KB, non-cacheable, XN (для DMA/ETH/LwIP) */
  MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress      = 0x30000000;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_32KB;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;  /* XN */
  MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Region 3: DTCM 0x20000000, 128KB, non-cacheable */
  MPU_InitStruct.Number           = MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress      = 0x20000000;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_128KB;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable MPU: важно включить PRIVILEGED_DEFAULT, чтобы не “отрезать” карту памяти */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

  /* Enable caches */
  SCB_EnableICache();
  SCB_EnableDCache();
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
