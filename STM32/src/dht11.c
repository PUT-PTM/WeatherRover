#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "dht11.h"
#include "config.h"

DHT11 measurement;

void DHT11_init(){
	DHT11_TIM5_config();
	TIM4_run_config();
}

void DHT11_TIM5_config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 84000000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 84;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM5, ENABLE);
}

void DHT11_GPIOA_output_config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DHT11_GPIOA_input_config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DHT11_delay(int time)
{
	TIM5->CNT = 0;
	while ((TIM5->CNT) <= time);
}

int DHT11_read_data()
{
	u8 bits [5] = { 0, 0, 0, 0, 0 };
	u8 index = 0;

	DHT11_GPIOA_output_config();
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	DHT11_delay(18000);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	DHT11_delay(40);
	DHT11_GPIOA_input_config();

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
	{
		return DHT11_ERROR_CONFIG;
	}

	DHT11_delay(80);

	if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
	{
		return DHT11_ERROR_CONFIG;
	}

	DHT11_delay(80);

	// 40 bajtow do odczytu
	for (int i = 1; i <= 40; i++ )
	{
		// oczekiwanie na stan wysoki
		while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
		DHT11_delay(35);

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
		{
			// jezeli po 35ms jest stan wysoki to logiczna jedynka
			bits[index] <<= 1;
			bits[index] |= 1;
		}
		else
		{
			// jezeli nie to 0
			bits[index] <<= 1;
		}

		// co 8 bitow zwiekszenie licznika tablicy
		if ((i % 8) == 0)
		{
			index++;
		}

		// oczekiwanie na stan niski
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
	}

	// sprawdzenie sumy kontrolnej
	if (!(bits[0] + bits[1] + bits[2] + bits[3] == bits[4]))
	{
		return DHT11_ERROR_CHECKSUM;
	}

	measurement.humidity = bits[0];
	DHT11_delay(35000);
	measurement.temperature = bits[2];

	return DHT11_SUCCESS;
}

void DHT11_send_to_bluetooth(){
	if(measurement.humidity >= 10){
		Send_char(measurement.humidity/10+'0');
		Send_char(measurement.humidity%10+'0');
	} else{
		Send_char('0');
		Send_char(measurement.humidity+'0');
	}
	if(measurement.temperature >=10){
		Send_char('+');
		Send_char(measurement.temperature/10+'0');
		Send_char(measurement.temperature%10+'0');
	} else if(measurement.temperature < 10 && measurement.temperature >= 0){
		Send_char('+');
		Send_char('0');
		Send_char(measurement.humidity+'0');
	} else if(measurement.temperature < 0 && measurement.temperature >= -9){
		Send_char('-');
		Send_char(measurement.temperature%10+'0');
		Send_char(measurement.temperature/10+'0');
	} else{
		Send_char('-');
		Send_char(measurement.temperature%10+'0');
		Send_char(measurement.temperature/10+'0');
	}

	Send("\r\n");
}
