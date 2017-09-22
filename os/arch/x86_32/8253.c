#include "x86_32.h"
#include "snow_arch.h"

void init_8253(void)
{
	outb(0x34, 0x43);
	outb(((1193182 + SNOW_HZ/2)/SNOW_HZ) % 256, 0x40);
	outb(((1193182 + SNOW_HZ/2)/SNOW_HZ) / 256, 0x40);
}

int read_8253_timer0(void)
{
	int time;

	outb(0, 0x43);
	time = inb(0x40);
	time += inb(0x40) << 8;
	return time;
}

