.PHONY: all minilzo clean

PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk
BUILDTOOLS = $(CFWSDK)/build-tools
SHELL := /bin/bash
CIPL_ROOT = $(CURDIR)

all: minilzo
	mkdir -p dist/CustomIPL/
	$(MAKE) CIPL_ROOT=$(CIPL_ROOT) -C Build/ARK
	$(MAKE) CIPL_ROOT=$(CIPL_ROOT) -C Build/PRO
	$(MAKE) CIPL_ROOT=$(CIPL_ROOT) -C Build/LME
	$(MAKE) -C Installer
	cp Installer/EBOOT.PBP dist/CustomIPL/
#	cp Resources/LIBS/*.prx dist/CustomIPL/

minilzo:
	$(MAKE) gcc -C minilzo

clean:
	$(MAKE) -C Payloadex/ARK/Nand clean
	$(MAKE) -C Payloadex/ARK/Ms clean
	$(MAKE) -C Payloadex/PRO/Nand clean
	$(MAKE) -C Payloadex/PRO/Ms clean
	$(MAKE) -C Payloadex/LME/Nand clean
	$(MAKE) -C Payloadex/LME/Ms clean
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
