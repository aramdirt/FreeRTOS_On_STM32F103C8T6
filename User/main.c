#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_CLI.h"
#include "serial.h"
#include "UARTCommandConsole.h"
#include "Sample-CLI-commands.h"

#define UART_CONSOLE_STACK     1000

#define UART_TASK_PRIO         1
#define START_TASK_PRIO        2 
#define TASK1_TASK_PRIO        4 /* Let Task 1 run first */
#define TASK2_TASK_PRIO        3

#define START_STK_SIZE         128
#define TASK1_STK_SIZE         128
#define TASK2_STK_SIZE         128

TaskHandle_t StartTask_Handler;
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;

/* The task functions prototype*/
void start_task(void *pvParameters);
void task1_task(void *pvParameters);
void task2_task(void *pvParameters);

/* Task parameter to be sent to the task function */
const char *pvTask1  = "Task1 is running.";
const char *pvTask2  = "Task2 is running.";

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

int main(void)
{
    /* Board initializations */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* Register sample commands */
    vRegisterSampleCLICommands();

    /* Start UART1 */
    vUARTCommandConsoleStart(UART_CONSOLE_STACK, UART_TASK_PRIO);
    

#if 0
    xComPortHandle xPort;
    xPort = xSerialPortInitMinimal( 115200, 200 );
    /* Create start task. */
    xTaskCreate((TaskFunction_t )start_task,
            (const char*    )"start_task",
            (uint16_t       )START_STK_SIZE,
            (void*          )NULL,
            (UBaseType_t    )START_TASK_PRIO,
            (TaskHandle_t*  )&StartTask_Handler);

    /* Start the scheduler so our tasks start executing. */
#endif
    vTaskStartScheduler();

    while(1)
    {

    }
}

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();

    /* Create task1. */
    xTaskCreate((TaskFunction_t )task1_task,            /* Pointer to the function that implements the task. */
                (const char*    )"task1",               /* Text name for the task.  This is to facilitate debugging only. */
                (uint16_t       )TASK1_STK_SIZE,        /* Stack depth in words. */
                (void*          )pvTask1,
                (UBaseType_t    )TASK1_TASK_PRIO,       /* This task will run at priority 1. */
                (TaskHandle_t*  )&Task1Task_Handler);

    /* Create task2. */
    xTaskCreate((TaskFunction_t )task2_task,            /* Pointer to the function that implements the task. */
                (const char*    )"task2",               /* Text name for the task.  This is to facilitate debugging only. */
                (uint16_t       )TASK2_STK_SIZE,        /* Stack depth in words. */
                (void*          )pvTask2,
                (UBaseType_t    )TASK2_TASK_PRIO,       /* This task will run at priority 1. */
                (TaskHandle_t*  )&Task2Task_Handler);

    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();
}

void task1_task(void *pvParameters)
{
    char *pcTaskName = (char *)pvParameters;
    for(;;)
    {
        /* Print out the name of this task. */
        vSerialPutString(0, ( signed char * ) pcTaskName, ( unsigned short ) strlen( pcTaskName ));
        portBASE_TYPE task1Priority = uxTaskPriorityGet(NULL);
        vTaskPrioritySet(Task2Task_Handler, task1Priority+1);

        vTaskDelay(500000/configTICK_RATE_HZ);
    }
}

void task2_task(void *pvParameters)
{
    char *pcTaskName = (char *)pvParameters;
    for(;;)
    {
        /* Print out the name of this task. */
        vSerialPutString(0, ( signed char * ) pcTaskName, ( unsigned short ) strlen( pcTaskName ));
        portBASE_TYPE task2Priority = uxTaskPriorityGet(NULL);
        vTaskPrioritySet(NULL, task2Priority-2);

        vTaskDelay(200000/configTICK_RATE_HZ);
    }
}
