#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x.h"

// Exercise 6 from presentation about I/O ports
// Simulate counter
// LEDs display a binary of the number of presses on the button

struct param{
	uint8_t counter;
	uint8_t flag;
	uint8_t button_flag;
};

void Port_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = 0xFF00;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void LED(void *pvParameters){
	while(1){
		GPIOE->ODR = ((struct param*)pvParameters)->counter<<8;
	}
}

void ClickCounter(void *pvParameters){
	while(1){
		if(((struct param*)pvParameters)->flag){
			((struct param*)pvParameters)->counter += 1;
			((struct param*)pvParameters)->flag = 0;
		}
	}
}

void ButtonScan(void *pvParameters){
	while(1){
		if(!((struct param*)pvParameters)->button_flag && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			vTaskDelay(20);
			((struct param*)pvParameters)->flag = 1;
			((struct param*)pvParameters)->button_flag = 1;
		}else if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			((struct param*)pvParameters)->button_flag = 0;
		}
	}
}

int main(void)
{
	Port_Init();

	struct param args = {0, 0, 0};

	struct param* pargs = &args;

	xTaskCreate(LED,
			(signed char*) "LED",
			configMINIMAL_STACK_SIZE,
			pargs,
			2,
			(xTaskHandle*) NULL);

	xTaskCreate(ClickCounter,
			(signed char*) "ClickCounter",
			configMINIMAL_STACK_SIZE,
			pargs,
			2,
			(xTaskHandle*) NULL);

	xTaskCreate(ButtonScan,
			(signed char*) "ButtonScan",
			configMINIMAL_STACK_SIZE,
			pargs,
			2,
			(xTaskHandle*) NULL);

	vTaskStartScheduler();
    while(1)
    {
    }
}

// dummy functions
void vApplicationIdleHook(void){}
void vApplicationMallocFailedHook(void){
	for(;;);
}
void vApplicationStackOverflowHook(xTaskHandle pxTask, char *pcTaskName){
	(void)pcTaskName;
	(void)pxTask;
	for(;;);
}
void vApplicationTickHook(void){}
