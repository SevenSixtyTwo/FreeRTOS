#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <task.h>
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x.h"

int flag = 0;
int button_flag = 0;
//int i = 0;
//int a_1 = 0, a_2 = 0, a_3 = 0, a_4 = 0, a_5 = 0, a_6 = 0, a_7 = 0, a_8 = 0;
//double t = 0;

//uint16_t m1[2] = {100, GPIO_Pin_9};
//uint16_t m2[2] = {250, GPIO_Pin_13};
//uint16_t m3[2] = {200, GPIO_Pin_15};
//uint16_t m4[2] = {150, GPIO_Pin_12};
//uint16_t m5[2] = {250, GPIO_Pin_10};

// x, y, z - LED numbers
//struct param {
//	unsigned char x;
//	unsigned char y;
//	unsigned char z;
//	unsigned int t;
//};

//struct param args = {8, 9, 10, 300};

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

void ButtonScan(void){
	while(1){
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) && (button_flag == 0)){
			vTaskDelay(20);
			if(flag){
				flag = 0;
			}else{
				flag = 1;
			}
			button_flag = 1;
		}else if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			button_flag = 0;
		}
	}
}

void LED(void){
	while(1){
		if(flag){
			GPIOE->ODR |= 1<<9;
		}else{
			GPIOE->ODR &= ~(1<<9);
		}
	}
}

void Test(void *pvParameters){
	while(1){
		GPIOE->ODR |= 1<<10;
	}
}

//void PWM(void *pvParameters){
//	while(1){
//		GPIOE->ODR |= 0xFF00;
//		while(i < 100){
//			if(i == a_1) GPIOE->ODR &= ~(1<<8);
//			if(i == a_2) GPIOE->ODR &= ~(1<<9);
//			if(i == a_3) GPIOE->ODR &= ~(1<<10);
//			if(i == a_4) GPIOE->ODR &= ~(1<<11);
//			if(i == a_5) GPIOE->ODR &= ~(1<<12);
//			if(i == a_6) GPIOE->ODR &= ~(1<<13);
//			if(i == a_7) GPIOE->ODR &= ~(1<<14);
//			if(i == a_8) GPIOE->ODR &= ~(1<<15);
//			i++;
//		}
//		i = 0;
//	}
//}

//void PARAM(void *pvParameters){
//	while(1){
//		a_1 = 50 * (1 +sin(6.28*1*t + 0.00));
//		a_2 = 50 * (1 +sin(6.28*1*t + 0.79));
//		a_3 = 50 * (1 +sin(6.28*1*t + 1.58));
//		a_4 = 50 * (1 +sin(6.28*1*t + 2.37));
//		a_5 = 50 * (1 +sin(6.28*1*t + 3.14));
//		a_6 = 50 * (1 +sin(6.28*1*t + 3.95));
//		a_7 = 50 * (1 +sin(6.28*1*t + 4.74));
//		a_8 = 50 * (1 +sin(6.28*1*t + 5.53));
//
//		t += 0.01;
//		if(t >= 1) t = 0;
//
//		vTaskDelay(10);
//	}
//}

//void TaskLED(void *pvParameters){
//	while(1){
//		GPIO_SetBits(GPIOE, *((uint16_t*)pvParameters+1));
//		vTaskDelay(*((uint16_t*)pvParameters));
//		GPIO_ResetBits(GPIOE, *((uint16_t*)pvParameters+1));
//		vTaskDelay(*((uint16_t*)pvParameters));
//	}
//}

//void Task(void *pvParameters){
//	while(1){
//		GPIOE->ODR ^= (1 << ((struct param*)pvParameters)->x)|
//					(1 << ((struct param*)pvParameters)->y)|
//					(1 << ((struct param*)pvParameters)->z);
//		vTaskDelay(((struct param*)pvParameters)->t);
//	}
//}

int main(void)
{
	Port_Init();

	xTaskCreate(LED,
			(signed char *) "LED",
			configMINIMAL_STACK_SIZE,
			NULL,
			2,
			(xTaskHandle *) NULL);

	xTaskCreate(ButtonScan,
			(signed char *) "ButtonScan",
			configMINIMAL_STACK_SIZE,
			NULL,
			2,
			(xTaskHandle *) NULL);

	xTaskCreate(Test,
			(signed char *) "Test",
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
