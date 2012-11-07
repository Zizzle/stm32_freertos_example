/* Scheduler includes. */
#include "FreeRTOS.h"

/* Library includes. */
#include "stm32f10x.h"

#include "serial.h"
/*-----------------------------------------------------------*/

static uint32_t g_usart;

int comm_test(void)
{
        return ( USART_GetFlagStatus(g_usart, USART_FLAG_RXNE) == RESET ) ? 0 : 1;
}

char comm_get(void)
{
        while(USART_GetFlagStatus(g_usart, USART_FLAG_RXNE) == RESET) { ; }
        return (char)USART_ReceiveData(g_usart);
}

void comm_put(char d)
{
        while(USART_GetFlagStatus(g_usart, USART_FLAG_TXE) == RESET) { ; }
        USART_SendData(g_usart, (uint16_t)d);
}

void comm_puts(const char* s)
{
        char c;
        while ( ( c = *s++) != '\0' ) {
                comm_put(c);
        }
}

void USARTInit(uint16_t tx_pin, uint16_t rx_pin, uint32_t usart)
{
	GPIO_InitTypeDef GPIO_InitStructure;
        

	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	g_usart = usart; // keep track of which USART we are using

	//enable bus clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	if (usart == USART1)
	    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
	else
	    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);

	//Configure USART1 Tx (PA.02) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = tx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//Configure USART1 Rx (PA.03) as input floating
	GPIO_InitStructure.GPIO_Pin = rx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 /* USART1 and USART2 configuration ------------------------------------------------------*/
        /* USART and USART2 configured as follow:
         - BaudRate = 115200 baud
         - Word Length = 8 Bits
         - One Stop Bit
         - No parity
         - Hardware flow control disabled (RTS and CTS signals)
         - Receive and transmit enabled
        */
        USART_InitStructure.USART_BaudRate = 115200;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

        /* Configure USART2 */
        USART_Init(g_usart, &USART_InitStructure);
        
        /* Enable the USART1 */
        USART_Cmd(g_usart, ENABLE);
        
        comm_puts("TEST\r\n");
}




