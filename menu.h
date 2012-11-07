///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, Matthew Pratt
//
// Licensed under the GNU General Public License v3 or greater
//
// Date: 21 Jun 2007
///////////////////////////////////////////////////////////////////////////////

#ifndef MENU_H
#define MENU_H

struct menu {
    const char *text;
    struct menu *next;
    void (*activate)(int initializing);
    void (*press_handler)(unsigned char button_down); // called when a menu item is pressed/released
    int  (*touch_handler)(int xx, int yy); // return non-zero if the key was consumed
};

#define MAX_DEPTH 10

void menu_set_root(struct menu *root_menu);
void menu_key(unsigned char key);
void menu_clear(void);
void menu_run_applet(int (*applet_key_handler)(unsigned char));

#define CRUMB_H 18
#define COL_W    (LCD_W / 2)

#endif
