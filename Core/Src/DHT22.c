/*********************************************************************************
 * \file   : DHT22.c															 *
 * \date  : Mar 20, 2021														 *
 * \author: Sofiane AOUCI														 *
 * \brief : DHT22 library														 *
 *																				 *
 *********************************************************************************
 *	 	 	 	 	 	 Released under MIT License								 *
 * 	 	 	 	 	 	Copyright 2021 Sofiane AOUCI							 *
 *																				 *
 * Permission is hereby granted, free of charge, to any person obtaining a		 *
 * copy of this software and associated documentation files (the "Software"),	 *
 * to deal in the Software without restriction, including without limitation	 *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,		 *
 * and/or sell copies of the Software, and to permit persons to whom the		 *
 * Software is furnished to do so, subject to the following conditions:			 *
 *																				 *
 * The above copyright notice and this permission notice shall be included in	 *
 * all copies or substantial portions of the Software.							 *
 *																				 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR	 *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		 *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL		 *
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER	 *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE *
 * SOFTWARE.																	 *
 *********************************************************************************/
#include "DHT22.h"

DHT22_InitTypedef DHT22;

volatile static uint32_t _TIM_TIC;
volatile static uint8_t _SENSOR_EDGES;
volatile static uint64_t data;


static void set_sensor_pin_mode(uint8_t in);
static void timer_init(TIM_HandleTypeDef* _htim);

void DHT22_Init(TIM_HandleTypeDef* _htim, uint32_t _timer_clock_freq, GPIO_TypeDef* _SENSOR_GPIO_Port, uint8_t _SENSOR_Pin, IRQn_Type _SENSOR_EXTI_IRQn)
{
	DHT22._htim 			= _htim;
	DHT22._tim_clock_freq 		= _timer_clock_freq;
	DHT22._SENSOR_GPIO_Port = _SENSOR_GPIO_Port;
	DHT22._SENSOR_Pin 		= _SENSOR_Pin;
	DHT22._SENSOR_EXTI_IRQn = _SENSOR_EXTI_IRQn;

	timer_init(DHT22._htim);

	set_sensor_pin_mode(0);
	_SENSOR_EDGES = 0;
	HAL_GPIO_WritePin(DHT22._SENSOR_GPIO_Port, DHT22._SENSOR_Pin, 1);
	HAL_Delay(10);
	HAL_TIM_Base_Start_IT(DHT22._htim);

}

DHT22_Data DHT22_ReadData()
{
	DHT22_Data _DHT22_data;
	uint16_t RH, T;
	uint8_t check_sum;
	HAL_GPIO_WritePin(DHT22._SENSOR_GPIO_Port, DHT22._SENSOR_Pin, 0);
	HAL_Delay(5);
	set_sensor_pin_mode(1);

	while(_SENSOR_EDGES <1 + 3 + 2 * 40 + 2);

	set_sensor_pin_mode(0);
	HAL_GPIO_WritePin(DHT22._SENSOR_GPIO_Port, DHT22._SENSOR_Pin, 1);
 	_SENSOR_EDGES = 0;

 	check_sum 	= data & 0xFF;
 	T 			= (data >> 8) & 0xFFFF;
 	RH 			= (data >> 24) & 0xFFFF;

 	_DHT22_data.check_sum 	= check_sum;
 	_DHT22_data.T 			= (float) (T & 0x7FFF) / 10.0;
 	_DHT22_data.T 		   *= (T & 0x8000) ? -1 : 1 ;
 	_DHT22_data.RH 			= (float) RH / 10.0;

 	uint8_t sum = 0;
 	sum = (T & 0xFF) + ((T >> 8) & 0xFF) + (RH & 0xFF) + ((RH >> 8) & 0xFF);

 	_DHT22_data.is_data_valid = (sum == check_sum) ? 1 : 0;

 	return _DHT22_data;
}


static void set_sensor_pin_mode(uint8_t in){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = DHT22._SENSOR_Pin;
	GPIO_InitStruct.Pull = GPIO_PULLUP;

	if(in) 	{
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		HAL_NVIC_EnableIRQ(DHT22._SENSOR_EXTI_IRQn);
	}
	else 	{
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		HAL_GPIO_WritePin(DHT22._SENSOR_GPIO_Port, DHT22._SENSOR_Pin, 1);
		HAL_NVIC_DisableIRQ(DHT22._SENSOR_EXTI_IRQn);
	}

	HAL_GPIO_Init(DHT22._SENSOR_GPIO_Port, &GPIO_InitStruct);

}


static void timer_init(TIM_HandleTypeDef* _htim)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  _htim->Init.Prescaler = DHT22._tim_clock_freq/1e6-1;
  _htim->Init.CounterMode = TIM_COUNTERMODE_UP;
  _htim->Init.Period = 10-1;
  _htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(_htim) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(_htim, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == DHT22._htim->Instance)
	{
		_TIM_TIC++;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) // TIM callback at interruptions
{
	if(GPIO_Pin == DHT22._SENSOR_Pin)
	{
		static uint8_t sensor_0_1 = 0;
		_SENSOR_EDGES++;

		if(_SENSOR_EDGES < 4 + 1);
		else if(_SENSOR_EDGES < 1 + 3 + 2 * 40 + 1)
		{
			if(_SENSOR_EDGES == 4 + 1){
				sensor_0_1 = 0;
				data = 0;
			}

			else if(_SENSOR_EDGES % 2 == 0) {
				_TIM_TIC = 0;
			}

			else {
				sensor_0_1 = _TIM_TIC;
				if(sensor_0_1 > 6) data |= 1;
				data = data << 1;
			}
		}
	}

}
