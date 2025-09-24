#include "msipl_compressed.h"

extern void DcacheClear();
extern void IcacheClear();
extern int lzo1x_decompress(void*, int, void*, int*, void*);

int entry(){

    void* load_addr = (void*)0x40c0000;
    int len = 0x20000;

    lzo1x_decompress(msipl_compressed, size_msipl_compressed, load_addr, &len, (void*)0);

    DcacheClear();
    IcacheClear();

    return ((int (*)()) load_addr)();
}