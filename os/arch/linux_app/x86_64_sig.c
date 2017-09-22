#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "snow_os.h"
#include "snow_internal.h"
#include "snow_arch.h"

void init_thread(void);
static int exit_idle;
static struct itimerval sig_timer_value = {
	.it_value.tv_usec = 1000*1000/SNOW_HZ,
	.it_interval.tv_usec = 1000*1000/SNOW_HZ,
};

#define SIG_SYSCALL	SIGUSR1
#define SIG_TICK	SIGVTALRM
static sigset_t sig_mask, sig_unmask;

static void handle_sig(int sig)
{
	static void *sp;

	asm("mov %%rsp, %0":"=r"(current->sp));
	sp = current->sp;
	if (sig == SIGINT) {
		__snow_dump();
		printf("Press 'x' to exit, others to continue ... ...");
		if (getchar() == 'x')
			exit(0);
	} else if (sig == SIG_TICK) {
		setitimer(ITIMER_VIRTUAL, &sig_timer_value, NULL);
		__snow_tick_handler();
	} else if (sig == SIG_SYSCALL) {
		__snow_syscall_handler(current->sp);
	} else {
		arch_assert(0, "unknown sig");
	}
	__snow_sched();
	if (sp != current->sp) {
		memcpy(current->sp, sp, 2*(sizeof(long)));
		exit_idle = 1;
	}
	asm("mov %0, %%rbp"::"r"(current->sp + 2*sizeof(long)));
	asm("mov %0, %%rsp"::"r"(current->sp));
}

void arch_idle(unsigned int us)
{
	printf("idle %d us\n", us);
	snow_interrupt_disable();
	exit_idle = us == 0 ? 1 : 0;
	snow_interrupt_enable();
	while(1) {
		snow_interrupt_disable();
		if(exit_idle)
			break;
		snow_interrupt_enable();
	}
	snow_interrupt_enable();
}

unsigned long long arch_time_fixup(unsigned long long us)
{
	return us;
}

void arch_set_sp(void *regs, long sp)
{
	((long *)regs)[24] = sp;
}

long arch_get_pc(void *regs)
{
	return ((long *)regs)[25];
}

void arch_set_pc(void *regs, long pc)
{
	((long *)regs)[25] = pc;
}

int arch_syscall(void)
{
	kill(getpid(), SIG_SYSCALL);
}

void snow_enable_irq(int irq)
{
}

void snow_disable_irq(int irq)
{
}

void arch_disable_interrupt(void)
{
	if (sigprocmask(SIG_BLOCK, &sig_mask, NULL) < 0)
		arch_assert(0, "mask sig");
}

void arch_enable_interrupt(void)
{
	if (sigprocmask(SIG_UNBLOCK, &sig_mask, NULL) < 0)
		arch_assert(0, "mask sig");
}

void arch_init(void)
{
	setitimer(ITIMER_VIRTUAL, &sig_timer_value, NULL);
}

unsigned long _estack, _sstack;
__attribute__ ((constructor))
void __arch_init(void)
{
	extern void *idle_stack;

	asm("mov %%rsp, %0":"=r"(idle_stack));
	signal(SIG_TICK, handle_sig);
	signal(SIG_SYSCALL, handle_sig);
	signal(SIGINT, handle_sig);
	sigemptyset(&sig_mask);
	sigaddset(&sig_mask, SIG_TICK);
	sigaddset(&sig_mask, SIG_SYSCALL);
	sigaddset(&sig_mask, SIGINT);
}

