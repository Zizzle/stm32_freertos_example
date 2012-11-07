#ifndef ILI_LCD_GENERAL_H_INCLUDED
#define ILI_LCD_GENERAL_H_INCLUDED

/*
 Compatible list:
 ili9320 ili9325 ili9328
 LG4531
*/

/* LCD color */
#define White            0xFFFF
#define Black            0x0000
#define Grey             0xF7DE
#define Blue             0x001F
#define Blue2            0x051F
#define Red              0xF800
#define Magenta          0xF81F
#define Green            0x07E0
#define Cyan             0x7FFF
#define Yellow           0xFFE0

/*---------------------- Graphic LCD size definitions ------------------------*/
#define LCD_WIDTH       240                 /* Screen Width (in pixels)   */
#define LCD_HEIGHT      320                 /* Screen Hight (in pixels)   */
#define BPP             16                  /* Bits per pixel             */
#define BYPP            ((BPP+7)/8)         /* Bytes per pixel            */

#define LCD_W 320
#define LCD_H 240

#define COL_BG_NORM 0x0890
#define COL_BG_HIGH 0x0000



/* Constants related to the LCD. */
#define mainMAX_LINE		( 240 )
#define mainROW_INCREMENT	( 24 )
#define mainMAX_COLUMN		( 20 )
#define mainCOLUMN_START	( 319 )
#define mainCOLUMN_INCREMENT 	( 16 )
#define mainLCD_QUEUE_SIZE	( 3 )

void lcd_init(void);
void lcd_clear(unsigned short Color);
void lcd_text_menu(uint16_t x_pos, uint16_t cell, const char * text);
struct menu;
void lcd_menu_update(struct menu * menu);
void lcd_PutString(unsigned int x, unsigned int y, const char * s, unsigned int textColor, unsigned int bkColor); 
void lcd_draw_buttons(void);
void lcd_draw_back_button(void);
void lcd_draw_applet_options(const char * text_1, char * text_2, char * text_3, char * text_4);
void lcd_DrawRect(int x1, int y1, int x2, int y2, int col);
void LCD_SetDisplayWindow(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width);
void DrawBMP(uint8_t* ptrBitmap);
/**
 * The LCD is written to by more than one task so is controlled by a
 * 'gatekeeper' task.  This is the only task that is actually permitted to
 * access the LCD directly.  Other tasks wanting to display a message send
 * the message to the gatekeeper.
 */
void vLCDTask( void *pvParameters );

//extern xQueueHandle xLCDQueue;

#define mainLCD_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE + 1500 )

#endif // ILI_LCD_GENERAL_H_INCLUDED
