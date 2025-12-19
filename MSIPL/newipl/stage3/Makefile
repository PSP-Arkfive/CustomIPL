PSPDEV = $(shell psp-config --pspdev-path)
PSPSDK = $(shell psp-config --pspsdk-path)
ARKSDK = $(PSPDEV)/share/ark-dev-sdk
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
	$(Q)$(MAKE) -C $(NEWIPL)/ipl_stage2_payload BFLAGS="-DIPL_$(PSP_MODEL) -DPSP_MODEL=$(PSP_MODEL) -DMSIPL=1"
	$(Q)$(MAKE) -C $(NEWIPL)/ipl_stage1_payload BFLAGS="-DIPL_$(PSP_MODEL) -DPSP_MODEL=$(PSP_MODEL) -DMSIPL=1"
	$(Q)dd if=$(NEWIPL)/ipl_stage1_payload/ipl_stage1_payload.bin of=ipl_$(PSP_MODEL).bin
	$(Q)$(PYTHON) $(ARKSDK)/build-tools/ipltools/unpack_ipl.py $(RESOURCES)/nandipl_$(PSP_MODEL).bin ipl_$(PSP_MODEL).dec $(XOR_VAL)
	$(Q)dd if=ipl_$(PSP_MODEL).dec of=ipl_$(PSP_MODEL).bin bs=1 seek=49152
	$(Q)$(MAKE) clean -C $(NEWIPL)/ipl_stage2_payload
	$(Q)$(MAKE) clean -C $(NEWIPL)/ipl_stage1_payload
	$(Q)rm ipl_$(PSP_MODEL).dec
	$(Q)bin2c ipl_$(PSP_MODEL).bin msipl_$(PSP_MODEL).h msipl_$(PSP_MODEL)

clean:
	$(Q)rm -f *.h *.bin *.dec | true
	$(Q)rm ipl_stage2_payload/ipl_stage2_payload.h | true
