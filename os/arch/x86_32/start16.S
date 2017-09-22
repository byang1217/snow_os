/*
 * Copyright (c) byang1217@gmail.com
 * Yang, Bin <byang1217@gmail.com> draft it on 2014
 */

#define __not_c__
#include "snow_os.h"
#define X86_CR0_PE              1

	.globl __start16

	.section ".text16"
	.code16
__start16:
	cli
	movw	%ds,%ax
	movzwl  %ax, %eax
	sall    $4, %eax
	addl	%eax, gdt_ptr
	addl	%eax, 2f

	leal    gdt, %eax
	lgdtw	(%eax)

	jmp	1f
1:
	movw	$__BOOT_DS, %cx
	movw	$__BOOT_TSS, %di

	movl	%cr0, %edx
	orb	$X86_CR0_PE, %dl
	movl	%edx, %cr0

	.byte	0x66, 0xea		# ljmpl opcode
2:	.long	__setup32		# offset
	.word	__BOOT_CS		# segment

	.code32
	.section ".text32","ax"
	.global __setup32
__setup32:
	movl	%ecx, %ds
	movl	%ecx, %es
	movl	%ecx, %fs
	movl	%ecx, %gs
	movl	%ecx, %ss

	movl	$START32_ADDR, %eax
	jmpl	*%eax

	
	.align 8
	.global gdt
	.global gdt_entry
	.global gdt_size
gdt:
gdt_size:
        .word   gdt_end - gdt_entry -1
gdt_ptr:
        .long   gdt_entry
        .word   0
gdt_entry:
        .quad   0x0000000000000000      /* NULL descriptor */
        .quad   0x0000000000000000      /* NULL descriptor */
        .quad   0x00cf9b000000ffff      /* __KERNEL_CS */
        .quad   0x00cf93000000ffff      /* __KERNEL_DS */
        .quad   0x0000890010000067      /* TS descriptor */
gdt_end:

