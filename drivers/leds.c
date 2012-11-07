//-------------------------------------------------------------------------
// Author: Brad Goold
// Date:  7 Feb 2012
// Email Address: W0085400@umail.usq.edu.au
// 
// Purpose:
// Pre:
// Post:
// RCS $Date$
// RCS $Revision$
// RCS $Source$
// RCS $Log$
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Library includes. */
#include "stm32f10x.h"
#include "leds.h"
#include <stdio.h>
#include "ds1820.h"

static unsigned portSHORT usOutputValue = 0;

void vStartupLEDTask ( void *pvParameters );
void vLEDFlashTask( void *pvParameters );
 
/*-----------------------------------------------------------*/

void vLEDInit( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //D1 = PC6, D2 = PC7 , D3 = PD13, D4 = PD6
       
    GPIO_InitStructure.GPIO_Pin =  D2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( D2_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin =  D3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( D3_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin =  D4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( D3_PORT, &GPIO_InitStructure );

#if 0    
    xTaskCreate( vStartupLEDTask, 
                 ( signed portCHAR * ) "LED",
                 configMINIMAL_STACK_SIZE + 100,
                 NULL,
                 tskIDLE_PRIORITY+5,
                 NULL);
    
#endif    
}

void vStartupLEDTask ( void *pvParameters ) {
    vLEDSet(D2_PORT, D2_PIN, ON);
    vLEDSet(D3_PORT, D3_PIN, ON);
    vLEDSet(D4_PORT, D4_PIN, ON);
    
    vTaskDelay(500/portTICK_RATE_MS);
    
    
    vLEDSet(D2_PORT, D2_PIN, OFF);
    vLEDSet(D3_PORT, D3_PIN, OFF);
    vLEDSet(D4_PORT, D4_PIN, OFF);
    
    xTaskCreate( vLEDFlashTask, 
                 ( signed portCHAR * ) "LEDf",
                 configMINIMAL_STACK_SIZE ,
                 NULL,
                 tskIDLE_PRIORITY+2,
                 NULL);
    // printf("LEDStartup HWM = %d1\r\n", uxTaskGetStackHighWaterMark(NULL));
    
        
    vTaskDelete(NULL);
    
    for (;;)
    {
        //Should never get here
        vTaskPrioritySet(NULL, tskIDLE_PRIORITY);
        // printf("LcdStartup Still Running\r\n");
    }
}

void vLEDFlashTask( void *pvParameters )
{
    portTickType xLastExecutionTime = xTaskGetTickCount();
    for (;;)
    {

        vTaskDelayUntil( &xLastExecutionTime, 1000/portTICK_RATE_MS ); 
        vLEDToggle( D4_PORT, D4_PIN );
             
        taskYIELD();
    }
    
}

//-------------------------------------------------------------------------

void vLEDSet( GPIO_TypeDef *GPIO_PORT , uint16_t GPIO_Pin, unsigned portBASE_TYPE uxValue)
{
    vTaskSuspendAll();
    
    if (uxValue)
        GPIO_WriteBit( GPIO_PORT, GPIO_Pin, 1 );       
    
    else 
        GPIO_WriteBit( GPIO_PORT, GPIO_Pin, 0 );
    
    
    xTaskResumeAll();
}

void vLEDToggle( GPIO_TypeDef *GPIO_PORT , uint16_t GPIO_Pin)
{
    uint16_t uxVal = GPIO_ReadOutputDataBit(GPIO_PORT,GPIO_Pin);
   
    if (uxVal) 
        vLEDSet(GPIO_PORT,GPIO_Pin, 0);
    else
        vLEDSet(GPIO_PORT,GPIO_Pin, 1);
   
}
