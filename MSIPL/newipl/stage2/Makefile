PSPDEV = $(shell psp-config --pspdev-path)
PSPSDK = $(shell psp-config --pspsdk-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk

TARGET = msipl
OBJS = crt0.o main.o

INCDIR = $(PSPSDK)/include $(PSPSDK)/include/iplsdk/
CFLAGS = -Os -G0 -Wall -fno-pic -std=c99 $(BFLAGS) -fdata-sections -ffunction-sections -Wl,--gc-sections
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti 
ASFLAGS = $(CFLAGS)

LIBS = -liplsdk
LIBDIR = $(PSPSDK)/lib
LDFLAGS = -nostartfiles  -T linkfile.l -fdata-sections -ffunction-sections -Wl,--gc-sections


PSPSDK=$(shell psp-config --pspsdk-path)
include $(CFWSDK)/build-tools/build_ipl.mak

clean:
	$(Q)rm -f *.h *.bin *.dec *.o *.lzo | true