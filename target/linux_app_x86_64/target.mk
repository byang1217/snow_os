CFLAGS += -O0

target_src += target/linux_app_x86_64/main.c
target_src += os/arch/linux_app/x86_64_sig.c 
target_src += os/snow.c

target: out/$(target)/$(target).elf
out/$(target)/$(target).elf: $(objs-final)
	$(Q)echo "Create $@"
ifeq ($(DEBUG),y)
	$(Q)echo $(CC) -o $@ $^ -Wl,-Map=$(target).map $(CFLAGS) $(LDFLAGS)
endif
	$(CC) -o $@ $^ -Wl,-Map=$(patsubst %.elf,%.map,$@) $(CFLAGS) $(LDFLAGS)
	$(OBJDUMP) -D $@ > $(patsubst %.elf,%.dump,$@)

	
