/*
* Referenced from akobyl/TM4C129_FreeRTOS_Demo
*/

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "gpio.h"
#include "inc/hw_memmap.h"


// Demo Task declarations
void LED1(void *pvParameters);
void STRING(void *pvParameters);
void NOTIFY(void *pvParameters);

#define QUEUE_LENGTH 5
#define QUEUE_ITEM_SIZE sizeof(tick_count)
TaskHandle_t task3;
QueueHandle_t Queue;
TickType_t tick_count;
uint8_t val=10;

void timer1_callback(TimerHandle_t xTimer )
{
    BaseType_t tnotify;
    tnotify = xTaskNotify(task3, 0x04, eSetBits );
    if(tnotify == pdFAIL)
        return 0;
}


void timer2_callback(TimerHandle_t xTimer )
{
    BaseType_t tnotify;
    tnotify = xTaskNotify(task3, 0x02, eSetBits );
    if(tnotify == pdFAIL)
        return 0;
    tick_count=xTaskGetTickCount();
    BaseType_t queSend_ret;
    queSend_ret = xQueueSend(Queue,&tick_count, portMAX_DELAY );
    if(queSend_ret == errQUEUE_FULL)
        return 0;

}

// Main function
int main(void)
{
    // Initialize system clock to 120 MHz
    uint32_t output_clock_rate_hz;

    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    // Create tasks
    xTaskCreate(LED1, "LED_Task1",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(STRING, "StringQue_Task2",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(NOTIFY, "Notify_othertasks",
                configMINIMAL_STACK_SIZE, NULL, 1,&task3);

    /* Create the queue, storing the returned handle in the xQueue variable. */
    Queue = xQueueCreate( QUEUE_LENGTH, QUEUE_ITEM_SIZE );
    if( Queue == NULL )
    {
        return 0;
    }
    vTaskStartScheduler();
    return 0;
}



void LED1(void *pvParameters)
{
    TimerHandle_t timer_task1;

    timer_task1 = xTimerCreate("Timer1",pdMS_TO_TICKS(500),pdTRUE,0,timer1_callback);
    if(timer_task1 == NULL)
         return 0;

    xTimerStart(timer_task1,0);

    for (;;)
    {

        // Turn off LED 1
          if(val == 1){
              GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, 0);
              val = 10;
          }


    }
}
void STRING(void *pvParameters)
{
    TimerHandle_t timer_task2;
    timer_task2 = xTimerCreate("Timer2",pdMS_TO_TICKS(250),pdTRUE,0,timer2_callback);
    if(timer_task2 == NULL)
         return 0;
    xTimerStart(timer_task2,0);

    for (;;)
    {
        //TURN OFF led 4
        /*if(val1==1)
            GPIOPinWrite(CLP_D4_PORT, CLP_D4_PIN, 0);

        // turn on led 4
        if(val1 == 20){
            GPIOPinWrite(CLP_D4_PORT, CLP_D4_PIN, CLP_D4_PIN);
            val1 =1;
        }*/


    }
}

void NOTIFY(void *pvParameters)
{
    BaseType_t n_wait ,qReceive_tick;
    uint32_t val_compare_task;
    TickType_t tick_val;
    char buffer[100];
    UARTStdioConfig(0, 115200, SYSTEM_CLOCK);
    for(;;){
        n_wait = xTaskNotifyWait(0x00,0xffffffff,&val_compare_task,portMAX_DELAY);
        if(n_wait == pdFALSE)
            return 0;
        if((val_compare_task & 0x04) != 0)
        {
            if(val == 10){
                GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, CLP_D1_PIN);
                val = 1;
            }
        }
        if((val_compare_task & 0x02) != 0)
        {
            qReceive_tick=xQueueReceive(Queue, &tick_val, portMAX_DELAY);
            if(qReceive_tick == errQUEUE_EMPTY)
                return 0;
            //UARTprintf("Current TICK Count :");
            sprintf(buffer,"Current TICK Count :%d",tick_val);
            UARTprintf(buffer);
            UARTprintf("\n");
        }
    }
}


/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}

