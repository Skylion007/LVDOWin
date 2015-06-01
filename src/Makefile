CC=gcc -I../Include -L..\Lib 
override CFLAGS+=-Wall -O3 #$(shell pkg-config --cflags fftw3)
#override LDFLAGS+=#$(shell pkg-config --libs fftw3 glib-2.0) -lm 


.PHONY: all clean

all: lvdoenc lvdodec

lvdoenc: lvdoenc.o lvdomain.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -I../Include -L../Lib -lfftw3
lvdodec: lvdodec.o lvdomain.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -I../Include -L../Lib -lfftw3
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^  
lvdoenc.c: lvdocommon.h
lvdodec.c: lvdocommon.h

	
clean:
	rm -f lvdoenc lvdodec lvdomain.o lvdoenc.o lvdodec.o