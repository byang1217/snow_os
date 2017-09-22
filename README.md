# snow_os
a very tiny RTOS

# demo
## Run as Linux app on x86_64 linux HOST
```
make linux_app_x86_64
./out/linux_app_x86_64/linux_app_x86_64.elf
press CTRL+C to show RTOS status
```
Know issue: 
Crash during RTOS resources init sometimes.
## Run by x86 Qemu 
```
make x86_qemu
make linux_app_x86_64
qemu-system-i386 -nographic -m 64M -kernel out/x86_qemu/x86_qemu.img
```
Know issue: 
crash for tss invalid setting on new Qemu version

Workarround: 
use old Qemu. E.g. Qemu v2.0.0

# TODO
Try to fix Know issues

STM32 version can work on my dev board with additional code for hw setup.
It needs to clean up the code and enable it with ARM Qemu for users who have
no HW.

