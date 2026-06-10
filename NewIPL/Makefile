PSPDEV = $(shell psp-config --pspdev-path)
PSPSDK = $(shell psp-config --pspsdk-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk

RESOURCES ?= ../Resources

ifeq ($(PSP_MODEL), 01G)
XOR_VAL = 
else
ifeq ($(PSP_MODEL), 02G)
XOR_VAL = 
else
ifeq ($(PSP_MODEL), 05G)
XOR_VAL = --xor 2
else
XOR_VAL = --xor 1
endif
endif
endif


all:
	$(MAKE) -C ipl_stage2_payload BFLAGS="-DIPL_$(PSP_MODEL) -DPSP_MODEL=$(PSP_MODEL)"
	$(MAKE) -C ipl_stage1_payload BFLAGS="-DIPL_$(PSP_MODEL) -DPSP_MODEL=$(PSP_MODEL)"
	$(PYTHON) $(CFWSDK)/build-tools/ipltools/unpack_ipl.py $(RESOURCES)/nandipl_$(PSP_MODEL).bin ipl_$(PSP_MODEL).dec $(XOR_VAL)
	dd if=ipl_stage1_payload/ipl_stage1_payload.bin of=ipl_$(PSP_MODEL).bin
	dd if=ipl_$(PSP_MODEL).dec of=ipl_$(PSP_MODEL).bin bs=1 seek=49152
	$(PYTHON) $(CFWSDK)/build-tools/ipltools/make_ipl.py ipl_$(PSP_MODEL).bin cipl_$(PSP_MODEL).bin reset_block 0x40e0000
	bin2c cipl_$(PSP_MODEL).bin cipl_$(PSP_MODEL).h cipl_$(PSP_MODEL)
	bin2c $(RESOURCES)/nandipl_$(PSP_MODEL).bin origipl_$(PSP_MODEL).h origipl_$(PSP_MODEL)
	$(MAKE) clean -C ipl_stage2_payload
	$(MAKE) clean -C ipl_stage1_payload

clean:
	rm -f origipl_*.h cipl_*.h cipl_*.bin ipl_*.bin ipl_*.dec | true
	rm -f ipl_stage1_payload/*.o | true
	rm -f ipl_stage1_payload/*.bin | true
	rm -f ipl_stage2_payload/ipl_stage2_payload.h | true
	rm -f ipl_stage2_payload/*.o | true
	rm -f ipl_stage2_payload/*.bin | true
