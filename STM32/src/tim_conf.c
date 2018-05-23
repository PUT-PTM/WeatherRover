#include "stm32f4_discovery.h"
#include "misc.h"

#include "tim_conf.h"

void TIM_config(uint32_t RCC_APB1Periph, uint32_t TIM_Period,  uint16_t TIM_Prescaler, TIM_TypeDef* TIMx, FunctionalState NewState){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = TIM_Period;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	TIM_Cmd(TIMx, NewState);
}
void TIM4_interrupt_config()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}
