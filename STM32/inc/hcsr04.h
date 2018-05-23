#ifndef HCSR04_H_
#define HCSR04_H_

extern double d1;
extern int t;

void HCSR04_init(void);
void HCSR04_delay(int l);
int HCSR04_odczyt(GPIO_TypeDef* TRIG_GPIOx, uint16_t TRIG_Pin, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_Pin);

#endif /* HCSR04_H_ */
