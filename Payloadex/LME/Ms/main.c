#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <bootloadex.h>
#include <bootloadex_lme.h>

#include <fat.h>
#include <syscon.h>
#include <comms.h>


BootLoadExConfig bleconf = {
    .boot_type = TYPE_PAYLOADEX,
    .boot_storage = MS_BOOT,
    .extra_io.psp_io = {
        .use_fatms371 = 0,
        #if PSP_FW == 660
        .tm_path = "/TM/LME660",
        #endif
        .FatMount = &MsFatMount,
        .FatOpen = &MsFatOpen,
        .FatRead = &MsFatRead,
        .FatClose = &MsFatClose,
    },
    .UnpackBootConfig = &UnpackBootConfigLMEPSP,
};


// Entry Point
int cfwBoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    // Configure
    configureBoot(&bleconf);

    // scan functions
    findBootFunctions();
    
    // patch sceboot
    patchBootPSP();
    
    // Forward Call
    return sceBoot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}
