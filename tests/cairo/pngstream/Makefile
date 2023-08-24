CC=gcc
CFLAGS=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0 gtk+-3.0` -fPIC -Wall -Wno-deprecated-declarations -g -I. -I./include -I./include/gdk -I./include/public
CFLAGS_CAIRO=`pkg-config --libs --cflags cairo` -fPIC -Wall -g -I. -I./include

all: clean terminal 

SRC = \
	connect-broadway-server.c \
	terminal.c \
	moving.c

#connect-broadway-server.c

#### Broadway Standalone

terminal :
	gcc -o $@ $(SRC) `pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0 gtk+-3.0` -fPIC -Wall -g -lX11

BROADWAYD_OBJS = $(patsubst %.c, %.o, $(BROADWAYD_SRC))
SERVER_OBJS = $(patsubst %.c, %.o, $(SERVER_SRC))

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
	rm -f terminal random

