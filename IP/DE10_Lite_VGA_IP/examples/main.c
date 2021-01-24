////////////////////////////////////////////
// Author: 	Linus Eriksson
// Date:	2017-04-15
// 
// Example code for using the VGA driver on
// the DE10-Lite board.
//
// This example will first initiate a test
// where a color is written to VRAM as position
// (100,100) and then read back to compare
// values, ensuring both writes and reads work.
//
// After this the program will draw a box that
// bounces on the screens borders.
// The update frequency is limited by a 
// busy wait loop.

#include <system.h>
#include <stdio.h>
#include <alt_types.h>
#include <DE10_Lite_VGA_Driver.h>

void clear_screen(alt_u32 color) {
	alt_u32 x,y;
	for(y=0;y<240;y++) {
		for(x=0;x<320;x++) {
			write_pixel(x,y,color);
		}
	}
}

alt_u32 read_write_test() {
	alt_u32 write_color=0xf0f;
	write_pixel(100,100,write_color);
	alt_u32 read_color=read_pixel(100,100);
	
	if(read_color=write_color) {
		return 1;
	}
	
	return 0;
}

void draw_box(alt_u32 box_x,alt_u32 box_y,alt_u32 color) {
	alt_u32 x,y;
	for(y=0;y<10;y++) {
		for(x=0;x<10;x++) {
			write_pixel(box_x+x,box_y+y,color);
		}
	}
}

int main() {
	
	printf("Starting system\n");
	
	
	// Initiate read and write test
	if(read_write_test()) {
		printf("Reading and writing to VRAM works\n");
	} else {
		printf("Error writing and/or reading to VRAM\n");
	}
	
	// Clear screen and set up box start values
	alt_u32 background_color=0x000;
	clear_screen(background_color);
	
	alt_u32 box_color=0xfff;
	float box_x=50.0f;
	float box_y=50.0f;
	float box_velocity_x=1.0f;
	float box_velocity_y=0.8f;
	
	
	while(1) {
		// clear previous box to background color
		draw_box((alt_u32)box_x,(alt_u32)box_y,background_color);
		
		// Calculate new box position
		float box_increment_x=box_x+box_velocity_x;
		float box_increment_y=box_y+box_velocity_y;
	
		if(box_increment_x>=310.0f||box_increment_x<=0.0f) {
			box_velocity_x*=-1.0f;
		}
		if(box_increment_y>=230||box_increment_y<=0.0f) {
			box_velocity_y*=-1.0f;
		}
		
		box_x+=box_velocity_x;
		box_y+=box_velocity_y;
		
		
		// draw new position
		draw_box((alt_u32)box_x,(alt_u32)box_y,box_color);

		int i;
		for(i=0;i<1000000;i++) {};
	}	
	
	return 0;
}
