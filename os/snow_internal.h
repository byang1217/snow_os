#ifndef __snow_internal_h__
#define __snow_internal_h__

#include "snow_arch.h"

extern struct snow_thread_t *current, *idle;
extern snow_isr_t snow_irq_table[SNOW_IRQ_MAX];

void arch_idle(unsigned int us);
unsigned long long arch_time_fixup(unsigned long long us);
long arch_get_pc(void *regs);
void arch_set_pc(void *regs, long pc);
void arch_set_sp(void *regs, long sp);
int arch_syscall(void);
void arch_disable_interrupt(void);
void arch_enable_interrupt(void);

void __snow_sched(void);
void __snow_syscall_handler(void *regs);
void __snow_tick_handler(void);
void __snow_dump(void);

#endif

