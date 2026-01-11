.PHONY: all minilzo cipl msipl clean

PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
ARKSDK = $(PSPDEV)/share/ark-dev-sdk
BUILDTOOLS = $(ARKSDK)/build-tools

all: minilzo cipl msipl

minilzo:
	$(Q)$(MAKE) gcc -C minilzo

cipl:
	$(Q)$(MAKE) -C Payloadex/Nand
	$(Q)$(MAKE) -C ClassicIPL/mainbinex
	$(Q)$(MAKE) -C ClassicIPL/combine
	$(Q)$(MAKE) PSP_MODEL=01G -C NewIPL
	$(Q)$(MAKE) PSP_MODEL=02G -C NewIPL
	$(Q)$(MAKE) PSP_MODEL=03G -C NewIPL
	$(Q)$(MAKE) PSP_MODEL=04G -C NewIPL
	$(Q)$(MAKE) PSP_MODEL=05G -C NewIPL
	$(Q)$(MAKE) PSP_MODEL=07G -C NewIPL
	$(Q)$(MAKE) PSP_MODEL=09G -C NewIPL
	$(Q)$(MAKE) PSP_MODEL=11G -C NewIPL

msipl:
	$(Q)$(MAKE) -C Payloadex/Ms
	$(Q)$(MAKE) -C MSIPL/newipl/stage2
	$(Q)$(MAKE) -C MSIPL/mainbinex
	$(Q)minilzo/testmini MSIPL/newipl/stage2/msipl.bin MSIPL/newipl/stage2/msipl.lzo
	$(Q)bin2c MSIPL/newipl/stage2/msipl.lzo MSIPL/newipl/stage1/msipl_compressed.h msipl_compressed
	$(Q)$(MAKE) -C MSIPL/newipl/stage1
	$(Q)$(PYTHON) $(ARKSDK)/build-tools/ipltools/make_ipl.py MSIPL/newipl/stage1/msipl.bin MSIPL/newipl/stage1/ipl.bin reset_block 0x4000000
	$(Q)bin2c MSIPL/newipl/stage1/ipl.bin MSIPL/newipl/stage2/new_msipl.h new_msipl
	$(Q)bin2c MSIPL/newipl/stage2/msipl.bin MSIPL/newipl/stage2/msipl_raw.h msipl_raw
	$(Q)$(MAKE) PSP_MODEL=01G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_01G.bin MSIPL/newipl/msipl_01g.bin
	$(Q)$(MAKE) PSP_MODEL=02G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_02G.bin MSIPL/newipl/msipl_02g.bin
	$(Q)$(MAKE) PSP_MODEL=03G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_03G.bin MSIPL/newipl/msipl_03g.bin
	$(Q)$(MAKE) PSP_MODEL=04G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_04G.bin MSIPL/newipl/msipl_04g.bin
	$(Q)$(MAKE) PSP_MODEL=05G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_05G.bin MSIPL/newipl/msipl_05g.bin
	$(Q)$(MAKE) PSP_MODEL=07G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_07G.bin MSIPL/newipl/msipl_07g.bin
	$(Q)$(MAKE) PSP_MODEL=09G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_09G.bin MSIPL/newipl/msipl_09g.bin
	$(Q)$(MAKE) PSP_MODEL=11G -C MSIPL/newipl/stage3/
	$(Q)mv MSIPL/newipl/stage3/ipl_11G.bin MSIPL/newipl/msipl_11g.bin
	$(Q)$(MAKE) -C Installer
	$(Q)mkdir -p dist/CustomIPL/
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/CustomIPL/CIPL.ARK package.txt -s
	$(Q)cp Installer/EBOOT.PBP dist/CustomIPL/

clean:
	$(Q)$(MAKE) -C Payloadex/Nand clean
	$(Q)$(MAKE) -C Payloadex/Ms clean
	$(Q)$(MAKE) -C NewIPL clean
	$(Q)$(MAKE) -C ClassicIPL/mainbinex clean
	$(Q)$(MAKE) -C ClassicIPL/combine clean
	$(Q)$(MAKE) -C minilzo clean
	$(Q)$(MAKE) -C MSIPL/mainbinex clean
	$(Q)$(MAKE) -C MSIPL/newipl/stage1 clean
	$(Q)$(MAKE) -C MSIPL/newipl/stage2 clean
	$(Q)$(MAKE) -C MSIPL/newipl/stage3 clean
	$(Q)$(MAKE) -C Installer clean
	$(Q)rm -f MSIPL/newipl/msipl_*.bin
	$(Q)rm -rf dist
