include ../Makefile.inc

MODULE=0014-echo.bin
SOURCES=$(wildcard [^_]*.c)
LIBC=$(wildcard ../Libc/*.o)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -I../Libc/include -T ../module.ld ../_loader.c $(SOURCES) $(LIBC) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean print
