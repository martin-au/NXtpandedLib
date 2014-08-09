#ifndef __NXT_LCD_H__
#  define __NXT_LCD_H__

#  include "mytypes.h"

#  define NXT_LCD_WIDTH 100
#  define NXT_LCD_DEPTH 8

// Martin 01.12.2013
// for NLcd
#include "nxt_spi.h"
static U8 *display = (U8 *)0;

void nxt_lcd_set_buffer(U8 *disp) {
	display = disp;
	nxt_spi_set_display(display);
}

void nxt_lcd_init(const U8 *disp);
void nxt_lcd_power_up(void);
void nxt_lcd_power_down(void);
void nxt_lcd_update();
void nxt_lcd_force_update();


#endif
