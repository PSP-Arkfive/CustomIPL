#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <bootloadex.h>
#include <bootloadex_ark.h>
#include <pspbtcnf.h>

#include <fat.h>
#include <syscon.h>

// ARK files
#define PATH_SYSTEMCTRL FLASH0_PATH "kd/ark_systemctrl.prx"
#define PATH_PSPCOMPAT FLASH0_PATH "kd/ark_pspcompat.prx"
#define PATH_VITACOMPAT FLASH0_PATH "kd/ark_vitacompat.prx"
#define PATH_VITAPOPS FLASH0_PATH "kd/ark_vitapops.prx"
#define PATH_VSHCTRL FLASH0_PATH "kd/ark_vshctrl.prx"
#define PATH_STARGATE FLASH0_PATH "kd/ark_stargate.prx"
#define PATH_INFERNO FLASH0_PATH "kd/ark_inferno.prx"
#define PATH_POPCORN FLASH0_PATH "kd/ark_popcorn.prx"

#define REG32(addr) *((volatile uint32_t *)(addr))

ARKConfig _arkconf = {
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
};

int UnpackBootConfigDummy(char **p_buffer, int length){
    int result = length;
    int newsize;
    char *buffer;

    result = (*UnpackBootConfig)(*p_buffer, length);
    buffer = (void*)BOOTCONFIG_TEMP_BUFFER;
    memcpy(buffer, *p_buffer, length);
    *p_buffer = buffer;

    // Insert SystemControl
    newsize = AddPRX(buffer, "/kd/init.prx", PATH_SYSTEMCTRL+sizeof(FLASH0_PATH)-2, 0x000000EF);
    if (newsize > 0) result = newsize;
    
    // Insert compat layer
    newsize = AddPRX(buffer, "/kd/init.prx", PATH_PSPCOMPAT+sizeof(FLASH0_PATH)-2, 0x000000EF);
    if (newsize > 0) result = newsize;
    
    // Insert Stargate No-DRM Engine
    newsize = AddPRX(buffer, "/kd/me_wrapper.prx", PATH_STARGATE+sizeof(FLASH0_PATH)-2, GAME_RUNLEVEL | UMDEMU_RUNLEVEL);
    if (newsize > 0) result = newsize;
    
    // Insert VSHControl
    if (SearchPrx(buffer, "/vsh/module/vshmain.prx") >= 0) {
        extern int patch_bootconf_vsh(char *buffer, int length);
        newsize = patch_bootconf_vsh(buffer, result);
        if (newsize > 0) result = newsize;
    }

    // Insert Popcorn
    extern int patch_bootconf_pops(char *buffer, int length);
    newsize = patch_bootconf_pops(buffer, result);
    if (newsize > 0) result = newsize;

    // initialize ARK reboot config
    //initArkRebootConfig(ble_config);

    // Configure boot mode
    switch(reboot_conf->iso_mode) {
        default:
            break;
        case MODE_VSHUMD:
            extern int patch_bootconf_vshumd(char *buffer, int length);
            newsize = patch_bootconf_vshumd(buffer, result);
            if (newsize > 0) result = newsize;
            break;
        case MODE_UPDATERUMD:
            extern int patch_bootconf_updaterumd(char *buffer, int length);
            newsize = patch_bootconf_updaterumd(buffer, result);
            if (newsize > 0) result = newsize;
            break;
        case MODE_MARCH33:
        case MODE_INFERNO:
            reboot_conf->iso_mode = MODE_INFERNO;
            extern int patch_bootconf_inferno(char *buffer, int length);
            newsize = patch_bootconf_inferno(buffer, result);
            if (newsize > 0) result = newsize;
            break;
    }

    //reboot variable set
    if(reboot_conf->rtm_mod.before && reboot_conf->rtm_mod.buffer && reboot_conf->rtm_mod.size)
    {
        //add reboot prx entry
        newsize = AddPRX(buffer, reboot_conf->rtm_mod.before, REBOOT_MODULE, reboot_conf->rtm_mod.flags);
        if(newsize > 0){
            result = newsize;
        }
    }

    if(!ark_config->recovery && is_fatms371())
    {
        newsize = patch_bootconf_fatms371(buffer, length);
        if (newsize > 0) result = newsize;
    }

    if (ble_config->boot_storage == MS_BOOT){
        newsize = patch_bootconf_timemachine(buffer, length);
        if (newsize > 0) result = newsize;
    }

    return result;
}

// Entry Point
int cfwBoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    #ifdef DEBUG
    colorDebug(0xff00);
    #endif

    u32 ctrl = _lw(BOOT_KEY_BUFFER);

    if ((ctrl & SYSCON_CTRL_HOME) == 0) {
        return sceBoot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }

    if ((ctrl & SYSCON_CTRL_RTRIGGER) == 0) {
        _arkconf.recovery = 1;
    }
    
    memcpy(ark_config, &_arkconf, sizeof(ARKConfig));

    // Configure
    configureBoot(&bleconf);

    // scan functions
    findBootFunctions();
    
    // patch sceboot
    patchBootPSP(&UnpackBootConfigArkPSP);
    //patchBootPSP(&UnpackBootConfigDummy);
    
    // Forward Call
    return sceBoot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}
