CC=gcc
CFLAGS=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0 gtk+-3.0` -fPIC -Wall -Wno-deprecated-declarations -g -I. -I./include -I./include/gdk -I./include/public

all: hello-world shapes-png-output

###
### Clean target, other useful targets
###

shapes-png-output : 
	gcc -o $@ shapes-png-output.c $(CFLAGS) 

hello-world : 
	gcc -o $@ hello-world.c $(CFLAGS) -lX11


###
### Clean target, other useful targets
###

clean:
	rm -f hello-world shapes-png-output 
	rm -f *.o *.a 

