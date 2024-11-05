#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <math.h>

void _delay(uint32_t i){
	uint32_t j = 0;
	i *= 7.2;
	for(j = 0; j <= i; j++){};
}

void Port_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = 0xFF00;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void PWM(uint8_t *a){
	int i = 0;

	GPIOE->ODR |= 0xFF00;

	while(i<100){
		if(i == *(a + 0)) GPIOE->ODR &= ~(1<<8);
		if(i == *(a + 1)) GPIOE->ODR &= ~(1<<9);
		if(i == *(a + 2)) GPIOE->ODR &= ~(1<<10);
		if(i == *(a + 3)) GPIOE->ODR &= ~(1<<11);
		if(i == *(a + 4)) GPIOE->ODR &= ~(1<<12);
		if(i == *(a + 5)) GPIOE->ODR &= ~(1<<13);
		if(i == *(a + 6)) GPIOE->ODR &= ~(1<<14);
		if(i == *(a + 7)) GPIOE->ODR &= ~(1<<15);

		i++;
	}
	i = 0;
}

int main(void)
{
	uint8_t a[8];
	uint8_t k;
	double t = 0;
	Port_Init();

    while(1)
    {
    	for(k = 0; k < 8; ++k){
    		a[k] = 50 * (1 + sin(6.28*1*t + k*6.28/5));
    	}
    	PWM(a);
    	t+=0.0005;
    	if(t >= 1) t=0;
    }
}
