#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "dht11.h"


double d1=0;
int t;
int stop=0;

int counter = 0;
int status;

int main(void) {
	DHT11_Init();
	Init();
	Init_bt();
	hc_sr04_init();
	tim2_init();
	tim3_init();
	for (;;) {
        t=odczyt(GPIOE, GPIO_Pin_4, GPIOE, GPIO_Pin_5);
        if(t!=-1){
            d1=t*0.01724137931;
        }
        delay(1000);
        if(d1<20){
        	stop=1;
			GPIO_ResetBits(GPIOD,GPIO_Pin_14 | GPIO_Pin_15);
        }
        else{
        	stop=0;
        }
	}
}

void USART3_IRQHandler(void) {

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
			;
		char direction = USART3->DR;
		switch (direction) {
		case 'u':
			GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
			if(stop==0){
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
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			;
	}
}
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		status = DHT11_read_data();
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
