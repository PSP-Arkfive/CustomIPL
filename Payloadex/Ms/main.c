#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <bootloadex.h>
#include <bootloadex_ark.h>

#include <fat.h>
#include <syscon.h>

#define REG32(addr) *((volatile uint32_t *)(addr))

ARKConfig arkconf = {
    .magic = ARK_CONFIG_MAGIC,
    .arkpath = ARK_DC_PATH "/ARK_01234/", // default path for ARK files
    .exploit_id = DC_EXPLOIT_ID,
    .launcher = {0},
    .exec_mode = PSP_ORIG, // run ARK in PSP mode
    .recovery = 0,
};

int UnpackBootConfigDummy(char **p_buffer, int length){
    int result = length;
    int newsize;
    char *buffer;

    result = (*UnpackBootConfig)(*p_buffer, length);
    buffer = (void*)BOOTCONFIG_TEMP_BUFFER;
    memcpy(buffer, *p_buffer, length);
    *p_buffer = buffer;



    return result;
}


BootLoadExConfig bleconf = {
    .boot_type = TYPE_PAYLOADEX,
    .boot_storage = MS_BOOT,
    .extra_io = {
        .psp_io = {
            .use_fatms371 = 0,
            .tm_path = "/TM/DCARK",
            .FatMount = &MsFatMount,
            .FatOpen = &MsFatOpen,
            .FatRead = &MsFatRead,
            .FatClose = &MsFatClose,
            .UnpackBootConfig = &UnpackBootConfigArkPSP, //&UnpackBootConfigDummy
        }
    }
};


// Entry Point
int cfwBoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{

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
