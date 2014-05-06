#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "lcd.h"
#include "semphr.h"
#include "stm32f10x.h"


// Doco for our LCD driver is here:
// http://www.displayfuture.com/engineering/specs/controller/LGDP4532.pdf

//---------------------------------------------------------------------/
//               Static Functions and prototypes
//---------------------------------------------------------------------/
static void display_ON(void);
static void display_OFF(void);
static void gamma_SET(void);
static void lcd_data_bus_test(void);
static void lcd_gram_test(void);
static void lcd_port_init(void);
static void power_SET(void);

//---------------------------------------------------------------------/
//                       INIT
//---------------------------------------------------------------------/
void lcd_init(void)
{
    xLcdSemaphore = xSemaphoreCreateMutex();

    lcd_port_init(); //initialise IO Registers

    /* deviceid check */    
    deviceid = read_reg(0x00); 
    if(deviceid != 0x4532)
    {
        printf("Invalid LCD ID:%08X\r\n",deviceid);
        printf("Please check you hardware and configure.");
        while(1);
    }
    else
    {
        printf("\r\nLCD Device ID : %04X ",deviceid);
    }
    
    //SET UP//
    power_SET(); //Set up the power Registers
    gamma_SET(); //Set up the Gamma Registers

#if 1
    //TEST//
    display_OFF(); //Switch off the display for tests
    lcd_data_bus_test();
    lcd_gram_test(); 
    display_ON();  //Switch on the display
#endif

#if 1
    write_reg(0x0003,0x1018);//Entry Mode BGR, Horizontal, then vertical

    //Eye Candy//
    /*
    Delay(10);      
    lcd_clear( 0xA0FF );
    Delay(100);      
    lcd_clear( Black );
    Delay(100);
    lcd_clear( 0xA0FF );
    Delay(100);
    lcd_clear( Black );

    Delay(1000);     
    */
#endif
    printf("Lcd_init done\r\n");
}

//---------------------------------------------------------------------/
//                       FMSC Setup
//---------------------------------------------------------------------/
static void LCD_FSMCConfig(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;

    /*-- FSMC Configuration ----------------------------------------------*/
    p.FSMC_AddressSetupTime = 2;             /* ��ַ����ʱ��  */
    p.FSMC_AddressHoldTime = 1;              /* ��ַ����ʱ��  */
    p.FSMC_DataSetupTime = 3;                /* ��ݽ���ʱ��  */
    p.FSMC_BusTurnAroundDuration = 0;        /* ���߷�תʱ��  */
    p.FSMC_CLKDivision = 0;                  /* ʱ�ӷ�Ƶ      */
    p.FSMC_DataLatency = 0;                  /* ��ݱ���ʱ��  */
    p.FSMC_AccessMode = FSMC_AccessMode_A;   /* FSMC ����ģʽ */

    /* Color LCD configuration ------------------------------------
       LCD configured as follow:
       - Data/Address MUX = Disable
       - Memory Type = SRAM
       - Data Width = 16bit
       - Write Operation = Enable
       - Extended Mode = Enable
       - Asynchronous Wait = Disable */
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

//---------------------------------------------------------------------/
//                       Hardware (I/O) Setup
//---------------------------------------------------------------------/
static void lcd_port_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
        
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
			   RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
			   RCC_APB2Periph_AFIO, ENABLE);



    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*
      FSMC_D0 ~ FSMC_D3
      PD14 FSMC_D0   PD15 FSMC_D1   PD0  FSMC_D2   PD1  FSMC_D3

      FSMC_A16 PD11 - RS
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |
        GPIO_Pin_1 | 
        GPIO_Pin_14 | 
        GPIO_Pin_15 | 
        GPIO_Pin_11;
    GPIO_Init(GPIOD,&GPIO_InitStructure);

    /*
      FSMC_D4 ~ FSMC_D12
      PE7 ~ PE15  FSMC_D4 ~ FSMC_D12
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |
        GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| 
        GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | 
        GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_Init(GPIOE,&GPIO_InitStructure);

    /* FSMC_D13 ~ FSMC_D15   PD8 ~ PD10 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOD,&GPIO_InitStructure);


    /* RD-PD4 WR-PD5 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOD,&GPIO_InitStructure);
    
    /* NE1/NCE2 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOD,&GPIO_InitStructure);

    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    Delay(10/portTICK_RATE_MS);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 //	 
    Delay(10/portTICK_RATE_MS);					   

    LCD_FSMCConfig();
}

//---------------------------------------------------------------------/
//                       REGISTER SET UP
//---------------------------------------------------------------------/
static void power_SET(void)
{
    //Toggle Reset Pin
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    Delay(10/portTICK_RATE_MS);
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 //	 
    Delay(10/portTICK_RATE_MS);    
    
   
    write_reg(0x0000,0x0001); //Start Oscillation
    Delay(10);

    write_reg(0x0015,0x0030);// internal voltage reg at 0.65*Vci
    write_reg(0x0011,0x0040);//Power Control Setup Reg 1 
    //Step-Up Circuit 1,2 = Fosc/128,
    // VciOut - 1 * Vci
    write_reg(0x0010,0x1628);//Power Control Setup Reg 1
    write_reg(0x0012,0x0000);//Power Control Setup Reg 3
    write_reg(0x0013,0x104d);//Power Control Setup Reg 4
    Delay(10);
    write_reg(0x0012,0x0010);//VREGout = 1.47
    Delay(10);
    write_reg(0x0010,0x2620);//Power Control Setup Reg1
    write_reg(0x0013,0x344d); //304d
    Delay(10);
    
    write_reg(0x0001,0x0100);//Driver Output Control
    write_reg(0x0002,0x0300);//Driving Range Control
    write_reg(0x0003,0x1008);//Entry Mode BGR, Horizontal, then vertical
    write_reg(0x0008,0x0604);//Display Control, first 4 and last 6
    //lines blank
    write_reg(0x0009,0x0000);//Display Control
    write_reg(0x000A,0x0008);//Output FMARK every 1 Frame

    write_reg(0x000C,0x1003);

    
    write_reg(0x0041,0x0002);
    write_reg(0x0060,0x2700);
    write_reg(0x0061,0x0001);
    write_reg(0x0090,0x0182);
    write_reg(0x0093,0x0001);
    write_reg(0x00a3,0x0010);
    Delay(10);
}

static void gamma_SET(void){
    Delay(10);
    write_reg(0x30,0x0000);		
    write_reg(0x31,0x0502);		
    write_reg(0x32,0x0307);		
    write_reg(0x33,0x0305);		
    write_reg(0x34,0x0004);		
    write_reg(0x35,0x0402);		
    write_reg(0x36,0x0707);		
    write_reg(0x37,0x0503);		
    write_reg(0x38,0x1505);		
    write_reg(0x39,0x1505);
    Delay(10);
}
static void display_ON(void)
{   
    Delay(10);
    write_reg(0x0007,0x0001);
    Delay(10);
    write_reg(0x0007,0x0021);
    write_reg(0x0007,0x0023);
    Delay(10);
    write_reg(0x0007,0x0033);
    Delay(10);
    write_reg(0x0007,0x0133);
}

static void display_OFF(void)
{
    Delay(10);
    write_reg(0x0007,0x0001);
    Delay(10);
}

static void lcd_data_bus_test(void)
{
    unsigned short temp1;
    unsigned short temp2;
    printf("bus test\r\n");

    /* [5:4]-ID~ID0 [3]-AM-1��ֱ-0ˮƽ */
    write_reg(0x0003,(1<<12)|(1<<5)|(1<<4) | (0<<3) );

    /* Write Alternating Bit Values */
    lcd_SetCursor(0,0);
    rw_data_prepare();
    write_data(0x5555);
    write_data(0xAAAA);
  

    /* Read it back and check*/
    lcd_SetCursor(0,0);
    temp1 = lcd_read_gram(0,0);
    temp2 = lcd_read_gram(1,0);
    if( (temp1 == 0x5555) && (temp2 == 0xAAAA) )
    {
        printf("Data bus test pass!\r\n");
    }
    else
    {
        printf("Data bus test error: %04X %04X\r\n",temp1,temp2);
    }
}

static void lcd_gram_test(void)
{
    unsigned short temp; //Temp value to put in GRAM
    unsigned int test_x;
    unsigned int test_y;

    printf("LCD GRAM test....\r\n");

    /* write */
    temp=0;
    
    write_reg(0x0003,(1<<12)|(1<<5)|(1<<4) | (0<<3) );
    lcd_SetCursor(0,0);
    rw_data_prepare();
    for(test_y=0; test_y<76800; test_y++) //test 320*240 Memory locations
    {
        write_data(temp); //put temp in GRAM
        temp++;
    }

    /* Read it back from GRAM and Test */
    temp=0;
    for(test_y=0; test_y<320; test_y++)
    {
	for(test_x=0; test_x<240; test_x++)
	{
	    if(  lcd_read_gram(test_x,test_y) != temp++)
	    {
		printf("LCD GRAM ERR!!");
		// while(1);
	    }
	}
    }
    printf("TEST PASS!\r\n");
}

/*******************************************************************************
 * Function Name  : LCD_SetDisplayWindow
 * Description    : Sets a display window
 * Input          : - Xpos: specifies the X buttom left position.
 *                  - Ypos: specifies the Y buttom left position.
 *                  - Height: display window height.
 *                  - Width: display window width.
 * Output         : None
 * Return         : None
 *******************************************************************************/
void LCD_SetDisplayWindow(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
    /* Horizontal GRAM Start Address */
    if(Xpos >= Height)
    {
	write_reg(0x0080, (Xpos - Height + 1));
	write_reg(0x0080, (Xpos));
    }
    else
    {
	printf("outside region\r\n");
	write_reg(0x0080, 0);
    }
    /* Horizontal GRAM End Address */
    write_reg(0x0082, Ypos);
    /* Vertical GRAM Start Address */
    if(Ypos >= Width)
    {
	write_reg(0x0081, (Ypos));
    }
    else
    {
	printf("outside region\r\n");
	write_reg(0x0081, 0);
    }
    /* Vertical GRAM End Address */
    write_reg(0x0082, Ypos+Height);

    lcd_SetCursor(Xpos, Ypos);
}

void lcd_SetCursor(unsigned int x,unsigned int y)
{
    write_reg(32,x);    /* 0-239 */
    write_reg(33,y);    /* 0-319 */
}

