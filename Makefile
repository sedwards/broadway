CC=gcc
CFLAGS=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0` -fPIC -Wall -O2 -I. -I./include
CFLAGS_GTK=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0 gtk+-3.0` -fPIC -Wall -O2 -I. -I./include

LIBBROADWAY=libbroadway.a
#LIBBROADWAY-GDK3=libbroadway-gdk3.a
LIBBROADWAY-WIN=libbroadway-win.a

all: broadwayd-gtk3 broadwayd-gtk4 libbroadway-win.a

SERVER_SRC = \
       broadway-server.c \
       broadway-buffer.c \
       broadway-output.c

BROADWAYD_SRC = \
	$(SERVER_SRC) \
	broadwayd.c

#### Broadway Standalone Daemon

broadwayd-gtk3 :
	gcc -o $@ $(BROADWAYD_SRC) `pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0` -fPIC -Wall

broadwayd-gtk4 :
	gcc -o $@ $(BROADWAYD_SRC) `pkg-config --libs --cflags gtk4` -fPIC -DGTK_MAJOR_VERSION=4 -Wall

##############################################################################################################
###ls -l win/*.c | awk '{print "\t" $9 " \/"}' | awk '{sub(/.$/,"")}1'

#GDK3_BROADWAY_SRC = \
	gdk3/gdkbroadway-server.c \
	gdk3/gdkcairocontext-broadway.c \
	gdk3/gdkcursor-broadway.c \
	gdk3/gdkdevice-broadway.c \
	gdk3/gdkdevicemanager-broadway.c \
	gdk3/gdkdisplay-broadway.c \
	gdk3/gdkdnd-broadway.c \
	gdk3/gdkdrawcontext-broadway.c \
	gdk3/gdkeventsource.c \
	gdk3/gdkglobals-broadway.c \
	gdk3/gdkkeys-broadway.c \
	gdk3/gdkmonitor-broadway.c \
	gdk3/gdkproperty-broadway.c \
	gdk3/gdkscreen-broadway.c \
	gdk3/gdkselection-broadway.c \
	gdk3/gdksurface-broadway.c \
	gdk3/gdktestutils-broadway.c \
	gdk3/gdkvisual-broadway.c 
#	gdk3/gdkwindow-broadway.c \

WIN_BROADWAY_SRC = \
	win/eventsource.c \
	win/winbroadway-server.c

#win/cairocontext-broadway.c \
	win/cursor-broadway.c \
	win/device-broadway.c \
	win/devicemanager-broadway.c \
	win/display-broadway.c \
	win/dnd-broadway.c \
	win/drawcontext-broadway.c \
	win/eventsource.c \
	win/globals-broadway.c \
	win/keys-broadway.c \
	win/monitor-broadway.c \
	win/property-broadway.c \
	win/screen-broadway.c \
	win/selection-broadway.c \
	win/surface-broadway.c \
	win/testutils-broadway.c \
	win/visual-broadway.c \
	win/window-broadway.c \
	win/winbroadway-server.c \

GDK3_BROADWAY_OBJS = $(patsubst %.c, %.o, $(GDK3_BROADWAY_SRC))
GDK4_BROADWAY_OBJS = $(patsubst %.c, %.o, $(GDK4_BROADWAY_SRC))
WIN_BROADWAY_OBJS = $(patsubst %.c, %.o, $(WIN_BROADWAY_SRC))
BROADWAYD_OBJS = $(patsubst %.c, %.o, $(BROADWAYD_SRC))
SERVER_OBJS = $(patsubst %.c, %.o, $(SERVER_SRC))

$(LIBBROADWAY) : $(SERVER_OBJS)
	ar rcs $@ $^

$(LIBBROADWAY-WIN) : $(SERVER_OBJS) $(WIN_BROADWAY_OBJS)
	ar rcs $@ $^

$(LIBBROADWAY-GDK3) : $(SERVER_OBJS) $(GDK3_BROADWAY_OBJS)
	ar rcs $@ $^

###
### Clean target, other useful targets
###

testsocket : 
	gcc -o tests/$@ tests/testsocket.c tests/testsocket_common.c $(CFLAGS_GTK) 

testsocket_child :
	gcc -o tests/$@ tests/testsocket_child.c tests/testsocket_common.c $(CFLAGS_GTK)

###
### Clean target, other useful targets
###

clean:
	rm -f broadwayd-gtk3 broadwayd-gtk4 
	rm -f win/*.o *.o *.a 
	rm -f tests/testsocket tests/testsocket_child

