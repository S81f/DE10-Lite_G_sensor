/*
 * draw_vga.c
 *
 *  Created on: 18 dec 2017
 *      Author: Calle Melander med flera
 *      Macros for draving lines, circles, text and integer on VGA
 */


#include <alt_types.h>
#include <string.h>
#include "char_map.h"
#include <DE10_Lite_VGA_Driver.h>

extern alt_u32 upper_char_line(alt_u8 char_pos); //__attribute__((section("My_glypt")));
extern alt_u32 lower_char_line(alt_u8 char_pos); //__attribute__((section("My_glypt")));

/*
void draw_pixel(alt_32 x,alt_32 y,alt_32 color){
	write_pixel(x,y,color);
}
*/

void clear_screen(alt_u32 color) {
	alt_u32 x,y;
	for(y=0;y<240;y++) {
		for(x=0;x<320;x++) {
			write_pixel(x,y,color);
		}
	}
}

void draw_hline(alt_u32 x_start,alt_u32 y_start,alt_u32 line_lenght,alt_u32 color){
	alt_u32 x;
	for(x=0;x<line_lenght;x++){
		write_pixel(x_start+x,y_start,color);
	}
}

void draw_vline(alt_u32 x_start,alt_u32 y_start,alt_u32 line_lenght,alt_u32 color){
	alt_u32 x;
	for(x=0;x<line_lenght;x++){
		write_pixel(x_start,y_start+x,color);
	}

}


void draw_circle(alt_u32 x_start,alt_u32 y_start,alt_u32 radius,alt_u32 color){
	//  Bresenham's circle algorithm
    alt_32 x = radius-1;
    alt_32 y = 0;
    alt_32 dx = 1;
    alt_32 dy = 1;
    alt_32 err = dx - (radius << 1);
    while (x >= y)
    {
    	write_pixel(x_start + x, y_start + y, color);
        write_pixel(x_start + y, y_start + x, color);
        write_pixel(x_start - y, y_start + x, color);
        write_pixel(x_start - x, y_start + y, color);
        write_pixel(x_start - x, y_start - y, color);
        write_pixel(x_start - y, y_start - x, color);
        write_pixel(x_start + y, y_start - x, color);
        write_pixel(x_start + x, y_start - y, color);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}


void draw_filled_circle(alt_32 x0, alt_32 y0, alt_32 radius,alt_32 color)
{
    alt_32 x = radius;
    alt_32 y = 0;
    alt_32 xChange = 1 - (radius << 1);
    alt_32 yChange = 0;
    alt_32 radiusError = 0;

    while (x >= y)
    {
        for (alt_32 i = x0 - x; i <= x0 + x; i++)
        {
        	write_pixel(i, y0 + y, color);
        	write_pixel(i, y0 - y, color);
        }
        for (alt_32 i = x0 - y; i <= x0 + y; i++)
        {
        	write_pixel(i, y0 + x, color);
        	write_pixel(i, y0 - x, color);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

void draw_angled_line(alt_u32 x_start,alt_u32 y_start,alt_u32 x_end,alt_u32 y_end,alt_u32 color){
    alt_u32 steep = 0;
    alt_32 x0,x1,y0,y1;
    x0 = x_start;
    x1 = x_end;
    y0 = y_start;
    y1 = y_end;

    // Justera vinklar
    if (abs(x_start-x_end)<abs(y_start-y_end)) {
    	if (y_start > y_end){
			steep = 1;
          	x0 = y_end;
			x1 = y_start;
			y0 = x_end;
			y1 = x_start;
		}else{
        x0 = y_start;
        y0 = x_start;
        x1 = y_end;
        y1 = x_end;
        steep = 1;
		}
    }
    if ((x_start>x_end)&&!steep) {
        x0 = x_end;
        x1 = x_start;
        y0 = y_end;
        y1 = y_start;
    }


    alt_32 dx = x1-x0;
    alt_32 dy = y1-y0;
    alt_32 derror = abs(dy)*2;
    alt_32 error = 0;
    alt_32 y = y0;
    for (alt_32 x=x0; x<=x1; x++) {
			if (steep) {
				write_pixel(y, x, color);
			} else {
				write_pixel(x, y, color);
			}

        error += derror;
        if (error > dx) {
            y += (y1>y0?1:-1);
            error -= dx*2;
        }
    }
}

 void tty_print(alt_32 x_start, alt_32 y_start, alt_8 *tty_string,alt_32 color, alt_u32 BGcolor){
	 alt_u8 *tpek;
	 alt_u32 n = strlen(tty_string);
	 tpek = tty_string;
	 for(alt_u32 i=0;i<n;i++){ // antal tecken
		 alt_u8 tkn = *tpek;
		 alt_u32 pry = y_start ;
		 alt_u32 half_tkn = upper_char_line(tkn-0x20);
		 alt_u32 dot =  half_tkn;
			 for(alt_u32 ii=0;ii<4;ii++){ // rader per ½ tecken
				 alt_u32 prx = x_start +i*8;
					 for(alt_u32 jj=0;jj<8;jj++){ // pixlar på rad (8)
					 alt_u32 dotcolor = (0x80000000 &dot)?color:BGcolor;
					 write_pixel(prx,pry,dotcolor);
					 dot = dot<<1;
					 prx++;
				 }
				 pry++;
			 }
			// nederdel
			 half_tkn = lower_char_line(tkn-0x20);
			  dot =  half_tkn;  // på/av färg  // 0x80000000 &
				 for(alt_u32 ii=0;ii<4;ii++){ // rader per ½ tecken
					 alt_u32 prx = x_start +i*8;
						 for(alt_u32 jj=0;jj<8;jj++){ // pixlar på rad (8)
						 alt_u32 dotcolor = (0x80000000 &dot)?color:BGcolor;
						 write_pixel(prx,pry,dotcolor);
						 dot = dot<<1;
						 prx++;
					 }
					 pry++;
				 }
		 tpek ++;
	 }
 };


 void char_print(alt_32 x_start, alt_32 y_start, alt_u8 tty_char,alt_32 color, alt_u32 BGcolor){
	 alt_u8 tkn = tty_char;
		 alt_u32 pry = y_start ;
		 alt_u32 dot = upper_char_line(tkn-0x20);
			 for(alt_u32 ii=0;ii<4;ii++){ // rader per ½ tecken
				 alt_u32 prx = x_start;
				 for(alt_u32 jj=0;jj<8;jj++){ // pixlar på rad (8)
					 alt_u32 dotcolor = (0x80000000 &dot)?color:BGcolor;
					 write_pixel(prx,pry,dotcolor);
					 dot = dot<<1;
					 prx++;
				 }
				 pry++;
			 }
			// nederdel
			 dot = lower_char_line(tkn-0x20);
				 for(alt_u32 ii=0;ii<4;ii++){ // rader per ½ tecken
					 alt_u32 prx = x_start;
					 for(alt_u32 jj=0;jj<8;jj++){ // pixlar på rad (8)
						 alt_u32 dotcolor = (0x80000000 &dot)?color:BGcolor;
						 write_pixel(prx,pry,dotcolor);
						 dot = dot<<1;
						 prx++;
					 }
					 pry++;
				 }
 };

/* Print integer
Exempel: int_print(10,10,(int) x_data,3,0xFFF,0x000);
Position 10,10 (X,Y)
Data X_data
Hur många siffror: 3
Färg: 0xFFF (vit)
Bakgrundsfärg: 0x000 (svart)
*/

void int_print(alt_32 x_start, alt_32 y_start, int data, int data_l,
alt_32 color, alt_u32 BGcolor) 
{
int i = 0, j = 1;
alt_32 x = 0;
if (data < 0) {
char_print(x_start, y_start, '-', color, BGcolor);
data = -data;
} else {
char_print(x_start, y_start, '+', color, BGcolor);
}
for (i = 0; i < data_l; i++) {
char_print(x_start + 10 * data_l - x, y_start, data / j % 10 + 48,
color, BGcolor);
j *= 10;
x += 10;
}
}
