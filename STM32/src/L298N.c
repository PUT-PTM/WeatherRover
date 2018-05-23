#include "stm32f4_discovery.h"
#include "L298N.h"

int L298N_stop= 0;

void L298N_init(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
			| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

void L298N_driveControl(){
	char direction = USART3->DR;
	switch (direction) {
	case 'u':
		GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		if(L298N_stop==0){
			GPIO_SetBits(GPIOD, GPIO_Pin_14 | GPIO_Pin_15);
		}
		break;
	case 'd':
		GPIO_ResetBits(GPIOD,
		GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		GPIO_SetBits(GPIOD, GPIO_Pin_12| GPIO_Pin_13);
		break;
	case 'l':
		GPIO_ResetBits(GPIOD,
		GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		GPIO_SetBits(GPIOD, GPIO_Pin_14|GPIO_Pin_13);
		break;
	case 'r':
		GPIO_ResetBits(GPIOD,
		GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		GPIO_SetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_15);
		break;
	case 's':
		GPIO_ResetBits(GPIOD,
		GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		break;
	}
}
