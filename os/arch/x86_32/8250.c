#include "x86_32.h"

#define XMTRDY          0x20
#define DLAB		0x80
#define TXR             0       /*  Transmit register (WRITE) */
#define RXR             0       /*  Receive register  (READ)  */
#define IER             1       /*  Interrupt Enable          */
#define IIR             2       /*  Interrupt ID              */
#define FCR             2       /*  FIFO control              */
#define LCR             3       /*  Line control              */
#define MCR             4       /*  Modem control             */
#define LSR             5       /*  Line Status               */
#define MSR             6       /*  Modem Status              */
#define DLL             0       /*  Divisor Latch Low         */
#define DLH             1       /*  Divisor latch High        */

#define DEFAULT_BAUD 115200
#define DEFAULT_PORT 0x3f8

int arch_getc(void)
{
	return 0;
}

void arch_putc(int ch)
{
        unsigned timeout = 0xffff;

        while ((inb(DEFAULT_PORT + LSR) & XMTRDY) == 0 && --timeout)
                ;
        outb(ch, DEFAULT_PORT + TXR);
}

void init_8250(void)
{
	unsigned char c;
	unsigned divisor;
	int port = DEFAULT_PORT;
	int baud = DEFAULT_BAUD;

	outb(0x3, port + LCR);	/* 8n1 */
	outb(0, port + IER);	/* no interrupt */
	outb(0, port + FCR);	/* no fifo */
	outb(0x3, port + MCR);	/* DTR + RTS */

	divisor	= 115200 / baud;
	c = inb(port + LCR);
	outb(c | DLAB, port + LCR);
	outb(divisor & 0xff, port + DLL);
	outb((divisor >> 8) & 0xff, port + DLH);
	outb(c & ~DLAB, port + LCR);
}
