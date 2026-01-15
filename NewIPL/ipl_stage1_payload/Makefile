PSPDEV = $(shell psp-config --pspdev-path)
PSPSDK = $(shell psp-config --pspsdk-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk

TARGET = ipl_stage1_payload
OBJS = crt0.o main.o

INCDIR = ../ipl_stage2_payload $(PSPSDK)/include $(PSPSDK)/include/iplsdk
CFLAGS = -Os -G0 -Wall -std=c99 -fno-pic $(BFLAGS) -fdata-sections -ffunction-sections -Wl,--gc-sections
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti 
ASFLAGS = $(CFLAGS)

LIBS = -liplsdk
LIBDIR = $(PSPSDK)/lib
LDFLAGS = -nostartfiles  -T linkfile.l -fdata-sections -ffunction-sections -Wl,--gc-sections

PSPSDK=$(shell psp-config --pspsdk-path)
include $(CFWSDK)/build-tools/build_ipl.mak
