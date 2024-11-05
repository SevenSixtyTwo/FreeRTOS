#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x.h"

// Exercise 1
// 1 LED blinks with 1Hz frequency
// Pressing the button will stop and then start blinking again

void _delay(uint32_t f){
	uint32_t i = 0;
	f *= 7.2;

	for(i = 0; i <= f; ++i){};
}

int main(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	uint8_t f = 1;

    while(1)
    {
    	if(f && !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			GPIO_SetBits(GPIOE, GPIO_Pin_8);
			_delay(50000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);
			_delay(50000);
    	}

    	if(!f && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
    		_delay(50000);
    		if(!f && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
				f = 1;
			}
    		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){};
    	}

    	if(f && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			_delay(50000);
			if(f && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
				f = 0;
			}
    		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){};
		}
	}
}
