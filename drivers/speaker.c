//-------------------------------------------------------------------------
// Author: Brad Goold
// Date: 15 Feb 2012
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
// Included Libraries
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
#include <stdio.h> 
#include "FreeRTOS.h" 
#include "stm32f10x.h"
#include "queue.h"
#include "speaker.h"
#define BEEP_QUEUE_SIZE 1
//-------------------------------------------------------------------------
xQueueHandle xBeepQueue;

void speaker_init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOE, &GPIO_InitStructure );

}

static void beep( unsigned portBASE_TYPE len){
    unsigned portBASE_TYPE jj;

    for (jj= 0; jj < len; jj++)
    {
        GPIO_WriteBit( GPIOE, GPIO_Pin_0,1 );           
        vTaskDelay(1);
        GPIO_WriteBit( GPIOE, GPIO_Pin_0,0 );
        vTaskDelay(1);
    }
}

void vBeepTask( void * pvParameters) 
{
    xBeepQueue = xQueueCreate(BEEP_QUEUE_SIZE, 1);
    char x;
    unsigned portBASE_TYPE ii,jj, uxVal;
    static unsigned portBASE_TYPE last;
    
    unsigned portBASE_TYPE xStatus = 0;
   
    for (;;)
    {
        
        if(xQueueReceive(xBeepQueue, &x, portMAX_DELAY))
        {
            switch (x){
            case TOUCH_BEEP:
                beep(5);
                break;
            case ALARM_BEEP:
                beep(20);
                break;
            default:
                break;
            }
            
            
            
        }
    }
}

