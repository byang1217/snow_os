#ifndef __snow_arch_h__
#define __snow_arch_h__

#define SNOW_IRQ_MAX		8
#define SNOW_SYS_TIMER		0	/*not used*/
#define SNOW_SYS_CALL		0	/*not used*/

#define SNOW_HZ			100
#define SNOW_THREAD_SCHED_US	10000
#define SNOW_MIN_STACK		(1024*4)

#define SNOW_OS_DEBUG

#ifndef __not_c__
#define snow_print(...) do{snow_interrupt_disable(); printf(__VA_ARGS__); snow_interrupt_enable();}while(0)
#define arch_assert(cond, str)	\
	do{if (!(cond)) {snow_print("SNOW assert(%s, %d): %s\n", __func__, __LINE__, str); while(1);}}while(0)
void arch_init(void);
#endif

#endif

