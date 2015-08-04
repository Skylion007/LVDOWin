CC=gcc -I../Include -L..\Lib 
override CFLAGS+=-Wall -O3 #$(shell pkg-config --cflags fftw3)
#override LDFLAGS+=#$(shell pkg-config --libs fftw3 glib-2.0) -lm 


.PHONY: all clean

all: lvdoenc lvdodec finale

lvdoenc: lvdoenc.o lvdomain.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -I../Include -L../Lib -lfftw3
lvdodec: lvdodec.o lvdomain.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -I../Include -L../Lib -lfftw3
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^  
lvdoenc.c: lvdocommon.h
lvdodec.c: lvdocommon.h

finale:
	rm -f lvdoenc.o lvdodec.o  lvdomain.o
	mv lvdoenc.exe ../Bin/lvdoenc.exe
	mv lvdodec.exe ../Bin/lvdodec.exe
	
clean:
	rm -f lvdoenc.exe lvdodec.exe lvdomain.o lvdoenc.o lvdodec.o

	