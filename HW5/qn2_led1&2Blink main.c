/*
* Referenced from akobyl/TM4C129_FreeRTOS_Demo
*/

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"

#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "gpio.h"
#include "inc/hw_memmap.h"


// Task  declarations
void LED1(void *pvParameters);
void LED2(void *pvParameters);

// Val declared to blink LED.
uint8_t val=1;
uint8_t val1 = 1;

void timer1_callback(TimerHandle_t xTimer )
{
    // Value changed for blinking
    val =10;
}


void timer2_callback(TimerHandle_t xTimer )
{
    // Value changed for blinking
    val1 =20;
}

int main(void)
{
    // Initialize system clock to 120 MHz
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins LED Tasks
    PinoutSet(false, false);

    // Create demo tasks
    xTaskCreate(LED1, "LED_Task1",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(LED2, "LED_Task2",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();
    return 0;
}



void LED1(void *pvParameters)
{
    TimerHandle_t timer_task1;
    timer_task1 = xTimerCreate("Timer1",pdMS_TO_TICKS(250),pdTRUE,0,timer1_callback);
    if(timer_task1 == NULL)
         return 0;
    xTimerStart(timer_task1,0);

    for (;;)
    {
        // Turn off LED 1
        if(val==1){
            GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, 0);
        }

        // turn on LED1
        if(val == 10){
            GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, CLP_D1_PIN);
            val =1;
        }

    }
}
void LED2(void *pvParameters)
{
    TimerHandle_t timer_task2;
    timer_task2 = xTimerCreate("Timer2",pdMS_TO_TICKS(500),pdTRUE,0,timer2_callback);
    if(timer_task2 == NULL)
         return 0;
    xTimerStart(timer_task2,0);

    for (;;)
    {
        //TURN OFF led 4
        if(val1==1)
            GPIOPinWrite(CLP_D4_PORT, CLP_D4_PIN, 0);

        // turn on led 4
        if(val1 == 20){
            GPIOPinWrite(CLP_D4_PORT, CLP_D4_PIN, CLP_D4_PIN);
            val1 =1;
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
