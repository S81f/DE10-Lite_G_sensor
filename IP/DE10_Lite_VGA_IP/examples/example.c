#include <system.h>
#include <stdio.h>
#include <alt_types.h>
#include <math.h>
#include <DE10_Lite_VGA_Driver.h>
#include "draw_vga.h"


int main() {

	// Colors
		alt_u32 Screen_BG1 = 0x000;
		alt_u32 Screen_BG2 = 0xFFF;
		alt_u32 Dial_RIM = 0x444;
		alt_u32 Dial_BG = 0xCCC;
		alt_u32 Dial_POINT1 = 0xF00;
		alt_u32 Dial_POINT2 = 0x0F0;
		alt_u32 Dial_POINT3 = 0x00F;
		alt_u32 box_color=0xfff;

	// X data
	alt_u32 Xx = 75; // x mittpunkt
	alt_u32 Xy = 75; // y mittpunkt
	alt_u32 Xr = 50; // visar radie
	alt_u32 Xdr = 51; // tavel radie
	alt_u32 Xrr = 52; // tavelram radie
	// Y data
	alt_u32 Yx = 225;
	alt_u32 Yy = 75;
	alt_u32 Yr = 50;
	alt_u32 Ydr = 51;
	alt_u32 Yrr = 52;


	alt_u32 Zx = 200;
	alt_u32 Zy = 200;
	alt_u32 Zr = 35;
	alt_u32 Zdr = 35;
	alt_u32 Zrr = 37;
	
	
	while(1) {	
	
				// First Dial
			DrawFilledCircle( Xx, Xy,Xdr,Dial_BG);
			draw_circle( Xx, Xy,Xrr,Dial_RIM);
			// Second Dial
			DrawFilledCircle( Yx, Yy,Ydr,Dial_BG);
			draw_circle( Yx, Yy,Yrr,Dial_RIM);
			// Third
			DrawFilledCircle( Zx, Zy,Zdr,Dial_BG);
			draw_circle( Zx, Zy,Zrr,Dial_RIM);
			
			
	}

	return 0;
}

