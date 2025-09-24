TARGET = msipl
OBJS = crt0.o main.o minilzo.o

INCDIR = $(ARKSDK)/include $(ARKSDK)/include/iplsdk/
CFLAGS = -Os -G0 -Wall -fno-pic -std=c99 $(BFLAGS) -fdata-sections -ffunction-sections -Wl,--gc-sections
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti 
ASFLAGS = $(CFLAGS)

LIBS = 
LIBDIR = $(ARKSDK)/libs
LDFLAGS = -nostartfiles  -T linkfile.l -fdata-sections -ffunction-sections -Wl,--gc-sections


PSPSDK=$(shell psp-config --pspsdk-path)
include $(ARKSDK)/build-tools/build_ipl.mak

clean:
	$(Q)rm -f msipl_compressed.h *.bin *.dec *.o | true