//-------------------------------------------------------------------------
// Author: Brad Goold
// Date: 16 Feb 2012
// Email Address: W0085400@umail.usq.edu.au
// 
// Purpose:
// Pre:
// Post:
//------------------------------------------------------------


//-------------------------------------------------------------------------
// Included Libraries
//-------------------------------------------------------------------------
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h" 
/* Library includes. */
#include "stm32f10x.h"
//#include "timer.h"
//-------------------------------------------------------------------------
/* The set frequency of the interrupt.  Deviations from this are measured as
   the jitter. */

#define timerINTERRUPT_FREQUENCY	( ( unsigned portSHORT ) 5000 )

/* The expected time between each of the timer interrupts - if the jitter was
   zero. */
#define timerEXPECTED_DIFFERENCE_VALUE (configCPU_CLOCK_HZ / timerINTERRUPT_FREQUENCY )

/* The highest available interrupt priority. */
#define timerHIGHEST_PRIORITY			( 0 )

/* Misc defines. */
#define timerMAX_32BIT_VALUE   		( 0xffffffffUL )
#define timerTIMER_1_COUNT_VALUE (*(( unsigned long * )(TIMER1_BASE + 0x48)))

/* The number of interrupts to pass before we start looking at the jitter. */
#define timerSETTLE_TIME       		5

/*-----------------------------------------------------------*/

/*
 * Configures the two timers used to perform the test.
 */
void vSetupTimerTest( void );

/* Interrupt handler in which the jitter is measured. */
void TIM2_IRQHandler( void );

/* Stores the value of the maximum recorded jitter between interrupts. */
volatile unsigned portSHORT usMaxJitter = 0;
void vSetupTimerTest( void )
{
    unsigned long ulFrequency;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
/* Enable timer clocks */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
     
/* Initialise data. */
    TIM_DeInit( TIM3 );
    TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// Alt Function - Push Pull
    GPIO_Init( GPIOC, &GPIO_InitStructure );
    GPIO_PinRemapConfig( GPIO_FullRemap_TIM3, ENABLE );// Map TIM3_CH3 to
                                                       // GPIOC.Pin8  
    
    //Period 10000, Prescaler 50, pulse = 40 gives 111Hz with 5us
    //Pulse width....  
    TIM_TimeBaseStructure.TIM_Period = 10000; 
    TIM_TimeBaseStructure.TIM_Prescaler = 50; //clock prescaler
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );
    TIM_ARRPreloadConfig( TIM3, ENABLE );
    
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit( &TIM_OCInitStruct );
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    // Initial duty cycle equals 0%. Value can range from zero to 1000.
    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OC3Init( TIM3, &TIM_OCInitStruct );
  
    
      
    /* Enable TIM2 IT.  TIM3 does not generate an interrupt. */
    // NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = timerHIGHEST_PRIORITY;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init( &NVIC_InitStructure );	
    // TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );
    
    /* Finally, enable both timers. */
    /* TIM_Cmd( TIM3, ENABLE ); */
 
  

}



/*-----------------------------------------------------------*/
char buf[50];
void TIM3_IRQHandler( void )
{
    unsigned portBASE_TYPE xStatus, xHPTW = pdFALSE;

    //
//    printf("%u\r\n", TIM2->CNT);
    //
    TIM_ClearITPendingBit( TIM3, TIM_IT_Update );
    
}

void vTimerSetupTask( void * pvParameters)
{
    
    vSetupTimerTest();
    

    for (;;)
        vTaskDelete(NULL);
    // sprintf(buf, "in interrupt at time = %u\r\n\0", TIM2->CNT );
    //xStatus = xQueueSendToBackFromISR( xConsoleQueue, &buf, &xHPTW );
}
