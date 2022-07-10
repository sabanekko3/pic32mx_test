/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#include <stdio.h>

#define PBCLK		(CPU_CLOCK_FREQUENCY / 2)		// peripheral bus clock
#define SAMPLE_RATE	5000
#define CCLK_US		(PBCLK / 1000 / 1000)	// used for micro second delay
#define CCLK_MS		(PBCLK / 1000)	// used for milli second delay

//#define SPI_TEST
#define UART_TEST

void __delay_ms(uint32_t);
void SPI_call_back(uintptr_t contextHandle);



// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    uint16_t adc_val = 0;
    //uint8_t RX_spi;
    //uint8_t RX_count;
    
    SPI1_CallbackRegister(SPI_call_back,(uintptr_t)0);
    
    TMR2_Start();
    TMR3_Start();
    ADC_Enable();
    OCMP3_Enable ();

    printf("some string to UART1 or stdout");

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ();
        GPIO_PinWrite(LED1_PIN, 1);

        adc_val = ADC_ResultGet(ADC_RESULT_BUFFER_0);
        OCMP3_CompareSecondaryValueSet(adc_val<<6);
#ifdef UART_TEST
        printf("ADC val:%d\n",adc_val);
#endif
        
#ifdef SPI_TEST

#endif
        GPIO_PinWrite(LED2_PIN, 0);
        __delay_ms(200);
        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

void SPI_call_back(uintptr_t contextHandle){
    uint8_t RX_spi = 0;
    if( SPI1_ErrorGet() == SPI_SLAVE_ERROR_NONE ){
        GPIO_PinWrite(LED2_PIN, 1);
            
        SPI1_Read(&RX_spi,1);
        printf("SPI reseive data:%d\n",RX_spi);
    }
}

void __delay_ms(uint32_t d){   
	unsigned int delayCount, startTime;       

	startTime = _CP0_GET_COUNT();   
	delayCount = d * CCLK_MS;   
	while((_CP0_GET_COUNT() - startTime) < delayCount);   
}
/*******************************************************************************
 End of File
*/

