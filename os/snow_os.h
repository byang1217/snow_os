#ifndef __snow_os_h__
#define __snow_os_h__

#define SNOW_OS_VERSION	"0.11"

#define __SYMBOL_STR(x) #x
#define SYMBOL_STR(x) __SYMBOL_STR(x)
#include <snow_arch.h>

#ifndef __not_c__
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

struct list_t {
	struct list_t *next, *prev;
};

struct snow_mutex_t {
	struct list_t	list; 
	char		*name;
	void		*thread;
	int		count;
};

struct snow_sem_t {
	struct list_t	list; 
	char		*name;
	int		count;
};

struct snow_timer_t {
	char		*name;
	unsigned long long us;
	void(*timer_fn)(void *);
	void		*arg;
	struct list_t	list; 
};

struct snow_thread_t {
	long		arg1;
	long		arg2;
	char		*sp;
	char		*name;
	int		flags;
	void(*thread_fn)(void *);
	void		*thread_arg;
	int		stack_size;
	char		*stack;
	struct list_t	list; 
	struct list_t	all_list; 
	void		*wait_list;
	unsigned long long timeout;
	unsigned int	sched_us;
	struct snow_timer_t wakeup_timer;
};

void snow_start(void(*main)(void *),
		void *main_sp,
		int main_stack_size,
		void *idle_stack);
void *snow_thread_create(char *name, 
		void(*thread_fn)(void *),
		void *arg,
		void *sp,
		int stack_size);
void snow_sched(void);
void snow_timer_init(struct snow_timer_t *timer,
		char *name,
		void(*timer_fn)(void *),
		void *arg);
void snow_timer_add(struct snow_timer_t *timer, unsigned int us);
void snow_timer_del(struct snow_timer_t *timer);
void snow_sem_init(struct snow_sem_t *sem, char *name, int count);
int snow_sem_down_timeout(struct snow_sem_t *sem, unsigned int us);
#define snow_sem_down(sem) do{while(snow_sem_down_timeout(sem, -1) != 0);}while(0)
#define snow_sem_trydown(sem) snow_sem_down_timeout(sem, 0)
void snow_sem_up(struct snow_sem_t *sem);
void snow_mutex_init(struct snow_mutex_t *mutex, char *name);
int snow_mutex_lock_timeout(struct snow_mutex_t *mutex, unsigned int us);
#define snow_mutex_lock(mutex) do{while(snow_mutex_lock_timeout(mutex, -1) != 0);}while(0)
#define snow_mutex_trylock(mutex) snow_mutex_lock_timeout(mutex, 0)
void snow_mutex_unlock(struct snow_mutex_t *mutex);
unsigned long long snow_gettime_us(void);
void snow_usleep(unsigned int us);
void snow_udelay(unsigned int us);

void snow_interrupt_enable(void);
void snow_interrupt_disable(void);
void snow_enable_irq(int irq);
void snow_disable_irq(int irq);
typedef void(*snow_isr_t)(void);
void snow_request_irq(int irq, snow_isr_t isr);

#ifndef snow_print
int snow_print(const char *fmt, ...);
int snow_snprintf(char *buf, int size, const char *fmt, ...);
int snow_vsnprintf(char *buf, int size, const char *fmt, va_list args);
#endif
#endif
#endif

