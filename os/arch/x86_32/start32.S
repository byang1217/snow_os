/*
 * Copyright (c) byang1217@gmail.com
 * Yang, Bin <byang1217@gmail.com> draft it on 2014
 */

#define __not_c__
#include "snow_os.h"

#define ENTRY(proc)\
	.align 2;\
	.globl proc;\
	.type proc,@function;\
	proc:
#define INTERRUPT_ENTRY(name,num)\
	ENTRY(name)\
	pushl $(num);\
	jmp _interrupt_entry

	.section ".start", "ax"
ENTRY(_start)
	movl	$_estack, %esp
	nop
	jmp	main


	.section ".text", "ax"
ENTRY(x86_32_entry)
_irq=0
.rept (IRQ_ENTRY_START + SNOW_IRQ_MAX)
	pushl $_irq
	jmp _interrupt_entry
	_irq=_irq+1
.endr

ENTRY(_interrupt_entry)
	pushal
	pushl %esp
	call x86_interrupt_entry
	movl %eax, %esp
	popal
	add $4,%esp
	iret


