#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <bootloadex.h>

#include <fat.h>
#include <syscon.h>

#define REG32(addr) *((volatile uint32_t *)(addr))

ARKConfig _arkconf = {
    .magic = ARK_CONFIG_MAGIC,
    .arkpath = ARK_DC_PATH "/ARK_01234/", // default path for ARK files
    .exploit_id = DC_EXPLOIT_ID,
    .launcher = {0},
    .exec_mode = PSP_ORIG, // run ARK in PSP mode
    .recovery = 0,
};

static ExtraIoFuncs ms_extra_io = {
    .FatMount = &MsFatMount,
    .FatOpen = &MsFatOpen,
    .FatRead = &MsFatRead,
    .FatClose = &MsFatClose,
};

// Entry Point
int cfwBoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    #ifdef DEBUG
    colorDebug(0xff00);
    #endif

    is_msipl = 1;
    is_payloadex = 1;
    extra_io = &ms_extra_io;

    memcpy(ark_config, &_arkconf, sizeof(ARKConfig));

    // GPIO enable
    REG32(0xbc10007c) |= 0xc8;
    __asm("sync"::);
    
    syscon_init();
    
    syscon_ctrl_ms_power(1);

    // check config
    checkRebootConfig();

    // scan for reboot functions
    findRebootFunctions();
    
    // patch reboot buffer
    patchRebootBufferPSP();
    
    // Forward Call
    return sceReboot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}
