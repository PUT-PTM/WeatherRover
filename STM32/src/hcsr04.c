#include "stm32f4_discovery.h"
#include "hcsr04.h"

double d1=0;
int t;

void HCSR04_init(void)
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

void HCSR04_delay(int l)
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

int HCSR04_odczyt(GPIO_TypeDef* TRIG_GPIOx, uint16_t TRIG_Pin, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_Pin)
{
    int tajm=0, timeout=10000;
    //upewniamy sie ze timer odlicza od 0
    TIM2->CNT=0;
    //Sygnal niski na trigger
    GPIO_ResetBits(TRIG_GPIOx, TRIG_Pin);
    //Sygnal wysoki na trigger
    GPIO_SetBits(TRIG_GPIOx, TRIG_Pin);
    //Opóznienie 10 us
    HCSR04_delay(1);
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
