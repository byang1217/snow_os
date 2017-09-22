#include <snow_os.h>
#include <snow_internal.h>

#include "stm32l1xx_conf.h"
#include "stm32l1xx.h"

struct cm3_regs_t {
	long	sp;
	long	r4;
	long	r5;
	long	r6;
	long	r7;
	long	r8;
	long	r9;
	long	r10;
	long	r11;
	long	_lr;
	long	r0;
	long	r1;
	long	r2;
	long	r3;
	long	r12;
	long	lr;
	long	pc;
	long	psr;
};

long arch_get_pc(void *regs)
{
	return ((struct cm3_regs_t *)regs)->pc;

}

void arch_set_pc(void *regs, long pc)
{
	((struct cm3_regs_t *)regs)->pc = pc;

}

long arch_get_sp(void *regs)
{
	return ((struct cm3_regs_t *)regs)->sp;

}

void arch_set_sp(void *regs, long sp)
{
	((struct cm3_regs_t *)regs)->sp = sp;

}

long arch_get_arg(void *regs, int index)
{
	if (index == 0)
		return ((struct cm3_regs_t *)regs)->r0;
	else if (index == 1)
		return ((struct cm3_regs_t *)regs)->r1;
	arch_assert(0, "bad index\n");
	return 0;
}

void arch_set_arg(void *regs, int index, long arg)
{
	if (index == 0)
		((struct cm3_regs_t *)regs)->r0 = arg;
	else if (index == 1)
		((struct cm3_regs_t *)regs)->r1 = arg;
	else
		arch_assert(0, "bad index\n");
}


#define ENTRY(entry, handler) \
__attribute__((__used__))						\
static void __entry_##entry(void)					\
{									\
	asm volatile(							\
		".globl "#entry"			\n"			\
		".type "#entry", function 	\n"			\
		#entry":				\n"			\
		"sub sp, #40			\n"			\
		"mov r3, sp			\n"			\
		"stmia sp, {r3 - r11, lr}	\n"			\
		"mov r0, sp			\n"			\
		"ldr r1, ="#handler"		\n"			\
		"bl entry_handler		\n"			\
		"ldmia r0, {r3 - r11, lr}	\n"			\
		"mov sp, r3			\n"			\
		"add sp, #40			\n"			\
		"bx lr				\n"			\
	);								\
}

ENTRY(SVC_Handler, __snow_syscall_handler)
ENTRY(SysTick_Handler, __snow_tick_handler)

__attribute__((noinline))
int arch_syscall(int call, long arg)
{
	int ret;

	asm volatile (
		"mov r0, %1			\n"
		"mov r1, %2			\n"
		"svc 0				\n"
		"mov %0, r0			\n"
		:"=r"(ret)
		:"r"(call), "r"(arg)
		:"r0", "r1"
	);
	return ret;
}

void *entry_handler(struct cm3_regs_t *regs, void(*handler)(void *))
{
	current->sp = (void *)regs;
	handler(regs);
	__snow_sched();
	return current->sp;
}

void arch_disable_interrupt(void)
{
	asm volatile ("cpsid i");
}

void arch_enable_interrupt(void)
{
	asm volatile ("cpsie i");
}

void arch_init(void)
{
	SysTick_Config(SystemCoreClock/SNOW_HZ);
}

