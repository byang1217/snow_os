#include "snow_os.h"
#include "snow_internal.h"

struct gate_desc_t {
	unsigned gd_off_15_0 : 16;   // low 16 bits of offset in segment
	unsigned gd_ss : 16;         // segment selector
	unsigned gd_args : 5;        // # args, 0 for interrupt/trap gates
	unsigned gd_rsv1 : 3;        // reserved(should be zero I guess)
	unsigned gd_type :4;         // type(STS_{TG,IG32,TG32})
	unsigned gd_s : 1;           // must be 0 (system)
	unsigned gd_dpl : 2;         // descriptor(meaning new) privilege level
	unsigned gd_p : 1;           // Present
	unsigned gd_off_31_16 : 16;  // high bits of offset in segment
};

/* Pseudo-descriptors used for LGDT, LLDT and LIDT instructions*/
struct pseudo_desc_t {
	u16 pd__garbage;         // LGDT supposed to be from address 4N+2
	u16 pd_lim;              // Limit
	u32 pd_base __attribute__ ((packed));       // Base address
};

#define SET_GATE(gate, istrap, sel, off, dpl)			\
{								\
	(gate).gd_off_15_0 = (u32) (off) & 0xffff;	\
	(gate).gd_ss = (sel);					\
	(gate).gd_args = 0;					\
	(gate).gd_rsv1 = 0;					\
	(gate).gd_type = (istrap) ? 0xf : 0xe;	\
	(gate).gd_s = 0;					\
	(gate).gd_dpl = dpl;					\
	(gate).gd_p = 1;					\
	(gate).gd_off_31_16 = (u32) (off) >> 16;		\
}

static struct gate_desc_t idt[IRQ_ENTRY_START + SNOW_IRQ_MAX];
struct pseudo_desc_t idt_pd = {0, sizeof(idt) - 1, (unsigned long) idt};
extern void x86_32_entry(void);

void init_idt(void)
{	
	int i, j;
	u32 entry = (u32)x86_32_entry;

	for (i = 0; i < sizeof(idt)/sizeof(idt[0]); i++)
		SET_GATE(idt[i], 0, __BOOT_CS, entry + (i*7), 0);
	asm volatile("lidt idt_pd + 2");
}

void x86_eoi(int irq);
struct snow_thread_t *old_current;
u32 x86_interrupt_entry(u32 sp)
{
	int entry = *(u32 *)(sp + 32);
	int irq = entry - IRQ_ENTRY_START;
	
	old_current = current;
	current->sp = (char *)sp;
	if (entry == SNOW_SYS_CALL) {
		__snow_syscall_handler(current->sp);
	} else if (entry < IRQ_ENTRY_START) {
		snow_print("exception (%x) at IP: %x\n",
			*(u32 *)(sp + 32), *(u32 *)(sp + 36));
		while(1);
	} else {
		if (snow_irq_table[irq] == NULL) {
			snow_print("irq: %d assert before request\n", irq);
		}else {
			snow_irq_table[irq]();
		}
		x86_eoi(0);
	}
	__snow_sched();
	return (u32)current->sp;
}
