#include "snow_os.h"

static u8 irq_mask_8259 = 0xFF;

void init_8259(void)
{
	outb(0x20, 0x11);
	io_delay();
	outb(0x21, 32);
	io_delay();
	outb(0x21, 4);
	io_delay();
	outb(0x21, 1);
	io_delay();
	outb(0x21, irq_mask_8259);
	io_delay();
}

void x86_eoi(int irq)
{
	outb(0x20, 0x20);
}

void snow_enable_irq(int irq)
{
	snow_interrupt_disable();
	irq_mask_8259 = irq_mask_8259 & ~(1<<irq);
	outb(0x20+1, irq_mask_8259);
	snow_interrupt_enable();
}

void snow_disable_irq(int irq)
{
	snow_interrupt_disable();
	irq_mask_8259 = irq_mask_8259 | (1<<irq);
	outb(0x20+1, irq_mask_8259);
	snow_interrupt_enable();
}

