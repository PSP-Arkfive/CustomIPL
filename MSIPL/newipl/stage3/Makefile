PSPDEV = $(shell psp-config --pspdev-path)
PSPSDK = $(shell psp-config --pspsdk-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk
PYTHON = $(shell which python3)

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

NEWIPL = ../../../NewIPL
RESOURCES = ../../../Resources

all:
	$(MAKE) -C $(NEWIPL)/ipl_stage2_payload BFLAGS="-DIPL_$(PSP_MODEL) -DPSP_MODEL=$(PSP_MODEL) -DMSIPL=1"
	$(MAKE) -C $(NEWIPL)/ipl_stage1_payload BFLAGS="-DIPL_$(PSP_MODEL) -DPSP_MODEL=$(PSP_MODEL) -DMSIPL=1"
	dd if=$(NEWIPL)/ipl_stage1_payload/ipl_stage1_payload.bin of=ipl_$(PSP_MODEL).bin
	$(PYTHON) $(CFWSDK)/build-tools/ipltools/unpack_ipl.py $(RESOURCES)/nandipl_$(PSP_MODEL).bin ipl_$(PSP_MODEL).dec $(XOR_VAL)
	dd if=ipl_$(PSP_MODEL).dec of=ipl_$(PSP_MODEL).bin bs=1 seek=49152
	$(MAKE) clean -C $(NEWIPL)/ipl_stage2_payload
	$(MAKE) clean -C $(NEWIPL)/ipl_stage1_payload
	rm ipl_$(PSP_MODEL).dec
	bin2c ipl_$(PSP_MODEL).bin msipl_$(PSP_MODEL).h msipl_$(PSP_MODEL)

clean:
	rm -f *.h *.bin *.dec | true
	rm ipl_stage2_payload/ipl_stage2_payload.h | true
