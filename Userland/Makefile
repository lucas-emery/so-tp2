include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: libc accessKernel sampleCodeModule sampleDataModule shell hello help time date clear roflcopter ps ls philosophers multi man sem echo idle kill

libc:
	cd Libc; make

accessKernel:
	cd AccessKernel; make

sampleCodeModule:
	cd SampleCodeModule; make

kill:
	cd Kill; make

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

man:
	cd Man; make

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
	cd AccessKernel; make clean
	cd Libc; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule all clean
