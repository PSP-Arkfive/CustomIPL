PSPSDK=$(shell psp-config --pspsdk-path)
PSPDEV = $(shell psp-config --pspdev-path)
ARKSDK = $(PSPDEV)/share/ark-dev-sdk

TARGET = patch.bin

INCDIR = $(PSPSDK)/include
LIBS = -lansic -L $(PSPSDK)/lib

all: $(TARGET)

CFLAGS = -std=c99 -Wall -Os -G0 -fno-pic $(addprefix -I, $(INCDIR))

CC = psp-gcc
LD = psp-ld
STRIP = psp-strip
OBJCOPY = psp-objcopy
LINKFILE = linkfile.l

C_OBJS = \
	main.o \
	cache.o

main.elf: $(C_OBJS) 

$(TARGET): main.elf
	$(Q)$(STRIP) -s $<
	$(Q)$(OBJCOPY) -O binary $< $(TARGET)
	@echo GET $@

clean:
	$(Q)rm -rf *~ *.s *.o *.elf nand_payloadex.bin nand_payloadex.h $(TARGET) $(EXTRA_CLEAN)


include $(ARKSDK)/build-tools/beauty_bin.mak
