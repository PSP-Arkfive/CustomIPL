PSPSDK=$(shell psp-config --pspsdk-path)
ARKSDK = $(PSPDEV)/share/ark-dev-sdk

TARGET = msipl
OBJS = crt0.o main.o minilzo.o

INCDIR = $(PSPSDK)/include $(PSPSDK)/include/iplsdk/
CFLAGS = -Os -G0 -Wall -fno-pic -std=c99 $(BFLAGS) -fdata-sections -ffunction-sections -Wl,--gc-sections
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti 
ASFLAGS = $(CFLAGS)

LIBS = 
LIBDIR = $(PSPSDK)/lib
LDFLAGS = -nostartfiles  -T linkfile.l -fdata-sections -ffunction-sections -Wl,--gc-sections

include $(ARKSDK)/build-tools/build_ipl.mak

clean:
	$(Q)rm -f msipl_compressed.h *.bin *.dec *.o | true