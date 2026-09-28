build_ipl_pro_660:
# Nand
	$(MAKE) -C Payloadex/PRO/Nand clean
	$(MAKE) -C Payloadex/PRO/Nand
	$(MAKE) PSP_MODEL=01G PSP_FW=660 PSP_CFW=PRO -C NewIPL
	$(MAKE) PSP_MODEL=02G PSP_FW=660 PSP_CFW=PRO -C NewIPL
	$(MAKE) PSP_MODEL=03G PSP_FW=660 PSP_CFW=PRO -C NewIPL
	$(MAKE) PSP_MODEL=04G PSP_FW=660 PSP_CFW=PRO -C NewIPL
	$(MAKE) PSP_MODEL=05G PSP_FW=660 PSP_CFW=PRO -C NewIPL
	$(MAKE) PSP_MODEL=07G PSP_FW=660 PSP_CFW=PRO -C NewIPL
	$(MAKE) PSP_MODEL=09G PSP_FW=660 PSP_CFW=PRO -C NewIPL
	$(MAKE) PSP_MODEL=11G PSP_FW=660 PSP_CFW=PRO -C NewIPL
# MS
	$(MAKE) -C Payloadex/PRO/Ms clean
	$(MAKE) -C Payloadex/PRO/Ms
	$(MAKE) -C MSIPL/newipl/stage2
	minilzo/testmini MSIPL/newipl/stage2/msipl.bin MSIPL/newipl/stage2/msipl.lzo
	bin2c MSIPL/newipl/stage2/msipl.lzo MSIPL/newipl/stage1/msipl_compressed.h msipl_compressed
	$(MAKE) -C MSIPL/newipl/stage1
	$(PYTHON) $(CFWSDK)/build-tools/ipltools/make_ipl.py MSIPL/newipl/stage1/msipl.bin MSIPL/newipl/stage1/ipl.bin reset_block 0x4000000
	bin2c MSIPL/newipl/stage1/ipl.bin MSIPL/newipl/stage2/new_msipl.h new_msipl
	bin2c MSIPL/newipl/stage2/msipl.bin MSIPL/newipl/stage2/msipl_raw.h msipl_raw
	$(MAKE) PSP_MODEL=01G PSP_FW=660 PSP_CFW=PRO -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_01G.bin MSIPL/newipl/msipl_01g.bin
	$(MAKE) PSP_MODEL=02G PSP_FW=660 PSP_CFW=PRO -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_02G.bin MSIPL/newipl/msipl_02g.bin
	$(MAKE) PSP_MODEL=03G PSP_FW=660 PSP_CFW=PRO -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_03G.bin MSIPL/newipl/msipl_03g.bin
	$(MAKE) PSP_MODEL=04G PSP_FW=660 PSP_CFW=PRO -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_04G.bin MSIPL/newipl/msipl_04g.bin
	$(MAKE) PSP_MODEL=05G PSP_FW=660 PSP_CFW=PRO -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_05G.bin MSIPL/newipl/msipl_05g.bin
	$(MAKE) PSP_MODEL=07G PSP_FW=660 PSP_CFW=PRO -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_07G.bin MSIPL/newipl/msipl_07g.bin
	$(MAKE) PSP_MODEL=09G PSP_FW=660 PSP_CFW=PRO -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_09G.bin MSIPL/newipl/msipl_09g.bin
	$(MAKE) PSP_MODEL=11G PSP_FW=660 PSP_CFW=PRO -C MSIPL/newipl/stage3/
	mv MSIPL/newipl/stage3/ipl_11G.bin MSIPL/newipl/msipl_11g.bin
# pkg
	mkdir -p dist/CustomIPL/
	$(PY) $(BUILDTOOLS)/pack/pack.py -p dist/CustomIPL/CIPL660.PRO Package/pkg_pro_660.txt -s
