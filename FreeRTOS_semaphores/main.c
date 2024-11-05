#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_exti.h"
#include "stm32f30x_misc.h"
#include "stm32f30x_syscfg.h"

xSemaphoreHandle EX;
//xSemaphoreHandle ButtonSemaphore_ON;
//xSemaphoreHandle ButtonSemaphore_OFF;

void Port_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = 0xFF00;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//void ButtonScan(void *pvParameters){
//	while(1){
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
//			xSemaphoreGive(ButtonSemaphore_ON);
//		}else{
//			xSemaphoreGive(ButtonSemaphore_OFF);
//		}
//	}
//}

//void TaskLED_ON(void *pvParameters){
//	while(1){
//		xSemaphoreTake(ButtonSemaphore_ON, portMAX_DELAY);
//		GPIOE->ODR |= (1<<13);
//	}
//}

//void TaskLED_OFF(void *pvParameters){
//	while(1){
//		xSemaphoreTake(ButtonSemaphore_OFF, portMAX_DELAY);
//		GPIOE->ODR &= ~(1<<13);
//	}
//}

//void Test(void *pvParameters){
//	while(1){
//		GPIOE->ODR |= 1<<10;
//	}
//}

void ext(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Task_LED_SWITCH(void *pvParameters){
	while(1){
		xSemaphoreTake(EX, portMAX_DELAY);
		GPIOE->ODR ^= 1<<9;
	}
}

void EXTI0_IRQHandler(){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(EX, &xHigherPriorityTaskWoken);

	if(xHigherPriorityTaskWoken == pdTRUE){
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	EXTI_ClearFlag(EXTI_Line0);
}

int main(void)
{
	Port_Init();
	ext();

	vSemaphoreCreateBinary(EX);

	xSemaphoreTake(EX, 1);

	xTaskCreate(Task_LED_SWITCH,
			(signed char *) "LED_SWITCH",
			configMINIMAL_STACK_SIZE,
			NULL,
			2,
			(xTaskHandle *) NULL);

	vTaskStartScheduler();

    while(1)
    {
    }
}

// dumb functions
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
