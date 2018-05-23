/*
 * TIM5, TIM4 - przerwania dla DHT11
 * TIM2 - timer wykorzystywany do odliczania czasu trwania sygnalow ECHO
 * TIM3 - wykorzystywany do generowania opoznien
 */

#ifndef _TIM4_H
#define _TIM4_H

void TIM4_interrupt_config();
void TIM_config(uint32_t RCC_APB1Periph, uint32_t TIM_Period,  uint16_t TIM_Prescaler, TIM_TypeDef* TIMx, FunctionalState NewState);


#endif
