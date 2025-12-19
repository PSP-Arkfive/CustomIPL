PSPDEV = $(shell psp-config --pspdev-path)
PSPSDK = $(shell psp-config --pspsdk-path)
ARKSDK = $(PSPDEV)/share/ark-dev-sdk
PYTHON = $(shell which python3)
MAINBINEXDIR =  ../../ClassicIPL/mainbinex

TARGET = patch.bin

all: $(TARGET)

CFLAGS = -std=c99 -Wall -Os -G0 -fno-pic $(addprefix -I, $(INCDIR)) -DMS_IPL
INCDIR = $(PSPSDK)/include $(PSPSDK)/include/iplsdk $(BOOTLOADEX)
LIBS = -lansic -L $(PSPSDK)/lib

CC = psp-gcc
LD = psp-ld
STRIP = psp-strip
OBJCOPY = psp-objcopy
LINKFILE = linkfile.l

MAINBINEX_OBJS = \
	main.o \
	cache.o

$(MAINBINEX_OBJS) : %.o : $(MAINBINEXDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

main.elf: $(MAINBINEX_OBJS) 

$(TARGET): main.elf
	$(Q)$(STRIP) -s $<
	$(Q)$(OBJCOPY) -O binary $< $(TARGET)
	$(Q)$(PYTHON) $(ARKSDK)/build-tools/ipltools/make_tm_ipl.py --input_payload $(TARGET) --payload_addr 0x040D6000 --ipl_addr 0x040EC000 --output payload.bin 
	$(Q)bin2c payload.bin payload.h ms_ipl_payload
	@echo GET $@

clean:
	$(Q)rm -rf *~ *.s *.o *.elf patch.bin payload.bin payload.h $(TARGET) $(EXTRA_CLEAN)


include $(ARKSDK)/build-tools/beauty_bin.mak
