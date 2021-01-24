#ifndef __ALTERA_AVALON_SIERRA_REGS_H__
#define __ALTERA_AVALON_SIERRA_REGS_H__

#include <altera_avalon_sierra_name.h>
#include <altera_avalon_sierra_io.h>
#include <system.h>

// Altera HAL Macros for NIOS
#define M_Sierra_HW_Version_reg IORD_32DIRECT(SIERRA_RTOS_BASE,0);
#define M_RD_SierraTime_base_reg IORD_32DIRECT(SIERRA_RTOS_BASE,0x10);
#define M_IOWR_SierraTime_base_reg(data)  IOWR_32DIRECT(SIERRA_RTOS_BASE,0x10, data); // Ej kollat!
#define M_RD_Sierra_statusA_reg IORD_32DIRECT(SIERRA_RTOS_BASE,0x20);
#define M_RD_Sierra_statusB_reg IORD_32DIRECT(SIERRA_RTOS_BASE,0x30);
#define IOWR_ALT_SVC_REGISTER(data)  IOWR_32DIRECT(SIERRA_RTOS_BASE,0x40, data);
#define IOWR_ALT_CTRL_REGISTER(data)  IOWR_32DIRECT(SIERRA_RTOS_BASE,0x50, data);
#define M_WD_SW_RESET_REGISTER(data)  IOWR_32DIRECT(SIERRA_RTOS_BASE,0x60, data);

/*
 * Memory mapped reigisters, can perhaps use in the most systems
 * For Native bus sizing
 * 0x80000000 = set bit 32 to bypass cache in NIOS
 */

// volatile unsigned int *SierraVersion_reg    = (volatile unsigned int *) (0x80000000 | SIERRA_RTOS_BASE);
//volatile unsigned int *SierraTime_base_reg_point  = (volatile unsigned int *) (0x80000000 | (SIERRA_RTOS_BASE+0x10) );
// #define M_RD_Sierra_statusA_reg (volatile int*)(0x80000000|(SIERRA_RTOS_BASE+0x20)) 
//volatile statusA_t    *CPUstatusA_reg       = (volatile statusA_t *)    (0x80000000 | (SIERRA_RTOS_BASE+0x20) );
//volatile statusB_t    *CPUstatusB_reg       = (volatile statusB_t *)    (0x80000000 | (SIERRA_RTOS_BASE+0x30) );
//volatile svc_t        *CPUsvc_request_reg   = (volatile svc_t *)        (0x80000000 | (SIERRA_RTOS_BASE+0x40) );
//volatile ctrl_t       *CPUctrl_reg          = (volatile ctrl_t*)        (0x80000000 | (SIERRA_RTOS_BASE+0x50) );
//volatile unsigned int *CPUctrl_sw_reset_reg  = (volatile unsigned int *) (0x80000000 | (SIERRA_RTOS_BASE+0x60) );
/* volatile  int *CPUctrl_sw_reset_reg  = (volatile  int *) (0x80000000 | (SIERRA_RTOS_BASE+0x50) ); */

#endif /* __ALTERA_AVALON_SIERRA_REGS_H__ */
