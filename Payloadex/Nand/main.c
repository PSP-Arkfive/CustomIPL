#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <bootloadex.h>
#include <bootloadex_ark.h>
#include <pspbtcnf.h>

#include <syscon.h>
#include <comms.h>


ARKConfig arkconf = {
    .magic = ARK_CONFIG_MAGIC,
    .arkpath = "ms0:/PSP/SAVEDATA/ARK_01234/", // default path for ARK files
    .exploit_id = CIPL_EXPLOIT_ID,
    .launcher = {0},
    .exec_mode = PSP_ORIG, // run ARK in PSP mode
    .recovery = 0,
};

BootLoadExConfig bleconf = {
    .boot_type = TYPE_PAYLOADEX,
    .boot_storage = FLASH_BOOT,
    .extra_io.psp_io = {
        .use_fatms371 = 1,
    },
    .UnpackBootConfig = &UnpackBootConfigArkPSP,
};


// Entry Point
int cfwBoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{

    *(u32 *) 0x8FB0000 = -1;
    syscon_issue_command_read(0x07, (u8 *) 0x8FB0000);

    u32 ctrl = _lw(BOOT_KEY_BUFFER);

    if ((ctrl & SYSCON_CTRL_HOME) == 0) {
        return sceBoot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }

    if ((ctrl & SYSCON_CTRL_RTRIGGER) == 0) {
        arkconf.recovery = 1;
    }
    
    memcpy(ark_config, &arkconf, sizeof(ARKConfig));

    // Configure
    configureBoot(&bleconf);

    // scan functions
    findBootFunctions();
    
    // patch sceboot
    patchBootPSP();
    
    // Forward Call
    return sceBoot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}
