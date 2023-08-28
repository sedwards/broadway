CC=gcc -w
CFLAGS=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0 gtk+-3.0` -fPIC -Wall -Wno-deprecated-declarations -g -I. -I../../ -I../../include/public -I../../broadwayd3 

all: clean connection-protocol.exe 

CONNECTION_PROTO = \
	tests.c \
	connection-proto.c \
	apiconnection.c

###
### Clean target, other useful targets
###

connection-protocol.exe :
	gcc -o $@ $(CONNECTION_PROTO) $(CFLAGS)

###
### Clean target, other useful targets
###

clean:
	rm -f *.o *.a connection-protocol connection-protocol.exe

