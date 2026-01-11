#include <pspsdk.h>
#include <syscon.h>
#include <comms.h>
#include "gpio.h"
#ifdef DEBUG
#include <uart.h>
#include <printf.h>
#endif

#ifdef MSIPL
#include "../../Payloadex/Ms/payload.h"
#else
#include "../../Payloadex/Nand/payload.h"
#endif

void Dcache();
void Icache();
int lzo1x_decompress(void*, int, void*, int*, void*);

void *memcpy(void *dest, void *src, uint32_t size)
{
    uint32_t *d = (uint32_t *) dest;
    uint32_t *s = (uint32_t *) src;
    
    size /= 4;
    
    while (size--)
        *(d++) = *(s++);

    return dest;
}

int main()
{
#ifdef DEBUG
    uart_init();
    printf("stage2 starting...\n");
#endif

    int len = 0x20000;
    lzo1x_decompress(payloadex, size_payloadex, (void*)0x8FC0000, &len, (void*)0);

    Dcache();
    Icache();
}
