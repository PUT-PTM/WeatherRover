#ifndef CONFIG_H_
#define CONFIG_H_

void Init_bt();
void Init();
void Send(char * tab); //send data
void Send_char(char c);
void hc_sr04_init(void);
void tim2_init(void);
void tim3_init(void);
void delay(int l);
int odczyt(GPIO_TypeDef* TRIG_GPIOx, uint16_t TRIG_Pin, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_Pin);

#endif /* CONFIG_H_ */
