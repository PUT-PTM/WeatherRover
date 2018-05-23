#include "stm32f4_discovery.h"

#include "dht11.h"
#include "sd_card.h"
#include "hcsr04.h"
#include "bluetooth.h"
#include "tim_conf.h"
#include "L298N.h"

int main(void) {
	TIM_config(RCC_APB1Periph_TIM5, 83999999, 84, TIM5, ENABLE);
	TIM_config(RCC_APB1Periph_TIM4, 49999, 16799, TIM4, ENABLE);
	TIM_config(RCC_APB1Periph_TIM2, 999999, 83, TIM2, DISABLE);
	TIM_config(RCC_APB1Periph_TIM3, 69, 11, TIM3, DISABLE);
	TIM4_interrupt_config();

	L298N_init();
	SD_init();
	BT_init();
	HCSR04_init();

	for (;;) {
        t=HCSR04_odczyt(GPIOE, GPIO_Pin_4, GPIOE, GPIO_Pin_5);
        if(t!=-1){
            d1=t*0.01724137931;
        }
        HCSR04_delay(1000);
        if(d1<20){
        	L298N_stop=1;
			GPIO_ResetBits(GPIOD,GPIO_Pin_14 | GPIO_Pin_15);
        }
        else{
        	L298N_stop=0;
        }
	}
}

void USART3_IRQHandler(void) {
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

		L298N_driveControl();

		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		measurement.status = DHT11_read_data();
		DHT11_send_to_bluetooth();
		SD_write_measurement("plik.txt");

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
