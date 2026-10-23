TARGET = cipl_flasher
OBJS = main.o flasher.o

UNAME := $(shell uname)

CFLAGS = -O2 -Os -G0 -Wall -std=c99
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LDFLAGS = -nostartfiles

BUILD_PRX = 1
ENCRYPT = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = cIPL Flasher
PSP_EBOOT_ICON = res/icon0.png
PSP_EBOOT_PIC1 = res/pic1.png

LIBDIR = 
LIBS = -lpspminicrt -lya2d -ltinyfont -lpng -lz -lpspmath -lpspvram -lpspgum -lpspgu -lpspsdk -lpsppower -lpspkubridge -lpspsystemctrl_user -larkctrl -lpspiplupdate -lpspkbootiupdate -lkpspident

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
