PYTHON = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk
BUILDTOOLS = $(CFWSDK)/build-tools

.PHONY: all ipl_660

all: ipl_660

ipl_660:
# Nand
	$(MAKE) -C $(CIPL_ROOT)/Payloadex/ARK/Nand
	$(MAKE) -C $(CIPL_ROOT)/ClassicIPL/mainbinex
	$(MAKE) -C $(CIPL_ROOT)/ClassicIPL/combine
	$(MAKE) PSP_MODEL=01G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/NewIPL
	$(MAKE) PSP_MODEL=02G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/NewIPL
	$(MAKE) PSP_MODEL=03G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/NewIPL
	$(MAKE) PSP_MODEL=04G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/NewIPL
	$(MAKE) PSP_MODEL=05G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/NewIPL
	$(MAKE) PSP_MODEL=07G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/NewIPL
	$(MAKE) PSP_MODEL=09G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/NewIPL
	$(MAKE) PSP_MODEL=11G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/NewIPL
# MS
	$(MAKE) -C $(CIPL_ROOT)/Payloadex/ARK/Ms
	$(MAKE) -C $(CIPL_ROOT)/MSIPL/newipl/stage2
	$(MAKE) -C $(CIPL_ROOT)/MSIPL/mainbinex
	$(CIPL_ROOT)/minilzo/testmini $(CIPL_ROOT)/MSIPL/newipl/stage2/msipl.bin $(CIPL_ROOT)/MSIPL/newipl/stage2/msipl.lzo
	bin2c $(CIPL_ROOT)/MSIPL/newipl/stage2/msipl.lzo $(CIPL_ROOT)/MSIPL/newipl/stage1/msipl_compressed.h msipl_compressed
	$(MAKE) -C $(CIPL_ROOT)/MSIPL/newipl/stage1
	$(PYTHON) $(CFWSDK)/build-tools/ipltools/make_ipl.py $(CIPL_ROOT)/MSIPL/newipl/stage1/msipl.bin $(CIPL_ROOT)/MSIPL/newipl/stage1/ipl.bin reset_block 0x4000000
	bin2c $(CIPL_ROOT)/MSIPL/newipl/stage1/ipl.bin $(CIPL_ROOT)/MSIPL/newipl/stage2/new_msipl.h new_msipl
	bin2c $(CIPL_ROOT)/MSIPL/newipl/stage2/msipl.bin $(CIPL_ROOT)/MSIPL/newipl/stage2/msipl_raw.h msipl_raw
	$(MAKE) PSP_MODEL=01G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/MSIPL/newipl/stage3/
	mv $(CIPL_ROOT)/MSIPL/newipl/stage3/ipl_01G.bin $(CIPL_ROOT)/MSIPL/newipl/msipl_01g.bin
	$(MAKE) PSP_MODEL=02G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/MSIPL/newipl/stage3/
	mv $(CIPL_ROOT)/MSIPL/newipl/stage3/ipl_02G.bin $(CIPL_ROOT)/MSIPL/newipl/msipl_02g.bin
	$(MAKE) PSP_MODEL=03G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/MSIPL/newipl/stage3/
	mv $(CIPL_ROOT)/MSIPL/newipl/stage3/ipl_03G.bin $(CIPL_ROOT)/MSIPL/newipl/msipl_03g.bin
	$(MAKE) PSP_MODEL=04G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/MSIPL/newipl/stage3/
	mv $(CIPL_ROOT)/MSIPL/newipl/stage3/ipl_04G.bin $(CIPL_ROOT)/MSIPL/newipl/msipl_04g.bin
	$(MAKE) PSP_MODEL=05G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/MSIPL/newipl/stage3/
	mv $(CIPL_ROOT)/MSIPL/newipl/stage3/ipl_05G.bin $(CIPL_ROOT)/MSIPL/newipl/msipl_05g.bin
	$(MAKE) PSP_MODEL=07G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/MSIPL/newipl/stage3/
	mv $(CIPL_ROOT)/MSIPL/newipl/stage3/ipl_07G.bin $(CIPL_ROOT)/MSIPL/newipl/msipl_07g.bin
	$(MAKE) PSP_MODEL=09G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/MSIPL/newipl/stage3/
	mv $(CIPL_ROOT)/MSIPL/newipl/stage3/ipl_09G.bin $(CIPL_ROOT)/MSIPL/newipl/msipl_09g.bin
	$(MAKE) PSP_MODEL=11G PSP_FW=660 PSP_CFW=ARK -C $(CIPL_ROOT)/MSIPL/newipl/stage3/
	mv $(CIPL_ROOT)/MSIPL/newipl/stage3/ipl_11G.bin $(CIPL_ROOT)/MSIPL/newipl/msipl_11g.bin
# pkg
	cd $(CIPL_ROOT) && $(PYTHON) $(BUILDTOOLS)/pack/pack.py -p dist/CustomIPL/CIPL.ARK Package/pkg_ark.txt -s
