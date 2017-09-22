/*
 * Copyright (c) byang1217@gmail.com
 * Yang, Bin <byang1217@gmail.com> draft it based on linux kernel. GPL2 license
 */
#ifndef __X86_32_H__
#define __X86_32_H__

#define __BOOT_CS               16
#define __BOOT_DS               24
#define __BOOT_TSS              32

#ifndef __not_c__
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;
typedef unsigned int		addr_t;

#define __text16 __attribute__((section(".text16")))
#define __data16 __attribute__((section(".data16")))
#define __packed __attribute__((packed))

static inline void outb(u8 v, u16 port)
{
        asm volatile("outb %0,%1" : : "a" (v), "dN" (port));
}
static inline u8 inb(u16 port)
{
        u8 v;
        asm volatile("inb %1,%0" : "=a" (v) : "dN" (port));
        return v;
}
static inline void outw(u16 v, u16 port)
{
	asm volatile("outw %0,%1" : : "a" (v), "dN" (port));
}
static inline u16 inw(u16 port)
{
	u16 v;
	asm volatile("inw %1,%0" : "=a" (v) : "dN" (port));
	return v;
}
static inline void outl(u32 v, u16 port)
{
	asm volatile("outl %0,%1" : : "a" (v), "dN" (port));
}
static inline u32 inl(u16 port)
{
	u32 v;
	asm volatile("inl %1,%0" : "=a" (v) : "dN" (port));
	return v;
}
static inline void io_delay(void)
{
	const u16 DELAY_PORT = 0x80;
	asm volatile("outb %%al,%0" : : "dN" (DELAY_PORT));
}
static inline u16 ds(void)
{
	u16 seg;
	asm("movw %%ds,%0" : "=rm" (seg));
	return seg;
}
static inline void set_fs(u16 seg)
{
	asm volatile("movw %0,%%fs" : : "rm" (seg));
}
static inline u16 fs(void)
{
	u16 seg;
	asm volatile("movw %%fs,%0" : "=rm" (seg));
	return seg;
}
static inline void set_gs(u16 seg)
{
	asm volatile("movw %0,%%gs" : : "rm" (seg));
}
static inline u16 gs(void)
{
	u16 seg;
	asm volatile("movw %%gs,%0" : "=rm" (seg));
	return seg;
}

static inline void arch_disable_interrupt(void)
{
        asm volatile("cli": : :"memory");
}

static inline void arch_enable_interrupt(void)
{
        asm volatile("sti": : :"memory");
}

void __start32(void);
void __start16(void);
#endif
#endif
