arch = x86_32
qemu_load_address = 0x10000
header_size = 4096
stack_size = 1024
start32_addr = `echo $$(($(qemu_load_address) + $(header_size)))`
CFLAGS += -Os -march=i386 -m32 \
		-DLD_OUTPUT_ARCH=i386 \
		-DLD_OUTPUT_FORMAT=elf32-i386 \
		-DLD_START_ADDR=$(start32_addr) \
		-DLD_STACK_SIZE=$(stack_size)
LDFLAGS += -m elf_i386 
HEADER_CFLAGS = -Os -march=i386 -m32 -DSTART32_ADDR=$(start32_addr) -Itarget/$(target) -Ios/arch/$(arch)/ -Ios 
HEADER_LDFLAGS += -m elf_i386

target_src += os/arch/x86_32/start32.S
target_src += os/arch/x86_32/x86_32.c
target_src += os/arch/x86_32/8250.c
target_src += os/arch/x86_32/8253.c
target_src += os/arch/x86_32/8259.c
target_src += os/arch/x86_32/interrupt.c
target_src += target/x86_qemu/main.c
target_src += os/snow.c

out/$(target)/$(target).bin: out/$(target)/$(target).elf
out/$(target)/$(target).elf: $(objs-final) $(ld-script)
	$(Q)echo "Create $@"
	$(CC) -o $@ $(objs-final) -nostdlib -Wl,-Map=out/$(target)/$(target).map -T $(ld-script)
	$(Q)$(OBJDUMP) -D $@ > $(patsubst %.elf,%.dump,$@)
	$(Q)$(OBJCOPY) -O binary out/$(target)/$(target).elf out/$(target)/$(target).bin

out/$(target)/header.padding: out/$(target)/header.bin
	dd if=out/$(target)/header.bin ibs=$(header_size) count=1 of=out/$(target)/header.padding conv=sync 2> /dev/null
out/$(target)/header.bin: out/$(target)/header.elf
	objcopy -O binary out/$(target)/header.elf out/$(target)/header.bin
out/$(target)/header.elf: target/x86_qemu/header.S os/arch/x86_32/start16.S
	$(Q)mkdir -p out/$(target)/target/x86_qemu/
	$(Q)mkdir -p out/$(target)/os/arch/x86_32/
	$(Q)$(CC) $(HEADER_CFLAGS) target/x86_qemu/header.S -c -o out/$(target)/target/x86_qemu/header.o
	$(Q)$(CC) $(HEADER_CFLAGS) os/arch/x86_32/start16.S -c -o out/$(target)/os/arch/x86_32/start16.o
	$(Q)$(LD) -T target/x86_qemu/ld_header.script -m elf_i386 -o $@ out/$(target)/target/x86_qemu/header.o  out/$(target)/os/arch/x86_32/start16.o

target: out/$(target)/$(target).img
out/$(target)/$(target).img: out/$(target)/header.padding out/$(target)/$(target).bin
	echo dummy > out/$(target)/dummy.bin
	cat out/$(target)/header.padding out/$(target)/$(target).bin > out/$(target)/tmp.bin
	./tools/gen_kernel_header out/$(target)/tmp.bin out/$(target)/dummy.bin out/$(target)/dummy.bin out/$(target)/$(target).img 2> /dev/null
	$(Q)echo "Create $@"
	$(Q)echo "Run qemu: qemu-system-i386 -nographic -m 64M -kernel $@"

