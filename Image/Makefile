BOOTLOADER_PATH=../Bootloader
BMFS=$(BOOTLOADER_PATH)/BMFS/bmfs.bin
MBR=$(BOOTLOADER_PATH)/Pure64/bmfs_mbr.sys
MP=../Toolchain/ModulePacker/mp.bin
PURE64=$(BOOTLOADER_PATH)/Pure64/pure64.sys
OSIMAGENAME=x64BareBonesImage
VMDK=$(OSIMAGENAME).vmdk
QCOW2=$(OSIMAGENAME).qcow2
IMG=$(OSIMAGENAME).img
KERNEL=../Kernel/kernel.bin
USERLAND=../Userland/0000-shell.bin ../Userland/0001-sampleDataModule.bin ../Userland/0002-ts.bin ../Userland/0003-hello.bin ../Userland/0004-help.bin ../Userland/0005-date.bin ../Userland/0006-time.bin ../Userland/0007-clear.bin ../Userland/0008-roflcopter.bin ../Userland/0009-ps.bin ../Userland/0010-ls.bin ../Userland/0011-philosophers.bin ../Userland/0012-multi.bin ../Userland/0013-sem.bin ../Userland/0014-echo.bin ../Userland/0015-kill.bin ../Userland/0016-accessKernel.bin ../Userland/0017-man.bin ../Userland/0018-prodcons.bin ../Userland/idle.bin

PACKEDKERNEL=packedKernel.bin
IMGSIZE=6291456

all: $(IMG) $(VMDK) $(QCOW2)

$(KERNEL):
	cd ../Kernel; make

$(PACKEDKERNEL): $(KERNEL) $(USERLAND)
	$(MP) $(KERNEL) $(USERLAND) -o $(PACKEDKERNEL)

$(IMG): $(BMFS) $(MBR) $(PURE64) $(PACKEDKERNEL)
	$(BMFS) $(IMG) initialize $(IMGSIZE) $(MBR) $(PURE64) $(PACKEDKERNEL)

$(VMDK): $(IMG)
	qemu-img convert -f raw -O vmdk $(IMG) $(VMDK)

$(QCOW2): $(IMG)
	qemu-img convert -f raw -O qcow2 $(IMG) $(QCOW2)

clean:
	rm -rf $(IMG) $(VMDK) $(QCOW2) *.bin

.PHONY: all clean
