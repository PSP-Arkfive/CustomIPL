PSPDEV = $(shell psp-config --pspdev-path)
PSPSDK = $(shell psp-config --pspsdk-path)
ARKSDK = $(PSPDEV)/share/ark-dev-sdk

TARGET = ipl_stage2_payload
EXTRA_TARGETS = ipl_stage2_payload.h
OBJS = crt0.o main.o

INCDIR = $(PSPSDK)/include $(PSPSDK)/include/iplsdk
CFLAGS = -Os -G0 -Wall -std=c99 -fno-pic $(BFLAGS) -fdata-sections -ffunction-sections -Wl,--gc-sections
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti 
ASFLAGS = $(CFLAGS)

LIBS = -liplsdk
LIBDIR = $(PSPSDK)/lib
LDFLAGS = -nostartfiles  -T linkfile.l -fdata-sections -ffunction-sections -Wl,--gc-sections

ipl_stage2_payload.h: $(TARGET).bin
	$(Q)bin2c $(TARGET).bin ipl_stage2_payload.h payload

PSPSDK=$(shell psp-config --pspsdk-path)
include $(ARKSDK)/build-tools/build_ipl.mak