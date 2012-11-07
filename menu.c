///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, Matthew Pratt
//
// Licensed under the GNU General Public License v3 or greater
//
// Date: 21 Jun 2007
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "touch.h"
#include "menu.h"
#include "queue.h"
#include "lcd.h"
#define HEIGHT 6

#define KEY_UP    0x8
#define KEY_DOWN  0x4
#define KEY_LEFT  0x2
#define KEY_RIGHT 0x1
#define KEY_PRESSED 0x10

static struct menu  *g_menu[MAX_DEPTH];
static unsigned char g_crumbs[MAX_DEPTH];
static unsigned      g_item = -1;
static unsigned char g_index = 0;
static unsigned char g_entries = 0;
static unsigned char g_rows = 0;
static unsigned      g_rowh;
static int (*g_menu_applet)(int, int) = NULL;
static int menu_touch_y;
static int menu_touch_x;

static unsigned start_time;

static void menu_update_hilight(void)
{

}

static void menu_run_callback(char init)
{
    if (g_index > 0)
    {
        void (*callback)(int) = g_menu[g_index - 1][g_crumbs[g_index - 1]].activate;
        if (callback)
            callback(init);
    }
}

static int menu_get_selected()
{
	for (int ii = 0; ii < g_rows; ii++)
	{
    	int yy = CRUMB_H + ii * (g_rowh);

    	if (menu_touch_y >= yy && menu_touch_y <= yy + g_rowh)
    	{
    		if (g_entries != g_rows)
    		{
    			for (int kk = 0; kk < 2; kk++)
    			{
    		    	int xx = COL_W * kk + kk;
    				if (menu_touch_x >= xx && menu_touch_x < xx + COL_W)
    				{
    					return kk * g_rows + ii;
    				}
    			}
    		}
    		return ii;
    	}
	}
	return -1;
}

static void menu_paint_cell(int index)
{
	int row = index % g_rows;
	int col = index / g_rows;
	int yy = CRUMB_H + row * (g_rowh);
	int ww = g_rows == g_entries ? LCD_W : COL_W;
	int xx = ww * col + col;
	int offset = g_rows == g_entries ? 55 : 10;
	uint16_t bgCol = COL_BG_NORM; 

	//printf("ii %d, row %d, xx %d, yy %d\r\n", index, row, xx, yy);

	if (index == g_item)
	{
		bgCol = COL_BG_HIGH;
	}
	lcd_fill(xx,  yy, ww, g_rowh - 1, bgCol);
	if (index < g_entries)
		lcd_text_xy(xx + offset, CRUMB_H + (row * g_rowh) + (g_rowh / 2) - 8, g_menu[g_index][index].text, 0xFFFF, bgCol);
}

static void menu_two_column()
{
    unsigned char ii;   
    unsigned char row = 0;
    unsigned char col = 0;
    uint16_t bgCol = COL_BG_NORM;

    g_rows = (g_entries / 2) + ((g_entries % 2) != 0);    
    g_rowh = (LCD_H - CRUMB_H) / g_rows;

    //printf("Rows: %d rowh: %d entries: %d\r\n", g_rows, g_rowh, g_entries);
    
    // draw the grid
    lcd_fill(COL_W, CRUMB_H, 1, LCD_H - CRUMB_H, 0xFFFF);
    for (ii = 0; ii < g_rows - 1; ii++)
		lcd_fill(0,  CRUMB_H + (ii +1) * (g_rowh) - 1, LCD_W, 1,    0xFFFF);
    
    for (ii = 0; g_menu[g_index][ii].text; ii++)
    {
    	menu_paint_cell(ii);
    }	
}

static void menu_paint_row(int row)
{
	int yy = CRUMB_H + row * (g_rowh);
	uint16_t bgCol = COL_BG_NORM; 
	
}

static void menu_update(void)
{
	start_time = xTaskGetTickCount();
	
    unsigned char ii;
    uint16_t bgCol = COL_BG_NORM;    	

	lcd_lock();
	lcd_background(0);

    // clear menu bg
    lcd_fill(0, CRUMB_H, LCD_W, LCD_H - CRUMB_H, bgCol);

    // draw the crumbs
    char crumbs[90] = "Demo2";
    for (ii = 1; ii <= g_index; ii++)
    {
        strcat(crumbs, ":");
        strcat(crumbs, g_menu[ii-1][g_crumbs[ii-1]].text);
    }
    lcd_fill(0, 0, LCD_W, CRUMB_H, 0x0);
    lcd_text(0, 0, crumbs);
	lcd_fill(0, CRUMB_H - 2, LCD_W, 2, 0xFFFF);

    // how big is the menu?
    g_entries = 0;
    for (ii = 0; g_menu[g_index][ii].text; ii++)
    {
    	g_entries++;
    }

    // if above a certain size draw in two columns
    if (g_entries > 4)
    {
    	menu_two_column();
    }
    else
    {
    	g_rows = g_entries;
    	g_rowh = (240 - CRUMB_H) / g_entries;

    	for (ii = 0; g_menu[g_index][ii].text; ii++)
    	{
    		menu_paint_cell(ii);
    		if (ii != g_entries - 1)
    			lcd_fill(0,  CRUMB_H + (ii +1) * (g_rowh) - 1, LCD_W, 1,    0xFFFF);
    	}
    }

    lcd_printf(30, 0, 10, "%dms", (xTaskGetTickCount() - start_time));
    lcd_release();
}

void menu_set_root(struct menu *root_menu)
{
    g_menu[0] = root_menu;
    menu_update();
}

//
// Go back in the menu hierachy after the left key has been pressed
// in an applet (or double clicked)
//
static void menu_back_after_applet()
{
    void (*callback)(int) = g_menu[g_index][g_item].activate;
	//printf("Item %d %x\r\n", g_item, callback);
    if (callback)
    	callback(0); // deactivate

    g_item = -1;
    menu_update();
    g_menu_applet = NULL;
    menu_run_callback(1);
}

void menu_touch(int xx, int yy)
{
    if (g_menu_applet) {
    	if (g_menu_applet(xx, yy))
    		menu_back_after_applet();
        return;
    }

    menu_touch_y = yy;
    menu_touch_x = xx;    
    //menu_update();       

    int old = g_item;
    g_item = menu_get_selected();

    lcd_lock();

    if (old != -1)
    	menu_paint_cell(old);
    if (g_item != -1)
    	menu_paint_cell(g_item);

    lcd_release();

    if (xx == -1 || yy == -1 || g_item == -1)
    {
    	if (old != -1)
    	{
    		if (g_menu[g_index][old].press_handler)
    		{
    			g_menu[g_index][old].press_handler(0);
    		}

    	    void (*callback)(int) = g_menu[g_index][old].activate;
    	    g_crumbs[g_index] = old;

    	    if (g_menu[g_index][old].next && g_index < MAX_DEPTH)
    	    {
    	        g_index++;
    	        g_menu[g_index] = g_menu[g_index-1][old].next;
    	        menu_update();
    	    }
    	    else if (g_menu[g_index][old].touch_handler)
    	    {
    	        g_menu_applet = g_menu[g_index][old].touch_handler;
    	        g_item = old;
    	        menu_clear();
    	    }
    	    else if (strcmp(g_menu[g_index][old].text, "Back") == 0)
    	    {
    	        menu_run_callback(0);    	

    	        if (g_index > 0)
    	            g_index--;
    	        menu_update();
    	        menu_run_callback(1);    	
    	    }
    	    
    	    // run the callback which should start the applet or update the display
    	    if (callback)
    	    {
    	        callback(1);
    	    }
    	}
    	return;
    }
    
    menu_touch_y = 0;
    menu_touch_x = 0;

    if (g_menu[g_index][g_item].press_handler)
    {
    	g_menu[g_index][g_item].press_handler(1);
    }
}

void menu_clear(void)
{
    lcd_clear(0x0);
//    lcd_clear_pixels(0, HILIGHT_Y(g_item + 1), HILIGHT_W, HILIGHT_H);
}
