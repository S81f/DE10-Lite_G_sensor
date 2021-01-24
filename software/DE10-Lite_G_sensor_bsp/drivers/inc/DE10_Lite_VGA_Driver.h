////////////////////////////////////////////
// Author: 	Linus Eriksson
// Date:	2017-04-15
// 
// Drivers for interfacing with VGA component
// for the DE10-Lite board.
//
// Functionality is implemented as macros that
// writes to and reads from registers.
//

#ifndef DE10LITEVGADRIVER_H_INCLUDED
#define DE10LITEVGADRIVER_H_INCLUDED

#include <system.h>
#include <io.h>

#define write_pixel(x,y,color) IOWR_32DIRECT(DE10_LITE_VGA_IP_0_BASE,((320*(y)+(x))*4),(color))
#define read_pixel(x,y) IORD_32DIRECT(DE10_LITE_VGA_IP_0_BASE,(320*(y)+(x))*4)

#endif // DE10LITEVGADRIVER_H_INCLUDED