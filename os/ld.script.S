#undef i386
#define __not_c__
#include "snow_os.h"

OUTPUT_FORMAT(SYMBOL_STR(LD_OUTPUT_FORMAT), SYMBOL_STR(LD_OUTPUT_FORMAT), SYMBOL_STR(LD_OUTPUT_FORMAT))
OUTPUT_ARCH(LD_OUTPUT_ARCH)
ENTRY(_start)

SECTIONS
{
	. = LD_START_ADDR;
	.text :
	{
		_stext = .;
		*(.start)
		. = ALIGN(4);
		*(.text)
		*(.text.*)
		*(.text .text.*)
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
		_etext = .;
	}

	.initdata :
	{
		_s_init = .;
		KEEP (*(.init.*))
		_s_init = .;
	}

	__data_in_rom__ = .;

#ifdef LD_DATA_ADDR
	. = LD_DATA_ADDR;
#endif
	.data :
	{
		_sdata = . ;
		*(.data)
		*(.data.*)
		*(.data.data.*)
		. = ALIGN(4);
		*(.idle_stack)
		_edata = . ;
	}

	.zdata : 
	{
		. = ALIGN(4);
		__zdata_start__ = .;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__zdata_end__ = .;
	}

	.stack : 
	{
		. = ALIGN(4);
		_sstack = .;
		. = . + LD_STACK_SIZE;
		_estack = .;
	}
	
	.nouse :
	{
		*(.got*)
		*(.note*)
	}
}
