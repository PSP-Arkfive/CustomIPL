PSPSDK=$(shell psp-config --pspsdk-path)

TARGET = flasher
OBJS = main.o stubkk.o

LIBS = -lpspkubridge -lpspsystemctrl_user -lpspIplUpdate -lpspKbootiUpdate -lkpspident
INCDIR = $(ARKROOT)/common/include $(ARKROOT)/loader/perma/cipl/classic $(ARKROOT)/loader/perma/cipl/new
LIBDIR = $(ARKROOT)/libs

CFLAGS = -O2 -G0 -Wall -fshort-wchar
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = cIPL Flasher
PSP_EBOOT_ICON = ICON0.PNG
PSP_FW_VERSION = 660
BUILD_PRX = 1

all: $(TARGET).prx


include $(PSPSDK)/lib/build.mak
