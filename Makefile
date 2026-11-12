.PHONY: all minilzo libs_latest clean

PY = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk
BUILDTOOLS = $(CFWSDK)/build-tools
SHELL := /bin/bash
CIPL_ROOT = $(CURDIR)

LIBS_DIR = tmp/LIBS
LIBS_ARCHIVE = tmp/LIBS.zip
LIBS_URL = https://github.com/pspdev/psp-dynamic-libraries/releases/download/latest/LIBS.zip

all: minilzo libs_latest
	mkdir -p dist/CustomIPL/
	$(MAKE) CIPL_ROOT=$(CIPL_ROOT) -C Build/ARK
	$(MAKE) CIPL_ROOT=$(CIPL_ROOT) -C Build/PRO
	$(MAKE) CIPL_ROOT=$(CIPL_ROOT) -C Build/LME
	$(MAKE) -C Installer
	cp Installer/EBOOT.PBP dist/CustomIPL/
	cp $(LIBS_DIR)/ipl_update.prx dist/CustomIPL/
	cp $(LIBS_DIR)/kbooti_update.prx dist/CustomIPL/
	cp $(LIBS_DIR)/kpspident.prx dist/CustomIPL/

libs_latest:
	mkdir -p $(LIBS_DIR)
	curl --fail --location --silent --show-error "$(LIBS_URL)" -o "$(LIBS_ARCHIVE)"
	unzip -o "$(LIBS_ARCHIVE)" -d "$(LIBS_DIR)"

minilzo:
	$(MAKE) gcc -C minilzo

clean:
	$(MAKE) -C Payloadex/ARK/Nand clean
	$(MAKE) -C Payloadex/ARK/Ms clean
	$(MAKE) -C Payloadex/PRO/Nand clean
	$(MAKE) -C Payloadex/LME/Nand clean
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
	rm -rf dist tmp
