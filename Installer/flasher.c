#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <psppower.h>
#include <pspsyscon.h>

#include <systemctrl_ark.h>
#include <cfwmacros.h>
#include <systemctrl.h>
#include <kubridge.h>
#include <pspipl_update.h>
#include <kbooti_update.h>

#include "main.h"

typedef struct{
    u8 filesize[4];
    char namelen;
    char name[1];
} PkgFile;

#define KBOOTI_UPDATE_PRX "kbooti_update.prx"

//#define ORIG_IPL_SIZE 126976
#define ORIG_IPL_SIZE 127312
#define ORIG_IPL_MAX_SIZE 131072
#define CLASSIC_CIPL_SIZE 147456
#define NEW_CIPL_SIZE 184320

ARKConfig ark_config;
int model;
static u8 big_buf[256*1024] __attribute__((aligned(64)));

void* pkg_data = NULL;
size_t pkg_size = 0;

int devkit, baryon_ver;


int findPkgFile(void** buffer, size_t* size, const char* path){
    u32 nfiles = *(u32*)(pkg_data);
    PkgFile* cur = (PkgFile*)((size_t)(pkg_data)+4);

    for (int i=0; i<nfiles; i++){
        size_t filesize = (cur->filesize[0]) + (cur->filesize[1]<<8) + (cur->filesize[2]<<16) + (cur->filesize[3]<<24);
        if (strncmp(path, cur->name, cur->namelen) == 0){
            *buffer = (void*)((size_t)(&(cur->name[0])) + cur->namelen);
            *size = filesize;
            return 0;
        }
        cur = (PkgFile*)((size_t)(cur)+filesize+cur->namelen+5);
    }
    return -1;
}

int ReadPkgFile()
{
    SceUID fd = sceIoOpen("CIPL.ARK", PSP_O_RDONLY, 0);
    if (fd < 0)
        return fd;

    pkg_size = sceIoLseek(fd, 0, PSP_SEEK_END);
    sceIoLseek(fd, 0, PSP_SEEK_SET);
    
    if (pkg_size <= 0)
    {
       	sceIoClose(fd);
      	return -1;
    }

    pkg_data = malloc(pkg_size);
    if (pkg_data == NULL) return -2;

    int read = sceIoRead(fd, pkg_data, pkg_size);
    
    sceIoClose(fd);
    
    if (read < pkg_size) return -3;

    return 0;
}

void open_flash(){
    while(sceIoUnassign("flash0:") < 0) {
        sceKernelDelayThread(500000);
    }
    while (sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", 0, NULL, 0)<0){
        sceKernelDelayThread(500000);
    }
    while(sceIoUnassign("flash1:") < 0) {
        sceKernelDelayThread(500000);
    }
    while (sceIoAssign("flash1:", "lflash0:0,1", "flashfat1:", 0, NULL, 0)<0){
        sceKernelDelayThread(500000);
    }
}

void loadIplUpdateModule(){
    SceUID mod;
    //load module
    mod = sceKernelLoadModule("ipl_update.prx", 0, NULL);

    if (mod == 0x80020139) return; // SCE_ERROR_KERNEL_EXCLUSIVE_LOAD

    if (mod < 0) mod = sceKernelLoadModule("ms0:/PSP/LIBS/ipl_update.prx", 0, NULL); // retry from LIBS folder
    if (mod < 0) {
        ErrorExit(5000,"Could not load ipl_update.prx!");
    }

    mod = sceKernelStartModule(mod, 0, NULL, NULL, NULL);

    if (mod == 0x80020133) return; // SCE_ERROR_KERNEL_MODULE_ALREADY_STARTED

    if (mod < 0) {
        ErrorExit(5000,"Could not start module!");
    }
}

int is_ta88v3(void)
{
    u32 model, tachyon;

    tachyon = sceSysregGetTachyonVersion();
    model = kuKernelGetModel();

    if(model == 1 && tachyon == 0x00600000) {
        return 1;
    }

    return 0;
}

void classicipl_menu(){
    static char* menuopts[] = {
        "X - Install Classic cIPL",
        "O - Revert to original IPL",
        "/\\ - Cancel and Exit",
    };
    cipl_type = "Classic cIPL";
    options = menuopts;
    nopts = NELEMS(menuopts);

    int size;

    void *ipl_block, *ipl_block_large;
    size_t size_ipl_block, size_ipl_block_large;
    char* ipl_file = (model == 0)? "ipl_classic_01g.bin" : "ipl_classic_02g.bin";

    if (findPkgFile(&ipl_block, &size_ipl_block, ipl_file) < 0){
        ErrorExit(5000,"Failed to find file in pkg: %s", ipl_file);
    }

    size_ipl_block_large = 0x4000+size_ipl_block;
    ipl_block_large = memalign(16, size_ipl_block_large);
    memcpy(ipl_block_large , ipl_block, size_ipl_block);

    loadIplUpdateModule();

    size = pspIplUpdateGetIpl(big_buf);

    if(size < 0) {
        ErrorExit(5000,"Failed to get IPL!");
    }

    int ipl_type = 0;
    
    if (size > ORIG_IPL_MAX_SIZE){
        if( size == CLASSIC_CIPL_SIZE ) {
        	setInfoMsg(INFO_MSG, "Custom IPL is installed");
        	size -= 0x4000;
        	memmove(ipl_block_large + 0x4000 , big_buf + 0x4000 , size);
        	ipl_type = 1;
        } else {
        	return newipl_menu();
        }
    } else {
        setInfoMsg(INFO_MSG, "Original IPL ");
        memmove(ipl_block_large + 0x4000, big_buf, size);
    }
    
    SceCtrlData pad;
    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_CROSS) {
        	setInfoMsg(INFO_MSG, "Flashing cIPL...");

        	if(pspIplUpdateClearIpl() < 0)
        		ErrorExit(5000,"Failed to clear IPL!");

        	if (pspIplUpdateSetIpl(ipl_block_large, size+0x4000, 0 ) < 0)
        		ErrorExit(5000,"Failed to write cIPL!");

        	setInfoMsg(INFO_MSG, "Done.");
        	break; 
        } else if ( (pad.Buttons & PSP_CTRL_CIRCLE) && ipl_type ) {		
        	setInfoMsg(INFO_MSG, "Flashing IPL...");

        	if(pspIplUpdateClearIpl() < 0) {
        		ErrorExit(5000,"Failed to clear IPL!");
        	}

        	if (pspIplUpdateSetIpl( ipl_block_large + 0x4000 , size, 0 ) < 0) {
        		ErrorExit(5000,"Failed to write IPL!");
        	}

        	setInfoMsg(INFO_MSG, "Done.");
        	break; 
        } else if (pad.Buttons & PSP_CTRL_RTRIGGER) {
        	NormalExit(2000,"Cancelled by user.");
        }

        sceKernelDelayThread(10000);
    }
    free(ipl_block_large);
    RebootExit(5000,"Install complete. Restarting in 5 seconds...");
}

void devtoolipl_menu(){
    static char* menuopts[] = {
        "X - Install cKbooti",
        "O - Revert to original Kbooti",
        "/\\ - Cancel and Exit",
    };
    cipl_type = "DevTool cKbooti";
    options = menuopts;
    nopts = NELEMS(menuopts);

    int size;
    SceUID mod;

    //load module
    mod = sceKernelLoadModule(KBOOTI_UPDATE_PRX, 0, NULL);

    if (mod < 0) mod = sceKernelLoadModule("ms0:/PSP/LIBS/" KBOOTI_UPDATE_PRX, 0, NULL);
    if (mod < 0) {
        ErrorExit(5000, "Could not load " KBOOTI_UPDATE_PRX "!");
    }

    mod = sceKernelStartModule(mod, 0, NULL, NULL, NULL);

    if (mod < 0) {
        ErrorExit(5000, "Could not start module!");
    }

    size = pspKbootiUpdateGetKbootiSize();

    if(size < 0) {
        ErrorExit(5000, "Failed to get kbooti!");
    }

    void *ipl_block;
    size_t size_ipl_block;

    if (findPkgFile(&ipl_block, &size_ipl_block, "kbooti_ipl_01g.bin") < 0){
        ErrorExit(5000, "Failed to find file in pkg: kbooti_ipl_01g.bin");
    };

    memcpy(big_buf, ipl_block, size_ipl_block);

    int ipl_type = 0;
    const int IPL_SIZE = 0x21000;
    const int CIPL_SIZE = 0x23D10;

    if(size == CIPL_SIZE) {
        setInfoMsg(INFO_MSG, "Custom kbooti is installed");
        ipl_type = 1;
    } else if( size == IPL_SIZE ) {
        setInfoMsg(INFO_MSG, "Original kbooti ");
    } else {
        ErrorExit(5000,"Unknown kbooti! (%08X)", size);
    }
    
    SceCtrlData pad;
    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_CROSS) {
        	setInfoMsg(INFO_MSG, "Flashing cKBOOTI...");
        	if (pspKbootiUpdateKbooti(big_buf, size_ipl_block) < 0)
        		ErrorExit(5000, "Failed to write kbooti!");

        	setInfoMsg(INFO_MSG, "Done.");
        	break; 
        } else if ( (pad.Buttons & PSP_CTRL_CIRCLE) && ipl_type ) {
        	setInfoMsg(INFO_MSG, "Flashing KBOOTI...");
        	if (pspKbootiUpdateRestoreKbooti() < 0) {
        		ErrorExit(5000, "Failed to write kbooti!");
        	}
        	setInfoMsg(INFO_MSG, "Done.");
        	break; 
        } else if (pad.Buttons & PSP_CTRL_RTRIGGER) {
        	NormalExit(2000, "Cancelled by user.");
        }

        sceKernelDelayThread(10000);
    }
    RebootExit(5000, "Install complete. Restarting in 5 seconds...");
}

void newipl_menu(){
    static char* menuopts[] = {
        "X - Install cIPL",
        "O - Revert to original IPL",
        "/\\ - Cancel and Exit",
        "LT - switch to Classic cIPL",
    };
    cipl_type = "New cIPL";
    options = menuopts;
    nopts = NELEMS(menuopts)-1; // hide LT option by default

    size_t size = NEW_CIPL_SIZE;
    u16 ipl_key = 0;

    static char* ipl_table[] = {
        (char*)"cipl_01g.bin",
        (char*)"cipl_02g.bin",
        (char*)"cipl_03g.bin",
        (char*)"cipl_04g.bin",
        (char*)"cipl_05g.bin",
        (char*)NULL, // 6g
        (char*)"cipl_07g.bin",
        (char*)NULL, // 8g
        (char*)"cipl_09g.bin",
        (char*)NULL, // 10g
        (char*)"cipl_11g.bin",
    };

    static char* orig_ipl_table[] = {
        (char*)"ipl_01g.bin",
        (char*)"ipl_02g.bin",
        (char*)"ipl_03g.bin",
        (char*)"ipl_04g.bin",
        (char*)"ipl_05g.bin",
        (char*)NULL, // 6g
        (char*)"ipl_07g.bin",
        (char*)NULL, // 8g
        (char*)"ipl_09g.bin",
        (char*)NULL, // 10g
        (char*)"ipl_11g.bin",
    };

    int supported_models = sizeof(ipl_table)/sizeof(ipl_table[0]);

    if (devkit == 0x02070110)
	    model = 0;

    if (model >= supported_models || ipl_table[model] == NULL) {
        ErrorExit(5000, "This installer does not support this model %02d", model);
    }

    if (model > 1){
        ipl_key = (model==4)?2:1;
    }

    loadIplUpdateModule();

    if (model < 2 && !is_ta88v3()) {
	    // allow classic install;
	    nopts++;
    }

    SceCtrlData pad;
    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_CROSS) {
        	setInfoMsg(INFO_MSG, "Flashing Custom cIPL...");

        	size = NEW_CIPL_SIZE;
            void* ipl_data = NULL;

            if (findPkgFile(&ipl_data, &size, ipl_table[model]) < 0){
                ErrorExit(5000,"Failed to find file in pkg: %s", ipl_table[model]);
            }

            memcpy(big_buf, ipl_data, size);

        	if(pspIplUpdateClearIpl() < 0)
        		ErrorExit(5000,"Failed to clear IPL!");

        	if (pspIplUpdateSetIpl(big_buf, size, 0 ) < 0)
        		ErrorExit(5000,"Failed to write cIPL!");

        	setInfoMsg(INFO_MSG, "Done.");
        	break; 
        }
        else if (pad.Buttons & PSP_CTRL_CIRCLE) {
        	setInfoMsg(INFO_MSG, "Flashing Original IPL...");

        	size = ORIG_IPL_SIZE;
            void* ipl_data = NULL;

            if (findPkgFile(&ipl_data, &size, orig_ipl_table[model]) < 0){
                ErrorExit(5000,"Failed to find file in pkg: %s", orig_ipl_table[model]);
            }
            memcpy(big_buf, ipl_data, size);

        	if(pspIplUpdateClearIpl() < 0) {
        		ErrorExit(5000,"Failed to clear IPL!");
        	}
        	if (pspIplUpdateSetIpl( big_buf, size,  ipl_key) < 0) {
        		ErrorExit(5000,"Failed to write IPL!");
        	}

        	setInfoMsg(INFO_MSG, "Done.");
        	break; 
        } else if (pad.Buttons & PSP_CTRL_TRIANGLE) {
        	NormalExit(2000,"Cancelled by user.");
            break;
        }

        sceKernelDelayThread(10000);
    }

    RebootExit(5000, "Install complete. Restarting in 5 seconds...");
}

void cipl_flasher() 
{
    SceUID kpspident;

    devkit = sceKernelDevkitVersion();

    SceIoStat dc_stat;
    int has_dc = sceIoGetstat("ms0:/TM/DCARK/msipl.raw", &dc_stat)>=0;

    int res = 0;
    if ((res=ReadPkgFile())<0){
        ErrorExit(5000, "Failed to read CIPL.ARK package: %d (%p)", res, res);
    }

    // check if running 6.60 or 6.61
    if(devkit != 0x06060010 && devkit != 0x06060110) {
        if(!has_dc) {
            ErrorExit(5000,"DCARK MISSING, INSTALL IT FIRST!");
        }
        else {
            setInfoMsg(WARNING_MSG, "After install your PSP will be 'bricked', turn on holding LT (Left Trigger) to boot DCARK");
            sceKernelDelayThread(4*1000*1000);
        }
    }

    // check if running infinity
    SceModule infinity_mod;
    if (kuKernelFindModuleByName("InfinityControl", &infinity_mod) == 0 && !has_dc){
        ErrorExit(5000, "ERROR: installing cIPL over Infinity is risky, make sure you install DC-ARK first before doing this!");
    }

    kpspident = pspSdkLoadStartModule("kpspident.prx", PSP_MEMORY_PARTITION_KERNEL);
    if (kpspident < 0) kpspident = pspSdkLoadStartModule("ms0:/PSP/LIBS/kpspident.prx", PSP_MEMORY_PARTITION_KERNEL); // retry from LIBS folder
    if (kpspident < 0) {
        ErrorExit(5000, "kpspident.prx loaded failed");
    }

    model = kuKernelGetModel();
    if(model<0) {
	    u32 tachyon = sceSysregGetTachyonVersion();
	    if(tachyon > 0x00400000 && baryon_ver <= 0x00243000)
              model = 1; // Fix for lower firmwares that do not support kubridge, and are not Phats
	    else
	    	model = 0; // Fix for lower firmwares that do not support kubridge
    }

    if (sceSysconGetBaryonVersion(&baryon_ver) < 0) {
        ErrorExit(5000, "Could not determine baryon version!");
    }

    // check if running ARK
    memset(&ark_config, 0, sizeof(ARKConfig));
    sctrlArkGetConfig(&ark_config);
    if (ark_config.magic != ARK_CONFIG_MAGIC){
        ErrorExit(5000, "Only available for ARK! Use older cIPL installer for PRO/ME compatibility.");
    }

    switch (sctrlHENIsToolKit()) {
	    case 0: newipl_menu();     break;
	    case 1: classicipl_menu(); break;
	    case 2: devtoolipl_menu(); break;
    }
}