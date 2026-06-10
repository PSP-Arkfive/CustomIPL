.PHONY: all minilzo cipl msipl clean

PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk
BUILDTOOLS = $(CFWSDK)/build-tools

all: minilzo cipl msipl

minilzo:
	$(MAKE) gcc -C minilzo

cipl:
	$(MAKE) -C Payloadex/Nand
	$(MAKE) -C ClassicIPL/mainbinex
	$(MAKE) -C ClassicIPL/combine
	$(MAKE) PSP_MODEL=01G -C NewIPL
	$(MAKE) PSP_MODEL=02G -C NewIPL
	$(MAKE) PSP_MODEL=03G -C NewIPL
	$(MAKE) PSP_MODEL=04G -C NewIPL
	$(MAKE) PSP_MODEL=05G -C NewIPL
	$(MAKE) PSP_MODEL=07G -C NewIPL
	$(MAKE) PSP_MODEL=09G -C NewIPL
	$(MAKE) PSP_MODEL=11G -C NewIPL

msipl:
	$(MAKE) -C Payloadex/Ms
	$(MAKE) -C MSIPL/newipl/stage2
	$(MAKE) -C MSIPL/mainbinex
	minilzo/testmini MSIPL/newipl/stage2/msipl.bin MSIPL/newipl/stage2/msipl.lzo
	bin2c MSIPL/newipl/stage2/msipl.lzo MSIPL/newipl/stage1/msipl_compressed.h msipl_compressed
	$(MAKE) -C MSIPL/newipl/stage1
	$(PYTHON) $(CFWSDK)/build-tools/ipltools/make_ipl.py MSIPL/newipl/stage1/msipl.bin MSIPL/newipl/stage1/ipl.bin reset_block 0x4000000
	bin2c MSIPL/newipl/stage1/ipl.bin MSIPL/newipl/stage2/new_msipl.h new_msipl
	bin2c MSIPL/newipl/stage2/msipl.bin MSIPL/newipl/stage2/msipl_raw.h msipl_raw
	$(MAKE) PSP_MODEL=01G -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_01G.bin MSIPL/newipl/msipl_01g.bin
	$(MAKE) PSP_MODEL=02G -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_02G.bin MSIPL/newipl/msipl_02g.bin
	$(MAKE) PSP_MODEL=03G -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_03G.bin MSIPL/newipl/msipl_03g.bin
	$(MAKE) PSP_MODEL=04G -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_04G.bin MSIPL/newipl/msipl_04g.bin
	$(MAKE) PSP_MODEL=05G -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_05G.bin MSIPL/newipl/msipl_05g.bin
	$(MAKE) PSP_MODEL=07G -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_07G.bin MSIPL/newipl/msipl_07g.bin
	$(MAKE) PSP_MODEL=09G -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_09G.bin MSIPL/newipl/msipl_09g.bin
	$(MAKE) PSP_MODEL=11G -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_11G.bin MSIPL/newipl/msipl_11g.bin
	$(MAKE) -C Installer
	mkdir -p dist/CustomIPL/
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/CustomIPL/CIPL.ARK package.txt -s
	cp Installer/EBOOT.PBP dist/CustomIPL/
	cp Resources/LIBS/*.prx dist/CustomIPL/

clean:
	$(MAKE) -C Payloadex/Nand clean
	$(MAKE) -C Payloadex/Ms clean
	$(MAKE) -C NewIPL clean
	$(MAKE) -C ClassicIPL/mainbinex clean
	$(MAKE) -C ClassicIPL/combine clean
	$(MAKE) -C minilzo clean
	$(MAKE) -C MSIPL/mainbinex clean
	$(MAKE) -C MSIPL/newipl/stage1 clean
	$(MAKE) -C MSIPL/newipl/stage2 clean
	$(MAKE) -C MSIPL/newipl/stage3 clean
	$(MAKE) -C Installer clean
	rm -f MSIPL/newipl/msipl_*.bin
	rm -rf dist
