CC=gcc
CFLAGS=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0 gtk+-3.0` -fPIC -Wall -Wno-deprecated-declarations -g -I. -I../ -I../include/public /usr/local/lib/libbroadway-wine.a

all: full-sim

###
### Clean target, other useful targets
###

shapes-png-output : 
	gcc -o $@ full-sim.c /usr/local/lib/libbroadway-wine.a $(CFLAGS) 

###
### Clean target, other useful targets
###

clean:
	rm -f full-sim
	rm -f *.o *.a 

