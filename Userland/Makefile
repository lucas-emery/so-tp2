include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: libc sampleCodeModule sampleDataModule shell hello help time date clear roflcopter ps ls

libc:
	cd Libc; make

sampleCodeModule:
	cd SampleCodeModule; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

shell:
	cd Shell; make

hello:
	cd Hello; make

help:
	cd Help; make

ps:
	cd Ps; make

ls:
	cd Ls; make

time:
	cd Time; make

date:
	cd Date; make

roflcopter:
	cd Roflcopter; make

clear:
	cd Clear; make	

clean:
	cd SampleCodeModule; make clean
	cd Shell; make clean
	cd Libc; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule all clean
