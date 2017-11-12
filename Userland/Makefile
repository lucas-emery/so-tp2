include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: libc sampleCodeModule sampleDataModule shell hello help time date clear roflcopter ps ls philosophers multi sem echo idle

libc:
	cd Libc; make

sampleCodeModule:
	cd SampleCodeModule; make

sem:
	cd Sem; make

idle:
	cd Idle; make

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

shell:
	cd Shell; make

multi:
	cd MultiThreading; make

echo:
	cd Echo; make

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

philosophers:
	cd Philosophers; make

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
