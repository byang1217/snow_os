#include "snow_os.h"

extern unsigned int _sdata, _edata, __data_in_rom__, __zdata_start__, __zdata_end__;
extern unsigned int _estack, _sstack;

int main(void)
{
}

void c_start(void)
{
	SystemInit();
        memcpy(&_sdata, &__data_in_rom__, (unsigned int)&_edata - (unsigned int)&_sdata);
        memset(&__zdata_start__, 0, (unsigned int)&__zdata_end__ - (unsigned int)&__zdata_start__);
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08005000);
	snow_start((void *)main, (void *)((unsigned int)&_sstack), (unsigned int)&_estack - (unsigned int)&_sstack - 512, &_estack);
}
