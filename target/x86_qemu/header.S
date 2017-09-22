	.section ".padding", "a"
	.byte	0
	
	.section ".header", "a"
	.byte 0xff, 0xff

	.byte 0
	.word 1
	.long 0			
	.word 0			
	.word 0xffff
	.word 0
	.word 0xAA55

	.global _start
_start:
	.byte	0xeb
	.byte	__setup-1f
1:
	.ascii "HdrS"
	.word 0x020c
	.word 0, 0
	.word 0
	.word 0	
	.byte 0
	.byte 1

	.code16
	.section ".text16", "ax"
__setup:
	jmp	__start16
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

