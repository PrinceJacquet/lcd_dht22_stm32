/*********************************************************************************
 * \file   : DHT22.h															 *
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


#ifndef INC_DHT22_H_
#define INC_DHT22_H_


#include "main.h"


typedef struct {
	TIM_HandleTypeDef* _htim;
	uint32_t _tim_clock_freq;
	GPIO_TypeDef* _SENSOR_GPIO_Port;
	uint8_t _SENSOR_Pin;
	IRQn_Type _SENSOR_EXTI_IRQn;
} DHT22_InitTypedef;

/**
 * \brief : structure to store the sensor data. Use .is_data_valid to check if the data received match the checksum
 */
typedef struct {
	uint8_t check_sum;
	uint8_t is_data_valid;
	float RH;
	float T;
}DHT22_Data;

/**
 * \brief : sensor initialization. You have to configure a timer and enable interrupts and an external interrupt pin
 * \param _timer_clock_freq : depending on the timer's bus (APB1 or APB2), its clock value may be different from the main clock. Refer to the datasheet for additional information
 */
void DHT22_Init(TIM_HandleTypeDef* _htim, uint32_t _timer_clock_freq, GPIO_TypeDef* _SENSOR_GPIO_Port, uint8_t _SENSOR_Pin, IRQn_Type _SENSOR_EXTI_IRQn);
DHT22_Data DHT22_ReadData();


#endif /* INC_DHT22_H_ */
