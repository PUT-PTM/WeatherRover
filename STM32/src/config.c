#include "config.h"

void Init() {

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

void Init_bt() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_Cmd(USART3, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_EnableIRQ(USART3_IRQn);

}

void Send(char * tab) {
	while(*tab)
		Send_char(*tab++);
}

void Send_char(char c){
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, c);
}

void hc_sr04_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitTypeDef  Trigger;
    Trigger.GPIO_Pin = GPIO_Pin_4;
    Trigger.GPIO_Mode = GPIO_Mode_OUT;
    Trigger.GPIO_OType = GPIO_OType_PP;
    Trigger.GPIO_Speed = GPIO_Speed_100MHz;
    Trigger.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &Trigger);

    GPIO_InitTypeDef  Echo;
    Echo.GPIO_Pin = GPIO_Pin_5;
    Echo.GPIO_Mode = GPIO_Mode_IN;
    Echo.GPIO_OType = GPIO_OType_PP;
    Echo.GPIO_Speed = GPIO_Speed_100MHz;
    Echo.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &Echo);
}

void tim2_init(void)
{
    //timer wykorzystywany do odliczania czasu trwania sygnalow ECHO
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef Timer2;
    /* czestotliwosc przeladowania nas nie interesuje */
    Timer2.TIM_Period =999999;
    Timer2.TIM_Prescaler = 83;
    Timer2.TIM_ClockDivision = TIM_CKD_DIV1;
    Timer2.TIM_CounterMode =  TIM_CounterMode_Up;
    Timer2.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &Timer2);
}

void tim3_init(void)
{
    //wykorzystywany do generowania opoznien
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef Timer3;
     /* Przeladowanie nastepuje co 10us */
    Timer3.TIM_Period =70-1;
    Timer3.TIM_Prescaler = 12-1;
    Timer3.TIM_ClockDivision = TIM_CKD_DIV1;
    Timer3.TIM_CounterMode =  TIM_CounterMode_Up;
    Timer3.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &Timer3);
}

void delay(int l)
{
    // l=1 oznacza odczekanie 10 us
    int i;
    TIM3->CNT=0;
    TIM_Cmd(TIM3, ENABLE);
    for(i=0;i<l;i++)
    {
        while(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update))
        {
            TIM_ClearFlag(TIM3, TIM_FLAG_Update);
        }
    }
    TIM_Cmd(TIM3, DISABLE);
}

int odczyt(GPIO_TypeDef* TRIG_GPIOx, uint16_t TRIG_Pin, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_Pin)
{
    int tajm=0, timeout=10000;
    //upewniamy sie ze timer odlicza od 0
    TIM2->CNT=0;
    //Sygnal niski na trigger
    GPIO_ResetBits(TRIG_GPIOx, TRIG_Pin);
    //Sygnal wysoki na trigger
    GPIO_SetBits(TRIG_GPIOx, TRIG_Pin);
    //Opóznienie 10 us
    delay(1);
    //Sygnal niski na trigger
    GPIO_ResetBits(TRIG_GPIOx, TRIG_Pin);
    //czekanie na pojewienie sie sygnalu Echo, w razie braku zwraca -1
    while (GPIO_ReadInputDataBit(ECHO_GPIOx, ECHO_Pin) == Bit_RESET)
    {
        if(timeout--==0)
        {
            return -1;
        }
    }
    //w tym miejscu rozpoczynamy pomiar czasu trwania sygnalu echo
    TIM_Cmd(TIM2, ENABLE);
    //Petla while czeka do momentu kiedy sygnal bedzie 0
    while (GPIO_ReadInputDataBit(ECHO_GPIOx, ECHO_Pin) == Bit_SET);
    TIM_Cmd(TIM2, DISABLE);
    //pobranie czasu trwania stanu wysokiego
    tajm=TIM2->CNT;
    //obliczanie odleglosci w cm
    return tajm;
}
