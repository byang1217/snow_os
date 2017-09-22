#include "snow_os.h"
#include "snow_internal.h"
#include "snow_arch.h"

void arch_idle(unsigned int us)
{
}

unsigned long long arch_time_fixup(unsigned long long us)
{
	return us;
}

void arch_set_sp(void *regs, long sp)
{
}

long arch_get_pc(void *regs)
{
	return ((u32 *)regs)[9];
}

void arch_set_pc(void *regs, long pc)
{
	((u32 *)regs)[9] = pc;
}

int arch_syscall(void)
{
	asm("int $" SYMBOL_STR(SNOW_SYS_CALL));
}

void arch_init(void)
{
	init_idt();
}

