# makefile 
#
# Copyright (C) <2013>  Bin Yang <byang1217@gmail.com>
#

A := target
PHONY += help
help:
	@echo  "make target			- To build specific target"
	@echo  "make clean			- Remove all generated files"
	@echo  "make target V=0|1		- 0: quiet build, 1: verbose build"
	@echo
	@echo "Target list:"
	for target in $(target_list) ;\
	do	\
		echo "	"$$target;\
	done
	@echo  ''
	@echo  'Copyright (C) Bin Yang <byang1217@gmail.com>'
	@echo  ''

target_list = $(shell ls target/)
PHONY += $(target_list)
$(target_list):
	mkdir -p out/$@
	make target=$@ target_mk=target/$@/target.mk $(A)

ld-script = out/$(target)/ld.script
objs-final = out/$(target)/all.o
-include $(target_mk)
CFLAGS += -g -Itarget/$(target) -Ios/arch/$(arch)/ -Ios 
objs-all += $(patsubst %.c,%.o,$(filter %.c, $(target_src)))
objs-all += $(patsubst %.s,%.o,$(filter %.s, $(target_src)))
objs-all += $(patsubst %.S,%.o,$(filter %.S, $(target_src)))

debug:
	echo $(patsubst %,out/$(target)/%,$(objs-all))

out/$(target)/all.o: $(patsubst %,out/$(target)/%,$(objs-all))
ifeq ($(DEBUG),y)
	$(Q)echo "Create $@ from $^"
endif
	$(LD) $(LDFLAGS) -r -o $@ $^

-include $(patsubst %.o,%.dep,$(filter %.o, $(obj-all)))

export ASM	:= $(CROSS_COMPILE)gcc
export LD	:= $(CROSS_COMPILE)ld
export CC	:= $(CROSS_COMPILE)gcc
export CPP	:= $(CROSS_COMPILE)gcc
export AR	:= $(CROSS_COMPILE)ar
export OBJDUMP	:= $(CROSS_COMPILE)objdump
export OBJCOPY	:= $(CROSS_COMPILE)objcopy
export GDB	:= $(CROSS_COMPILE)gdb

export DEBUG	:=n
V		:=0
ifeq ($(V), 1)
export JOBS	:= 1
export Q	:=
export S	:=
export MAKEFLAGS := -e
else
export JOBS	:= $(shell grep --count processor /proc/cpuinfo)
export Q	:=@
export S	:=-s
export MAKEFLAGS := -e -s --no-print-directory
endif

gdeps	+=Makefile $(target_mk)

.SUFFIXES :
.SUFFIXES : .o .s .S .c
out/$(target)/%.o: %.c $(gdeps)
ifeq ($(DEBUG),y)
	$(Q)echo "$(CC) $(CFLAGS) $(MY_CFLAGS) -c -o $@ $<"
else
	$(Q)echo "CC $<"
endif
	mkdir -p `dirname $@`
	$(Q)$(CC) $(CFLAGS) $(MY_CFLAGS) -MM $< > $(patsubst %.o,%.dep,$@)
	$(Q)sed -i 's+^.*.o:+$@:+' $(patsubst %.o,%.dep,$@)
ifeq ($(DEBUG),y)
	$(Q)$(CC) -E $(CFLAGS) $(MY_CFLAGS) -c -o $(patsubst %.o,%.e,$@) $<
	$(Q)$(CC) -S $(CFLAGS) $(MY_CFLAGS) -c -o $(patsubst %.o,%.s,$@) $<
endif
	$(Q)$(CC) $(CFLAGS) $(MY_CFLAGS) -c -o $@ $<

out/$(target)/%.o: %.s $(gdeps)
ifeq ($(DEBUG),y)
	$(Q)echo "$(CC) $(CFLAGS) $(MY_CFLAGS) -c -o $@ $<"
else
	$(Q)echo "CC $<"
endif
	mkdir -p `dirname $@`
	$(Q)$(CC) $(CFLAGS) $(MY_CFLAGS) -MM $< > $(patsubst %.o,%.dep,$@)
	$(Q)sed -i 's+^.*.o:+$@+' $(patsubst %.o,%.dep,$@)
ifeq ($(DEBUG),y)
	$(Q)$(CC) -E $(CFLAGS) $(MY_CFLAGS) -c -o $(patsubst %.o,%.e,$@) $<
endif
	$(Q)$(CC) $(CFLAGS) $(MY_CFLAGS) -c -o $@ $<

out/$(target)/%.o: %.S $(gdeps)
ifeq ($(DEBUG),y)
	$(Q)echo "$(CC) $(CFLAGS) $(MY_CFLAGS) -c -o $@ $<"
else
	$(Q)echo "CC $<"
endif
	mkdir -p `dirname $@`
	$(Q)$(CC) $(CFLAGS) $(MY_CFLAGS) -MM $< > $(patsubst %.o,%.dep,$@)
	$(Q)sed -i 's+^.*.o:+$@+' $(patsubst %.o,%.dep,$@)
ifeq ($(DEBUG),y)
	$(Q)$(CC) -E $(CFLAGS) $(MY_CFLAGS) -c -o $(patsubst %.o,%.e,$@) $<
endif
	$(Q)$(CC) $(CFLAGS) $(MY_CFLAGS) -c -o $@ $<

$(ld-script): os/ld.script.S $(gdeps)
	$(Q)echo "Gen ld.script"
	$(Q)$(CC) -E $(CFLAGS)-P -o $@ os/ld.script.S

PHONY += clean
clean:
	rm -rf out

FORCE:

.PHONY: $(PHONY)

