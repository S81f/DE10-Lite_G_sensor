# This file generated on 2010.03.12.13:40:39

C_LIB_SRCS   +=   sierra.c sierra_info.c sierra_sem.c sierra_task.c sierra_time.c

ASM_LIB_SRCS += alt_exception_entry.S alt_exception_trap.S csw.S

INCLUDE_PATH += ../inc/altera_avalon_sierra_io.h ../inc/altera_avalon_sierra_ker.h ../inc/altera_avalon_sierra_regs.h ../inc/altera_avalon_sierra_tcb.h ../inc/altera_avalon_sierra_tcb_offset.h

# end of file