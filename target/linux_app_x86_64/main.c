#include "snow_os.h"
#include <stdlib.h>
#include <time.h>

extern unsigned long _estack, _sstack;

void *idle_stack = &_estack;
static int stack[9][4096];
static struct snow_sem_t test_sem;
static struct snow_mutex_t test_mutex;
static struct snow_timer_t test_timer;

extern struct snow_thread_t *current, *idle;
static void mutex_thread(void *p)
{
	while (1) {
		snow_mutex_lock(&test_mutex);
		snow_print("%s\n", current->name);
		snow_usleep(rand()%100000);
		snow_mutex_unlock(&test_mutex);
	}
}

static void sem_thread(void *p)
{
	while (1) {
		snow_sem_down(&test_sem);
		snow_print("%s\n", current->name);
		snow_usleep(rand()%100000);
		snow_sem_up(&test_sem);
	}
}
static void timer_func(void *p)
{
	/* add self again */
	snow_timer_add(&test_timer, 100000);
	snow_print("Timer trigger\n");
}

static void main_thread(void *p)
{
	int poll = 1;
	srand(time(NULL));

	snow_sem_init(&test_sem, "test sem", 1);
	snow_mutex_init(&test_mutex, "test mutex");

	snow_timer_init(&test_timer, "test timer", timer_func, NULL);
	snow_timer_add(&test_timer, 100000);

	snow_thread_create("test mutex thread 1", mutex_thread, NULL, stack[1], 4096*sizeof(int));
	snow_thread_create("test mutex thread 2", mutex_thread, NULL, stack[2], 4096*sizeof(int));
	snow_thread_create("test mutex thread 3", mutex_thread, NULL, stack[3], 4096*sizeof(int));
	snow_thread_create("test mutex thread c", mutex_thread, NULL, stack[4], 4096*sizeof(int));

	snow_thread_create("test sem thread a", sem_thread, NULL, stack[5], 4096*sizeof(int));
	snow_thread_create("test sem thread b", sem_thread, NULL, stack[6], 4096*sizeof(int));
	snow_thread_create("test sem thread c", sem_thread, NULL, stack[7], 4096*sizeof(int));
	snow_thread_create("test sem thread d", sem_thread, NULL, stack[8], 4096*sizeof(int));

	while (1) {
		snow_print("%s\n", current->name);
		poll = !poll;
		if (poll)
			snow_udelay(rand()%100000);
		else
			snow_usleep(rand()%100000);
	}
}

int main(int argc, char **argv)
{
	snow_start(main_thread, stack[0], 4096*4, idle_stack);
	/* no return from snow_start */
} 

