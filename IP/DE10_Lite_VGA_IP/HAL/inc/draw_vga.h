/*
 * draw_vga.h
 *
 *  Created on: 12 dec 2017
 *      Author: Calle Melander
 *      Macros for draving lines, circles and text
 */

#ifndef DRAW_VGA_H_
#define DRAW_VGA_H_

// draw_vga.h
 void draw_hline(alt_u32 x_start,alt_u32 y_start,alt_u32 line_lenght,alt_u32 color);
 void draw_vline(alt_u32 x_start,alt_u32 y_start,alt_u32 line_lenght,alt_u32 color);
 void draw_angled_line(alt_u32 x_start,alt_u32 y_start,alt_u32 x_end,alt_u32 y_end,alt_u32 color);
 void draw_circle(alt_u32 x_start,alt_u32 y_start,alt_u32 radius,alt_u32 color);
 void clear_screen(alt_u32 color);
 void draw_filled_circle(alt_32 x0, alt_32 y0, alt_32 radius,alt_32 color);
 void tty_print(alt_32 x_start, alt_32 y_start, alt_8 *tty_string,alt_32 color, alt_u32 BGcolor);
 void char_print(alt_32 x_start, alt_32 y_start, alt_u8 tty_char,alt_32 color, alt_u32 BGcolor);
void int_print(alt_32 x_start, alt_32 y_start, int data, int data_l, alt_32 color, alt_u32 BGcolor); 

#endif /* DRAW_VGA_H_ */
