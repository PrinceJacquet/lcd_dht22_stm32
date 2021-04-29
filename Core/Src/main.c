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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>

/* ---- WIFI ---- */
#include "wifly_lib.h"

/*	WIFLY_AUTH_OPEN        0    // Open (default)
	WIFLY_AUTH_WEP         1    // WEP-128
	WIFLY_AUTH_WPA1        2    // WPA1
	WIFLY_AUTH_WPA1_2      3    // Mixed-mode WPA1 and WPA2-PSK
	WIFLY_AUTH_WPA2_PSK    4    // WPA2-PSK
	WIFLY_AUTH_ADHOC       6    // Ad-hoc, join any Ad-hoc network
*/
#define _home_settings

#ifdef _smartphone_settings
	#define SSID	"Galaxy-S8"
	#define KEY		"123456789"
	#define AUTH	WIFLY_AUTH_WPA2_PSK
#endif

#ifdef _home_settings
	#define SSID	"Bbox-37DD33DC"
	#define KEY		"dpKRFLNP32ynFkkPr9"
	#define AUTH	WIFLY_AUTH_WPA2_PSK
#endif

#define URL		"httpbin.org"//"api.openweathermap.org" //"e40810fd255dcc.localhost.run"

#define HOST_IP 	"192.168.1.49" 	//IP of the server we want to access
#define HOST_PORT 	"4010" 				//The port to access
#define STD_PORT 	"80"


/* ---- LCD ---- */
#include "lib_lcd.h"
/* ---- DHT22 ---- */
#include "DHT22.h"
/* ---- TCS34725 ---- */
#include "tcs34725.h"







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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_USART2_UART_Init();
  MX_TIM7_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */



  int ok[MAX_RCP_LEN ] = {0};
  char GET_DATA_Send[100] = "GET /ShowData?id=10";
  //char path = "/ShowData?";
  int  id = 10, temp = 2000, humA = 4560, humS = 6700 , Waterlvl = 5000;
  int reboot = 0, i =0;



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */




  /* ----- INIT LCD ----- */

    rgb_lcd LCD_DataStruct;
    LCD_DataStruct._displaycontrol = LCD_DISPLAYON;
    LCD_DataStruct._displayfunction = LCD_2LINE;
    LCD_DataStruct._displaymode = LCD_ENTRYLEFT;


    lcd_init(&hi2c1, &LCD_DataStruct);

    reglagecouleur(50,100,255);
    HAL_Delay(1000);
    reglagecouleur(100,200,255);
    HAL_Delay(1000);
    reglagecouleur(255,255,255);
    HAL_Delay(1000);



    char text_buff[32] = "Initialisation           ";
    //sprintf(text_buff,"Temp : %.2f C",DHT22_DataStruct.T);

    lcd_position(&hi2c1,0,0);
    lcd_print(&hi2c1, text_buff);




    /* lCD INIT : DHT22 */

    sprintf(text_buff,"du DHT22              ");

    lcd_position(&hi2c1,0,1);
    lcd_print(&hi2c1, text_buff);
    HAL_Delay(500);

    /* INIT DHT22 */
     DHT22_Init(&htim7, 32e6,DHT22_SENS_GPIO_Port,DHT22_SENS_Pin, DHT22_SENS_EXTI_IRQn);
     DHT22_Data DHT22_DataStruct;

//    /* TEST : Print measures on the LCD screen */

 /*   HAL_Delay(500);

    	DHT22_DataStruct = DHT22_ReadData();
   	  if(DHT22_DataStruct.is_data_valid)
   	  {
   		  char text_buff[32];
   		  sprintf(text_buff,"Temp : %.2f C         ",DHT22_DataStruct.T);
   		  lcd_position(&hi2c1,0,1);
   		  lcd_print(&hi2c1, text_buff);
   		  HAL_Delay(1000);


   		  sprintf(text_buff,"Hum : %.2f %%         ",DHT22_DataStruct.RH);
   		  lcd_position(&hi2c1,0,1);
   		  lcd_print(&hi2c1, text_buff);
   		  HAL_Delay(1000);



   	  }
*/


   	/* ----- ADC - Water Level  ----- */

   	sprintf(text_buff,"du Niv D'eau           ");

   	lcd_position(&hi2c1,0,1);
   	lcd_print(&hi2c1, text_buff);
   	HAL_Delay(1000);

    uint16_t raw_wtr_lvl;
    char msg_raw_wtr_lvl[10];
    //int cln_wtr_lvl =0;





    /* ----- LCD INIT : TCS34 ---- */
        sprintf(text_buff,"du TCS34          ");

        lcd_position(&hi2c1,0,1);
        lcd_print(&hi2c1, text_buff);
        HAL_Delay(1000);


        float red,green,blue;


    /* lCD INIT : WIFI */
    sprintf(text_buff,"du module wifi          ");

    lcd_position(&hi2c1,0,1);
    lcd_print(&hi2c1, text_buff);













  /* -----  WIFI_authentification(SSID,AUTH,KEY);  ----- */

    commandMode();

	sendCommand("set wlan ssid " SSID "\r", "OK",ok);
	  if (strcmp(AUTH , WIFLY_AUTH_OPEN) > 0 || strcmp(AUTH , WIFLY_AUTH_OPEN) < 0 )
	    {
		  sendCommand("set wlan auth " AUTH "\r","OK", ok);
	  	  if (strcmp(AUTH , WIFLY_AUTH_WEP)==0)
	  	  {
	  		sendCommand("set wlan key " KEY "\r","OK", ok); // Key must be EXACTLY 13 bytes (26 ASCII chars)
	  	  }
	  	  else{
	  		sendCommand("set wlan phrase " KEY"\r","OK", ok);

	  	  }
	    }




   /* -----  Connect to server ----- */



  //sendCommand("set ip address 0\r","OK",ok);// so WiFly will use DNS (when of need of DNS)
  //sendCommand("set dns name " URL"\r", "AOK", ok);



  //sendCommand("set comm open *OPEN*\r","OK",ok); // set the string that the wifi shield will output when a connection is opened

  //sendCommand("open " HOST_IP " " HOST_PORT"\r","*OPEN*",ok);
  //sendCommand("open\r","*OPEN*",ok); // One time opening of connection to server


  /* -----  Save wlan config ----- */

  sendCommand("save\r", "Storing in config",ok);
  sendCommand("reboot\r", "*READY*", ok); //After reboot we directly go to dataMode

 /*

    -----  Join access point -----
    HAL_Delay(1000);
    commandMode();
    sendCommand("join " SSID "\r", "Associated!",ok);

  sendCommand("exit\r", "EXIT", ok); // go into DATA mode (mode in which we can communicate with the network/http)

 */




  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  /*-----  Join access point -----*/

	  HAL_Delay(1000);
	  commandMode();
	  sendCommand("show  net\r", "Assoc=OK",ok);
	  if (strstr((char *)ok,"Assoc=OK")==NULL)
	  {
		  sendCommand("join\r","Associated!",ok);
		  HAL_Delay(2000);
		  sendCommand("set ip protocol 18\r","OK",ok); //enable html client
		    sendCommand("set com remote 0\r","OK",ok); // turn off the REMOTE string so it does not interfere with the post


		    sendCommand("set ip host " HOST_IP"\r","OK",ok); //set remote IP to connect to
		    sendCommand("set ip remote "HOST_PORT"\r","OK",ok); //set remote Port to connect to
	  sendCommand("set uart mode 2\r","AOK",ok); //auto connect on every UART message


	  }
	  sendCommand("exit\r","EXIT", ok);



	  /* Code Capteurs */



		  	  char text_buff[32] = "Les Mesures :       ";
		 	  lcd_position(&hi2c1,0,0);
		  	  lcd_print(&hi2c1, text_buff);



		  	  sprintf(text_buff,"                          .");
		  	  lcd_position(&hi2c1,0,1);
		  	  lcd_print(&hi2c1, text_buff);




	  // DHT22

	    DHT22_DataStruct = DHT22_ReadData();
	   	  if(DHT22_DataStruct.is_data_valid)
	   	  {
	   		  char text_buff[32];
	   		  sprintf(text_buff,"Temp : %d C       ",(int)(DHT22_DataStruct.T));
	   		  lcd_position(&hi2c1,0,1);
	   		  lcd_print(&hi2c1, text_buff);
	   		  HAL_Delay(1000);


	   		  sprintf(text_buff,"Hum : %d %%      ",(int)(DHT22_DataStruct.RH));
	   		  lcd_position(&hi2c1,0,1);
	   		  lcd_print(&hi2c1, text_buff);
	   		  HAL_Delay(1000);


	   	  }




	    // WATER LEVEL



	    // Get ADC value
	   	    HAL_ADC_Start(&hadc);
	   	    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
	   	    raw_wtr_lvl = HAL_ADC_GetValue(&hadc);



	   	    // Convert to string and print
	   	    sprintf(msg_raw_wtr_lvl, "%hu\r\n", raw_wtr_lvl);
	   	    HAL_UART_Transmit(&huart2, (uint8_t*)msg_raw_wtr_lvl, strlen(msg_raw_wtr_lvl), HAL_MAX_DELAY);

	   	    // get pourcentage value
	   	    Waterlvl = raw_wtr_lvl * 100 / 4095 ;//4095 adc 12 bits resolution

	   //affichage LCD water level


	   	 sprintf(text_buff,"Hum Terre : %d %%     ",Waterlvl);
	   	 lcd_position(&hi2c1,0,1);
	   	 lcd_print(&hi2c1, text_buff);
	   	 HAL_Delay(1000);


	   	tcs34725_get_RGB_Values(&red, &green, &blue);//lecture rgb

	   	//LCD screen take ambient color
	   	reglagecouleur((int) red,(int) green,(int) blue);


	    // affichage de la temperature sur l'ecran lcd
	    //lcd_clear();

	   	sprintf(text_buff,"R%.0d G%.0d B%.0d      ",(int) red,(int) green,(int) blue);
	    lcd_position(&hi2c1,0,1);
	    lcd_print(&hi2c1, text_buff);
	    HAL_Delay(1000);





	  /* Send measures to server */

	  snprintf(GET_DATA_Send,100,"GET %sid=%d&temp=%d&huma=%d&hums=%d&wtrlvl=%d","/ShowData?",
			  	  	  	  	  id + i, (int)(DHT22_DataStruct.T * 100), (int)(DHT22_DataStruct.RH * 100), humS, Waterlvl);

	  sendData(GET_DATA_Send,"200 OK", ok);
	  i++;

	  /* test Failure and reboot Wifi Module*/
	  if( strstr((char *)ok,"ERR") != NULL ||  strcmp((char *)ok,"") == 0 )
	  {
		  reboot ++;
		  if(reboot >= 2)
		  {
			  reboot = 0;
			  commandMode();
			  sendCommand("reboot\r", "*READY*", ok);
		  }

	  }





	  //HAL_Delay(5000);


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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
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
