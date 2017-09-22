#ifndef __snow_arch_h__
#define __snow_arch_h__
#include "x86_32.h"

#define SNOW_OS_DEBUG

#define SNOW_SYS_TIMER		0
#define SNOW_SYS_CALL		3
#define IRQ_ENTRY_START		8
#define IRQ0_OFFSET		32 /* IRQ 0: =32 */

#define SNOW_IRQ_MAX		8
#define SNOW_HZ			100
#define SNOW_THREAD_SCHED_US	10000
#define SNOW_MIN_STACK		(512)

#ifndef __not_c__
#ifndef SNOW_OS_DEBUG
#define snow_print(...) do{}while(0)
#endif
#define arch_assert(cond, str)	\
	do{if (!(cond)) {snow_print("SNOW assert(%s, %d): %s\n", __func__, __LINE__, str); while(1);}}while(0)
void arch_init(void);
void arch_putc(int c);
int arch_getc(void);
#endif
#endif

