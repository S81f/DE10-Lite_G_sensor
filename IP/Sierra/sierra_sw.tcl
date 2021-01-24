#
# sierra_driver.tcl
#

# Create a new driver
create_driver sierra_driver

# Associate it with some hardware known as "altera_avalon_uart"
set_sw_property hw_class_name sierra

# The version of this driver
set_sw_property version 8

# This driver may be incompatible with versions of hardware less
# than specified below. Updates to hardware and device drivers
# rendering the driver incompatible with older versions of
# hardware are noted with this property assignment.
#
# Multiple-Version compatibility was introduced in version 7.1;
# prior versions are therefore excluded.
set_sw_property min_compatible_hw_version 1.0

# Initialize the driver in alt_sys_init()
# set_sw_property auto_initialize true

# Location in generated BSP that above sources will be copied into
set_sw_property bsp_subdirectory drivers

#
# Source file listings...
#

# C/C++ source files
add_sw_property c_source HAL/src/sierra.c
add_sw_property c_source HAL/src/sierra_info.c
add_sw_property c_source HAL/src/sierra_sem.c
add_sw_property c_source HAL/src/sierra_task.c
add_sw_property c_source HAL/src/sierra_time.c
add_sw_property asm_source HAL/src/csw.S
add_sw_property asm_source HAL/src/alt_exception_trap.S
add_sw_property asm_source HAL/src/alt_exception_entry.S



# Include files
add_sw_property include_source HAL/inc/altera_avalon_sierra_io.h
add_sw_property include_source HAL/inc/altera_avalon_sierra_ker.h
add_sw_property include_source HAL/inc/altera_avalon_sierra_name.h
add_sw_property include_source HAL/inc/altera_avalon_sierra_regs.h
add_sw_property include_source HAL/inc/altera_avalon_sierra_tcb.h
add_sw_property include_source HAL/inc/altera_avalon_sierra_tcb_offset.h

# This driver supports HAL type
add_sw_property supported_bsp_type HAL


# End of file
