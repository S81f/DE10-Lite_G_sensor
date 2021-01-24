////////////////////////////////////////////
// Author: 	Linus Eriksson
// Date:	2017-04-14
// 
// Example using the Seven Segment Display driver for
// DE10-Lite board. The hardware implements display of
// hexadecimal numbers. Each display has a range from
// 0x0 to 0xf. A decimal point can be activated by using
// the HEX_DECIMAL constant as a flag with the data.
//
#include <system.h>
#include <io.h>
#include <stdio.h>
#include <alt_types.h>
#include <DE10_Lite_SSD_Driver.h>

int main() {
	printf("Starting system\n");
	
	// Enables all displays
	// HEX_ALL_ENABLE is equivalent to
	// (HEX0_ENABLE_ID|HEX1_ENABLE_ID|HEX2_ENABLE_ID|HEX3_ENABLE_ID|HEX4_ENABLE_ID|HEX5_ENABLE_ID)
	// and these flags can be used in any combination to enable displays as needed.
	ssd_enable(HEX_ALL_ENABLE);
	

	// This program will write the decimal number of ticks to the sevent segment displays
	alt_u32 ticks=0;	
	while(1) {	
		ssd_write(HEX0_DATA_ID,(ticks%10));
		ssd_write(HEX1_DATA_ID,(ticks/10)%10);
		ssd_write(HEX2_DATA_ID,(ticks/100)%10);
		ssd_write(HEX3_DATA_ID,(ticks/1000)%10);
		ssd_write(HEX4_DATA_ID,(ticks/10000)%10);
		ssd_write(HEX5_DATA_ID,(ticks/100000)%10);
		ticks++;
		for(alt_u32 i=0;i<100000;i++) {}		
	}
	
	return 0;
}
