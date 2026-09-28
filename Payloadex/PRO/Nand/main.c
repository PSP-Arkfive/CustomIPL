#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <bootloadex.h>
#include <bootloadex_pro.h>
#include <pspbtcnf.h>

#include <syscon.h>
#include <comms.h>


BootLoadExConfig bleconf = {
    .boot_type = TYPE_PAYLOADEX,
    .boot_storage = FLASH_BOOT,
    .extra_io.psp_io = {
        .use_fatms371 = 1,
    },
    .UnpackBootConfig = &UnpackBootConfigPROPSP,
};


// Entry Point
int cfwBoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{

    *(u32 *) BOOT_KEY_BUFFER = -1;
    syscon_issue_command_read(0x07, (u8 *) BOOT_KEY_BUFFER);

    u32 ctrl = _lw(BOOT_KEY_BUFFER);

    if ((ctrl & SYSCON_CTRL_HOME) == 0) {
        return sceBoot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }

    if ((ctrl & SYSCON_CTRL_RTRIGGER) == 0) {
        pro_recovery_mode = 1;
    }

    // Configure
    configureBoot(&bleconf);

    // scan functions
    findBootFunctions();
    
    // patch sceboot
    patchBootPSP();
    
    // Forward Call
    return sceBoot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}
