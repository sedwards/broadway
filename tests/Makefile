CC=gcc -w
CFLAGS=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0 gtk+-3.0` -fPIC -Wall -Wno-deprecated-declarations -g -I. -I../ -I../include/public -I../broadwayd3 

all: full-sim

CONNECTION_PROTO = \
	connection-protocol/tests.c \
	connection-protocol/connection-proto.c \
	connection-protocol/apiconnection.c

###
### Clean target, other useful targets
###

full-sim : 
	gcc -w -o $@ full-sim.c $(CFLAGS) 

broadway-client-funcs :
	gcc -o $@ broadway-client-funcs.c $(CFLAGS)

connection-protocol.exe :
	gcc -o connection-protocol/$@ $(CONNECTION_PROTO) $(CFLAGS)

#gcc -o connection-protocol/$@ connection-protocol/connection-proto.c connection-protocol/apiconnection.c $(CFLAGS)
###
### Clean target, other useful targets
###

clean:
	rm -f full-sim
	rm -f *.o *.a 

