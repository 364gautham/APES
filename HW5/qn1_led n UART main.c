
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
uint32_t g_ui32SysClock;
// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif
// Send a string to the UART.
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{   
 // Loop while there are more characters to send.
    while(ui32Count--)
    {
        // Write the next character to the UART.
         ROM_UARTCharPut(UART0_BASE, *pui8Buffer++);
    }
}


int main(void)
{
        
uint32_t sy_clock1;
sy_clock1 = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);

    // Enable the peripherals used by this example.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
        {
        }
     // Enable the GPIO port that is used for the on-board LED.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);      
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Enable processor interrupts.
    // ROM_IntMasterEnable();
    // Set GPIO A0 and A1 as UART pins.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Configure the UART for 115,200, 8-N-1 operation
    ROM_UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
    // Enable the UART interrupt.
    // ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    // Prompt for text to be entered.
    UARTSend((uint8_t *)"APES Project :7 April 2018 Gautham",34);
    UARTSend((uint8_t *)"\n",1);
    UARTSend((uint8_t *)"\r",1);
    int long  a = 0;
    char b[2];
    // Loop forever echoing data through the UART.

    while(1)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        ROM_SysCtlDelay(10000000);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
        a++;
        ltoa(a,b);
        UARTSend((uint8_t *)(b),2);
        UARTSend((uint8_t *)"\r",1);
        UARTSend((uint8_t *)"\n",1);
        ROM_SysCtlDelay(10000000);

    }
}
