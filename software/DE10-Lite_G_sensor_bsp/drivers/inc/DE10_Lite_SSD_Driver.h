////////////////////////////////////////////
// Author: 	Linus Eriksson
// Date:	2017-04-14
// 
// Drivers for the Sevent Segment Display component
// for use with the DE10-Lite board.
//
// Functionality is implemented as macros doing
// writing to registers.
// 
// ssd_enable accepts a parameter which can be
// any combination of HEX ID values.
// A HEX_ALL_ENABLE definition can be used to
// enable all displays, value of 0 turns all off.
// 
// ssd_write takes in a ID for the data register
// and value to be writen.
// The data register IDs can be found below
// as defines.
//
// Example:
// To display F in display HEX0 use
// ssd_write(HEX0_DATA_ID,0xf)
//
// The decimal point can be displayed
// by using the OR operator on data value
// and HEX_DECIMAL.

#ifndef DE10LITESSDDRIVER_H_INCLUDED
#define DE10LITESSDDRIVER_H_INCLUDED

#include <system.h>
#include <io.h>

#define HEX0_ENABLE_ID	0x01
#define HEX1_ENABLE_ID	0x02
#define HEX2_ENABLE_ID	0x04
#define HEX3_ENABLE_ID	0x08
#define HEX4_ENABLE_ID	0x10
#define HEX5_ENABLE_ID	0x20
#define HEX_ALL_ENABLE	0x3f

#define HEX0_DATA_ID	0x00
#define HEX1_DATA_ID	0x01
#define HEX2_DATA_ID	0x02
#define HEX3_DATA_ID	0x03
#define HEX4_DATA_ID	0x04
#define HEX5_DATA_ID	0x05

#define HEX_DECIMAL		0x80

#define ssd_enable(displays) IOWR_32DIRECT(DE10_LITE_SSD_IP_0_BASE,0,(displays))
#define ssd_write(display,value) IOWR_32DIRECT(DE10_LITE_SSD_IP_0_BASE,4+(4*(display)),(value))

#endif // DE10LITESSDDRIVER_H_INCLUDED