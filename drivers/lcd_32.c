#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "lcd.h"
#include "semphr.h"
#include "stm32f10x.h"
#include "lcd_common.h"


//---------------------------------------------------------------------/
//               Static Functions and prototypes
//---------------------------------------------------------------------/
static void lcd_port_init(void);

void lcd_SetCursor(unsigned int x,unsigned int y)
{
	  if(deviceid==0x8989)
	  {
	    write_reg(0x004e,x); /* Row */
	    write_reg(0x004f,y); /* Line */
	  }
	  else if(deviceid==0x9919)
	  {
	    write_reg(0x004e,x); /* Row */
	    write_reg(0x004f,y); /* Line */
	  }
	  else
	  {
	    write_reg(0x0020,x); /* Row */
	    write_reg(0x0021,y); /* Line */
	  }
}

static void LCD_Initializtion2(long DeviceCode)
{
    printf("\r\nLCD Device ID : %04X \r\n",deviceid);

  if(DeviceCode==0x9325 || DeviceCode==0x9328)	/* ²»Í¬ÆÁÇý¶¯IC ³õÊŒ»¯²»Í¬ */
  {
    write_reg(0x00e7,0x0010);
    write_reg(0x0000,0x0001);  	/* start internal osc */
    write_reg(0x0001,0x0100);
    write_reg(0x0002,0x0700); 	/* power on sequence */
	write_reg(0x0003,(1<<12)|(1<<5)|(1<<4)|(0<<3) ); 	/* importance */
    write_reg(0x0004,0x0000);
    write_reg(0x0008,0x0207);
    write_reg(0x0009,0x0000);
    write_reg(0x000a,0x0000); 	/* display setting */
    write_reg(0x000c,0x0001);	/* display setting */
    write_reg(0x000d,0x0000);
    write_reg(0x000f,0x0000);
    /* Power On sequence */
    write_reg(0x0010,0x0000);
    write_reg(0x0011,0x0007);
    write_reg(0x0012,0x0000);
    write_reg(0x0013,0x0000);
    Delay(5);  /* Delay 50 ms */
    write_reg(0x0010,0x1590);
    write_reg(0x0011,0x0227);
    Delay(5);  /* Delay 50 ms */
    write_reg(0x0012,0x009c);
    Delay(5);  /* Delay 50 ms */
    write_reg(0x0013,0x1900);
    write_reg(0x0029,0x0023);
    write_reg(0x002b,0x000e);
    Delay(5);  /* Delay 50 ms */
    write_reg(0x0020,0x0000);
    write_reg(0x0021,0x0000);
    Delay(5);  /* Delay 50 ms */
    write_reg(0x0030,0x0007);
    write_reg(0x0031,0x0707);
    write_reg(0x0032,0x0006);
    write_reg(0x0035,0x0704);
    write_reg(0x0036,0x1f04);
    write_reg(0x0037,0x0004);
    write_reg(0x0038,0x0000);
    write_reg(0x0039,0x0706);
    write_reg(0x003c,0x0701);
    write_reg(0x003d,0x000f);
    Delay(5);  /* Delay 50 ms */
    write_reg(0x0050,0x0000);
    write_reg(0x0051,0x00ef);
    write_reg(0x0052,0x0000);
    write_reg(0x0053,0x013f);
    write_reg(0x0060,0xa700);
    write_reg(0x0061,0x0001);
    write_reg(0x006a,0x0000);
    write_reg(0x0080,0x0000);
    write_reg(0x0081,0x0000);
    write_reg(0x0082,0x0000);
    write_reg(0x0083,0x0000);
    write_reg(0x0084,0x0000);
    write_reg(0x0085,0x0000);

    write_reg(0x0090,0x0010);
    write_reg(0x0092,0x0000);
    write_reg(0x0093,0x0003);
    write_reg(0x0095,0x0110);
    write_reg(0x0097,0x0000);
    write_reg(0x0098,0x0000);
    /* display on sequence */
    write_reg(0x0007,0x0133);

    write_reg(0x0020,0x0000);  /* ÐÐÊ×Ö·0 */
    write_reg(0x0021,0x0000);  /* ÁÐÊ×Ö·0 */
  }
  else if(DeviceCode==0x9320 || DeviceCode==0x9300)
  {
    write_reg(0x00,0x0000);
	write_reg(0x01,0x0100);	/* Driver Output Contral */
	write_reg(0x02,0x0700);	/* LCD Driver Waveform Contral */
	write_reg(0x03,0x1018);	/* Entry Mode Set */

	write_reg(0x04,0x0000);	/* Scalling Contral */
    write_reg(0x08,0x0202);	/* Display Contral */
	write_reg(0x09,0x0000);	/* Display Contral 3.(0x0000) */
	write_reg(0x0a,0x0000);	/* Frame Cycle Contal.(0x0000) */
    write_reg(0x0c,(1<<0));	/* Extern Display Interface Contral */
	write_reg(0x0d,0x0000);	/* Frame Maker Position */
	write_reg(0x0f,0x0000);	/* Extern Display Interface Contral 2. */

    Delay(10);  /* Delay 100 ms */
	write_reg(0x07,0x0101);	/* Display Contral */
    Delay(10);  /* Delay 100 ms */

	write_reg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	/* Power Control 1.(0x16b0)	*/
	write_reg(0x11,0x0007);								/* Power Control 2 */
	write_reg(0x12,(1<<8)|(1<<4)|(0<<0));				/* Power Control 3.(0x0138)	*/
	write_reg(0x13,0x0b00);								/* Power Control 4 */
	write_reg(0x29,0x0000);								/* Power Control 7 */

	write_reg(0x2b,(1<<14)|(1<<4));

	write_reg(0x50,0);       /* Set X Start */
	write_reg(0x51,239);	    /* Set X End */
	write_reg(0x52,0);	    /* Set Y Start */
	write_reg(0x53,319);	    /* Set Y End */

	write_reg(0x60,0x2700);	/* Driver Output Control */
	write_reg(0x61,0x0001);	/* Driver Output Control */
	write_reg(0x6a,0x0000);	/* Vertical Srcoll Control */

	write_reg(0x80,0x0000);	/* Display Position? Partial Display 1 */
	write_reg(0x81,0x0000);	/* RAM Address Start? Partial Display 1 */
	write_reg(0x82,0x0000);	/* RAM Address End-Partial Display 1 */
	write_reg(0x83,0x0000);	/* Displsy Position? Partial Display 2 */
	write_reg(0x84,0x0000);	/* RAM Address Start? Partial Display 2 */
	write_reg(0x85,0x0000);	/* RAM Address End? Partial Display 2 */

    write_reg(0x90,(0<<7)|(16<<0));	/* Frame Cycle Contral.(0x0013)	*/
	write_reg(0x92,0x0000);	/* Panel Interface Contral 2.(0x0000) */
	write_reg(0x93,0x0001);	/* Panel Interface Contral 3. */
    write_reg(0x95,0x0110);	/* Frame Cycle Contral.(0x0110)	*/
	write_reg(0x97,(0<<8));
	write_reg(0x98,0x0000);	/* Frame Cycle Contral */

    write_reg(0x07,0x0173);
  }
  else if(DeviceCode==0x9331)
  {
	write_reg(0x00E7, 0x1014);
	write_reg(0x0001, 0x0100);   /* set SS and SM bit */
	write_reg(0x0002, 0x0200);   /* set 1 line inversion */
	write_reg(0x0003, 0x1030);   /* set GRAM write direction and BGR=1 */
	write_reg(0x0008, 0x0202);   /* set the back porch and front porch */
    write_reg(0x0009, 0x0000);   /* set non-display area refresh cycle ISC[3:0] */
	write_reg(0x000A, 0x0000);   /* FMARK function */
	write_reg(0x000C, 0x0000);   /* RGB interface setting */
	write_reg(0x000D, 0x0000);   /* Frame marker Position */
	write_reg(0x000F, 0x0000);   /* RGB interface polarity */
	/* Power On sequence */
	write_reg(0x0010, 0x0000);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
	write_reg(0x0011, 0x0007);   /* DC1[2:0], DC0[2:0], VC[2:0] */
	write_reg(0x0012, 0x0000);   /* VREG1OUT voltage	*/
	write_reg(0x0013, 0x0000);   /* VDV[4:0] for VCOM amplitude */
    Delay(20);                  /* Delay 200 ms */
	write_reg(0x0010, 0x1690);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
	write_reg(0x0011, 0x0227);   /* DC1[2:0], DC0[2:0], VC[2:0] */
    Delay(5);                   /* Delay 50 ms */
	write_reg(0x0012, 0x000C);   /* Internal reference voltage= Vci	*/
    Delay(5);                    /* Delay 50 ms */
	write_reg(0x0013, 0x0800);   /* Set VDV[4:0] for VCOM amplitude */
	write_reg(0x0029, 0x0011);   /* Set VCM[5:0] for VCOMH */
	write_reg(0x002B, 0x000B);   /* Set Frame Rate */
    Delay(5);                   /* Delay 50 ms */
	write_reg(0x0020, 0x0000);   /* GRAM horizontal Address */
	write_reg(0x0021, 0x0000);   /* GRAM Vertical Address */
	/* Adjust the Gamma Curve */
	write_reg(0x0030, 0x0000);
	write_reg(0x0031, 0x0106);
	write_reg(0x0032, 0x0000);
	write_reg(0x0035, 0x0204);
	write_reg(0x0036, 0x160A);
	write_reg(0x0037, 0x0707);
	write_reg(0x0038, 0x0106);
	write_reg(0x0039, 0x0707);
	write_reg(0x003C, 0x0402);
	write_reg(0x003D, 0x0C0F);
	/* Set GRAM area */
	write_reg(0x0050, 0x0000);   /* Horizontal GRAM Start Address */
	write_reg(0x0051, 0x00EF);   /* Horizontal GRAM End Address */
	write_reg(0x0052, 0x0000);   /* Vertical GRAM Start Address */
	write_reg(0x0053, 0x013F);   /* Vertical GRAM Start Address */
	write_reg(0x0060, 0x2700);   /* Gate Scan Line */
	write_reg(0x0061, 0x0001);   /*  NDL,VLE, REV */
	write_reg(0x006A, 0x0000);   /* set scrolling line */
	/* Partial Display Control */
	write_reg(0x0080, 0x0000);
	write_reg(0x0081, 0x0000);
	write_reg(0x0082, 0x0000);
	write_reg(0x0083, 0x0000);
	write_reg(0x0084, 0x0000);
	write_reg(0x0085, 0x0000);
	/* Panel Control */
	write_reg(0x0090, 0x0010);
	write_reg(0x0092, 0x0600);
	write_reg(0x0007,0x0021);
    Delay(5);                   /* Delay 50 ms */
	write_reg(0x0007,0x0061);
    Delay(5);                   /* Delay 50 ms */
	write_reg(0x0007,0x0133);    /* 262K color and display ON */
    Delay(5);                   /* Delay 50 ms */
  }
  else if(DeviceCode==0x9919)
  {
    /* POWER ON &RESET DISPLAY OFF */
	write_reg(0x28,0x0006);
	write_reg(0x00,0x0001);
	write_reg(0x10,0x0000);
	write_reg(0x01,0x72ef);
	write_reg(0x02,0x0600);
	write_reg(0x03,0x6a38);
	write_reg(0x11,0x6874);
	write_reg(0x0f,0x0000);    /* RAM WRITE DATA MASK */
	write_reg(0x0b,0x5308);    /* RAM WRITE DATA MASK */
	write_reg(0x0c,0x0003);
	write_reg(0x0d,0x000a);
	write_reg(0x0e,0x2e00);
	write_reg(0x1e,0x00be);
	write_reg(0x25,0x8000);
	write_reg(0x26,0x7800);
	write_reg(0x27,0x0078);
	write_reg(0x4e,0x0000);
	write_reg(0x4f,0x0000);
	write_reg(0x12,0x08d9);
	/* Adjust the Gamma Curve */
	write_reg(0x30,0x0000);
	write_reg(0x31,0x0104);
	write_reg(0x32,0x0100);
    write_reg(0x33,0x0305);
    write_reg(0x34,0x0505);
	write_reg(0x35,0x0305);
    write_reg(0x36,0x0707);
    write_reg(0x37,0x0300);
	write_reg(0x3a,0x1200);
	write_reg(0x3b,0x0800);
    write_reg(0x07,0x0033);
  }
  else if(DeviceCode==0x1505)
  {
    /* second release on 3/5  ,luminance is acceptable,water wave appear during camera preview */
    write_reg(0x0007,0x0000);
    Delay(5);                   /* Delay 50 ms */
    write_reg(0x0012,0x011C);    /* why need to set several times?	*/
    write_reg(0x00A4,0x0001);    /* NVM */
    write_reg(0x0008,0x000F);
    write_reg(0x000A,0x0008);
    write_reg(0x000D,0x0008);
    /* GAMMA CONTROL */
    write_reg(0x0030,0x0707);
    write_reg(0x0031,0x0007);
    write_reg(0x0032,0x0603);
    write_reg(0x0033,0x0700);
    write_reg(0x0034,0x0202);
    write_reg(0x0035,0x0002);
    write_reg(0x0036,0x1F0F);
    write_reg(0x0037,0x0707);
    write_reg(0x0038,0x0000);
    write_reg(0x0039,0x0000);
    write_reg(0x003A,0x0707);
    write_reg(0x003B,0x0000);
    write_reg(0x003C,0x0007);
    write_reg(0x003D,0x0000);
    Delay(5);                   /* Delay 50 ms */
    write_reg(0x0007,0x0001);
    write_reg(0x0017,0x0001);    /* Power supply startup enable */
    Delay(5);                   /* Delay 50 ms */
    /* power control */
    write_reg(0x0010,0x17A0);
    write_reg(0x0011,0x0217);    /* reference voltage VC[2:0]   Vciout = 1.00*Vcivl */
    write_reg(0x0012,0x011E);    /* Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?	*/
    write_reg(0x0013,0x0F00);    /* VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl */
    write_reg(0x002A,0x0000);
    write_reg(0x0029,0x000A);    /* Vcomh = VCM1[4:0]*Vreg1out    gate source voltage?? */
    write_reg(0x0012,0x013E);    /* power supply on */
    /* Coordinates Control */
    write_reg(0x0050,0x0000);
    write_reg(0x0051,0x00EF);
    write_reg(0x0052,0x0000);
    write_reg(0x0053,0x013F);
    /* Pannel Image Control */
    write_reg(0x0060,0x2700);
    write_reg(0x0061,0x0001);
    write_reg(0x006A,0x0000);
    write_reg(0x0080,0x0000);
    /* Partial Image Control */
    write_reg(0x0081,0x0000);
    write_reg(0x0082,0x0000);
    write_reg(0x0083,0x0000);
    write_reg(0x0084,0x0000);
    write_reg(0x0085,0x0000);
    /* Panel Interface Control */
    write_reg(0x0090,0x0013);      /* frenqucy */
    write_reg(0x0092,0x0300);
    write_reg(0x0093,0x0005);
    write_reg(0x0095,0x0000);
    write_reg(0x0097,0x0000);
    write_reg(0x0098,0x0000);

    write_reg(0x0001,0x0100);
    write_reg(0x0002,0x0700);
    write_reg(0x0003,0x1030);
    write_reg(0x0004,0x0000);
    write_reg(0x000C,0x0000);
    write_reg(0x000F,0x0000);
    write_reg(0x0020,0x0000);
    write_reg(0x0021,0x0000);
    write_reg(0x0007,0x0021);
    Delay(20);                   /* Delay 200 ms */
    write_reg(0x0007,0x0061);
    Delay(20);                   /* Delay 200 ms */
    write_reg(0x0007,0x0173);
    Delay(20);                   /* Delay 200 ms */
  }
  else if(DeviceCode==0x8989)
  {
    write_reg(0x0000,0x0001);    Delay(5);   /* Žò¿ªŸ§Õñ */
    write_reg(0x0003,0xA8A4);    Delay(5);
    write_reg(0x000C,0x0000);    Delay(5);
    write_reg(0x000D,0x080C);    Delay(5);
    write_reg(0x000E,0x2B00);    Delay(5);
    write_reg(0x001E,0x00B0);    Delay(5);
    write_reg(0x0001,0x2B3F);    Delay(5);   /* Çý¶¯Êä³ö¿ØÖÆ320*240 0x2B3F */
    write_reg(0x0002,0x0600);    Delay(5);
    write_reg(0x0010,0x0000);    Delay(5);
    write_reg(0x0011,0x6818);    Delay(5);   /* ¶šÒåÊýŸÝžñÊœ 16Î»É« ºáÆÁ 0x6070 */
    write_reg(0x0005,0x0000);    Delay(5);
    write_reg(0x0006,0x0000);    Delay(5);
    write_reg(0x0016,0xEF1C);    Delay(5);
    write_reg(0x0017,0x0003);    Delay(5);
    write_reg(0x0007,0x0133);    Delay(5);
    write_reg(0x000B,0x0000);    Delay(5);
    write_reg(0x000F,0x0000);    Delay(5);   /* ÉšÃè¿ªÊŒµØÖ· */
    write_reg(0x0041,0x0000);    Delay(5);
    write_reg(0x0042,0x0000);    Delay(5);
    write_reg(0x0048,0x0000);    Delay(5);
    write_reg(0x0049,0x013F);    Delay(5);
    write_reg(0x004A,0x0000);    Delay(5);
    write_reg(0x004B,0x0000);    Delay(5);
    write_reg(0x0044,0xEF00);    Delay(5);
    write_reg(0x0045,0x0000);    Delay(5);
    write_reg(0x0046,0x013F);    Delay(5);
    write_reg(0x0030,0x0707);    Delay(5);
    write_reg(0x0031,0x0204);    Delay(5);
    write_reg(0x0032,0x0204);    Delay(5);
    write_reg(0x0033,0x0502);    Delay(5);
    write_reg(0x0034,0x0507);    Delay(5);
    write_reg(0x0035,0x0204);    Delay(5);
    write_reg(0x0036,0x0204);    Delay(5);
    write_reg(0x0037,0x0502);    Delay(5);
    write_reg(0x003A,0x0302);    Delay(5);
    write_reg(0x003B,0x0302);    Delay(5);
    write_reg(0x0023,0x0000);    Delay(5);
    write_reg(0x0024,0x0000);    Delay(5);
    write_reg(0x0025,0x8000);    Delay(5);
    write_reg(0x004f,0);        /* ÐÐÊ×Ö·0 */
    write_reg(0x004e,0);        /* ÁÐÊ×Ö·0 */
  }
  Delay(5);  /* Delay 50 ms */
}


//---------------------------------------------------------------------/
//                       INIT
//---------------------------------------------------------------------/
void lcd_init(void)
{
    xLcdSemaphore = xSemaphoreCreateMutex();

    lcd_port_init(); //initialise IO Registers

    /* deviceid check */
    deviceid = read_reg(0x00);
    LCD_Initializtion2(deviceid);

#if 0
//    write_reg(0x0011,0x6838);//Entry Mode BGR, Horizontal, then vertical

    //Eye Candy//
    Delay(10);
    lcd_clear( 0xA0FF );
    Delay(100);
    lcd_clear( Black );
    Delay(100);
    lcd_clear( 0xA0FF );
    Delay(100);
    lcd_clear( Black );
#endif
    printf("Lcd_init done\r\n");
}

static void LCD_FSMCConfig(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;

    /*-- FSMC Configuration ----------------------------------------------*/
    p.FSMC_AddressSetupTime = 1;             /* ��ַ����ʱ��  */
    p.FSMC_AddressHoldTime = 0;              /* ��ַ����ʱ��  */
    p.FSMC_DataSetupTime = 2;                /* ��ݽ���ʱ��  */
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

	  /* Enable FSMC, GPIOD, GPIOE and AFIO clocks */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	  /* PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9),
	     PE.13(D10), PE.14(D11), PE.15(D12) */
	  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                                 GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                                 GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);

	  /* PD.00(D2), PD.01(D3), PD.04(RD), PD.5(WR), PD.7(CS), PD.8(D13), PD.9(D14),
	     PD.10(D15), PD.11(RS) PD.14(D0) PD.15(D1) */
	  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 |
	                                 GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
	                                 GPIO_Pin_14 | GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);


    LCD_FSMCConfig();
}
