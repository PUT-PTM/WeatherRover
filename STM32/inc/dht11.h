#ifndef _DHT11_H
#define _DHT11_H

#include "integer.h"
/**
 * DHT11	STM32F4xx	Description
 * ---------------------------------
 * OUT		PA1			OneWire
 * VCC		3V			Power supply
 * GND		GND			Ground
 */

#define DHT11_SUCCESS         1
#define DHT11_ERROR_CHECKSUM  2
#define DHT11_ERROR_TIMEOUT   3
#define DHT11_ERROR_CONFIG    4

typedef struct DHT11{
	INT temperature;
	INT humidity;
	unsigned short t;
	unsigned short h;
	int status;
} DHT11;

void DHT11_Init();
void DHT11_TIM5_config();
void DHT11_GPIOA_output_config();
void DHT11_GPIOA_input_config();
void DHT11_delay();
void DHT11_delay(int time);
int DHT11_read_data();
void DHT11_send_to_bluetooth();
#endif
