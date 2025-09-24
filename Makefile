.PHONY: all payloadex cipl msipl clean

ARKSDK ?= $(CURDIR)/../ark-dev-sdk
BOOTLOADEX ?= $(CURDIR)/../BootLoadEx

all: payloadex cipl msipl

payloadex:
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C $(BOOTLOADEX)/nand_payloadex/
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C $(BOOTLOADEX)/ms_payloadex/

cipl:
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C ClassicIPL/mainbinex
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C ClassicIPL/combine
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=01G -C NewIPL
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=02G -C NewIPL
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=03G -C NewIPL
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=04G -C NewIPL
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=05G -C NewIPL
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=07G -C NewIPL
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=09G -C NewIPL
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=11G -C NewIPL

msipl:
	$(Q)$(MAKE) gcc -C MSIPL/minilzo
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C MSIPL/newipl/stage2
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C MSIPL/mainbinex
	$(Q)MSIPL/minilzo/testmini MSIPL/newipl/stage2/msipl.bin MSIPL/newipl/stage2/msipl.lzo
	$(Q)bin2c MSIPL/newipl/stage2/msipl.lzo MSIPL/newipl/stage1/msipl_compressed.h msipl_compressed
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C MSIPL/newipl/stage1
	$(Q)$(PYTHON) $(ARKSDK)/build-tools/ipltools/make_ipl.py MSIPL/newipl/stage1/msipl.bin MSIPL/newipl/stage1/ipl.bin reset_block 0x4000000
	$(Q)bin2c MSIPL/newipl/stage1/ipl.bin MSIPL/newipl/stage2/new_msipl.h new_msipl
	$(Q)bin2c MSIPL/newipl/stage2/msipl.bin MSIPL/newipl/stage2/msipl_raw.h msipl_raw
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=01G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_01G.bin MSIPL/newipl/msipl_01g.bin
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=02G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_02G.bin MSIPL/newipl/msipl_02g.bin
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=03G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_03G.bin MSIPL/newipl/msipl_03g.bin
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=04G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_04G.bin MSIPL/newipl/msipl_04g.bin
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=05G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_05G.bin MSIPL/newipl/msipl_05g.bin
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=07G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_07G.bin MSIPL/newipl/msipl_07g.bin
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=09G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_09G.bin MSIPL/newipl/msipl_09g.bin
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) PSP_MODEL=11G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_11G.bin MSIPL/newipl/msipl_11g.bin

clean:
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C $(BOOTLOADEX)
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C NewIPL clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C ClassicIPL/mainbinex clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C ClassicIPL/combine clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C MSIPL/minilzo clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C MSIPL/mainbinex clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C MSIPL/newipl/stage1 clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C MSIPL/newipl/stage2 clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX=$(BOOTLOADEX) -C MSIPL/newipl/stage3 clean
	$(Q)rm -f MSIPL/newipl/msipl_*.bin
