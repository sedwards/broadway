CC=gcc -w
CFLAGS=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0 gtk+-3.0` -fPIC -Wall -Wno-deprecated-declarations -g -I. -I../ -I../include/public -I../broadwayd3 /usr/local/lib/libbroadway-wine.a 

all: full-sim

###
### Clean target, other useful targets
###

full-sim : 
	gcc -w -o $@ full-sim.c gdkwindow-broadway.c $(CFLAGS) 

###
### Clean target, other useful targets
###

clean:
	rm -f full-sim
	rm -f *.o *.a 

