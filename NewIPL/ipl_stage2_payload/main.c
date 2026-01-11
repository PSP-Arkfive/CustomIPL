#include <pspsdk.h>
#include <syscon.h>
#include <comms.h>
#include "gpio.h"

#ifdef MSIPL
#include "../../Payloadex/Ms/payload.h"
#else
#include "../../Payloadex/Nand/payload.h"
#endif

void Dcache();
void Icache();
int lzo1x_decompress(void*, int, void*, int*, void*);

int main()
{
    int len = 0x20000;
    lzo1x_decompress(payloadex, size_payloadex, (void*)0x8FC0000, &len, (void*)0);

    Dcache();
    Icache();
}
