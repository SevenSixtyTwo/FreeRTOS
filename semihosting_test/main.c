#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "semihosting.h"
#include "stm32f30x.h"

void Delay_ms(uint32_t nTime){
	uint32_t TimingDelay = nTime*5800;
	while(TimingDelay != 0) {
		--TimingDelay;
	}
}

void Port_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = 0xFF00;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

int main(void)
{
	unsigned char k;
	Port_Init();

    while(1){
    	// ascii '0' is 48
    	// ascii '8' is 56
    	k = SH_GetChar() - '0';
    	if(k <= 8){
    		GPIOE->ODR ^= 1<<(k+7);
    		SH_SendChar(k);
    	}
    }
}
