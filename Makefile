CC=gcc
CFLAGS=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0 gtk+-3.0` -fPIC -Wall -Wno-deprecated-declarations -g -I. -I./include -I./include/gdk -I./include/public
CFLAGS_SANS_GTK=`pkg-config --libs --cflags gio-2.0 gobject-2.0 cairo glib-2.0` -fPIC -Wall -g -I. -I./include

LIBBROADWAY=libbroadway.a
LIBBROADWAY-GDK3=libbroadway-gdk3.a
LIBBROADWAY-WINE=libbroadway-wine.a
#LIBBROADWAY-WIN=libbroadway-win.a

all: broadwayd-gtk3 broadwayd-gtk4 libbroadway-gdk3.a libbroadway-wine.a testsocket connection-proto

SERVER_SRC = \
       broadwayd3/broadway-server.c \
       broadwayd3/broadway-buffer.c \
       broadwayd3/broadway-output.c

BROADWAYD3_SRC = \
	$(SERVER_SRC) \
	broadwayd3/broadwayd.c

#### Broadway Standalone Daemon

broadwayd-gtk3 :
	gcc -o bin/$@ $(BROADWAYD3_SRC) $(CFLAGS) 

broadwayd-gtk4 :
	gcc -o bin/$@ $(BROADWAYD4_SRC) `pkg-config --libs --cflags gtk4` -fPIC -DGTK_MAJOR_VERSION=4 -Wall

##############################################################################################################
###ls -l win/*.c | awk '{print "\t" $9 " \/"}' | awk '{sub(/.$/,"")}1'

GDK3_BROADWAY_SRC = \
	wine/gdkbroadway-server.c \
	wine/gdkcursor-broadway.c \
	wine/gdkdevice-broadway.c \
	wine/gdkdevicemanager-broadway.c \
	wine/gdkdisplay-broadway.c \
	wine/gdkdnd-broadway.c \
	wine/gdkeventsource.c \
	wine/gdkglobals-broadway.c \
	wine/gdkkeys-broadway.c \
	wine/gdkmonitor-broadway.c \
	wine/gdkproperty-broadway.c \
	wine/gdkscreen-broadway.c \
	wine/gdkselection-broadway.c \
	wine/gdktestutils-broadway.c \
	wine/gdkvisual-broadway.c \
	wine/gdkwindow-broadway.c \
	wine/port.c

WIN_BROADWAY_SRC = \
	win/eventsource.c \
	win/winbroadway-server.c

WINE_BROADWAY_SRC = \
        wine/gdkbroadway-server.c \
        wine/gdkcursor-broadway.c \
        wine/gdkdevice-broadway.c \
        wine/gdkdevicemanager-broadway.c \
        wine/gdkdisplay-broadway.c \
        wine/gdkdnd-broadway.c \
        wine/gdkeventsource.c \
        wine/gdkglobals-broadway.c \
        wine/gdkkeys-broadway.c \
        wine/gdkmonitor-broadway.c \
        wine/gdkproperty-broadway.c \
        wine/gdkscreen-broadway.c \
        wine/gdkselection-broadway.c \
        wine/gdktestutils-broadway.c \
        wine/gdkvisual-broadway.c \
        wine/gdkwindow-broadway.c

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
WIN_BROADWAY_OBJS = $(patsubst %.c, %.o, $(WIN_BROADWAY_SRC))
WINE_BROADWAY_OBJS = $(patsubst %.c, %.o, $(WINE_BROADWAY_SRC))
BROADWAYD3_OBJS = $(patsubst %.c, %.o, $(BROADWAYD3_SRC))
BROADWAYD4_OBJS = $(patsubst %.c, %.o, $(BROADWAYD4_SRC))
SERVER_OBJS = $(patsubst %.c, %.o, $(SERVER_SRC))


$(LIBBROADWAY) : $(SERVER_OBJS)
	ar rcs $@ $^

#$(LIBBROADWAY-WIN) : $(SERVER_OBJS) $(WIN_BROADWAY_OBJS)
#	ar rcs $@ $^

$(LIBBROADWAY-WINE) : $(SERVER_OBJS) $(WINE_BROADWAY_OBJS)
	ar rcs $@ $^

$(LIBBROADWAY-GDK3) : $(SERVER_OBJS) $(GDK3_BROADWAY_OBJS)
	ar rcs $@ $^


###
### Clean target, other useful targets
###

testsocket : 
	gcc -o tests/gtk/$@ tests/gtk/testsocket.c tests/gtk/testsocket_common.c $(CFLAGS)

connection-proto :
	gcc -o tests/$@ tests/connection-proto.c $(LIBBROADWAY-WINE) $(CFLAGS)

full-sim :
	gcc -o tests/$@ tests/full-sim.c $(LIBBROADWAY-WINE) $(CFLAGS)
###
### Clean target, other useful targets
###

clean:
	rm -f bin/broadwayd-gtk3 bin/broadwayd-gtk4 
	rm -f gdk3/*.o win/*.o wine/*.o *.a 
	rm -f tests/gtk/testsocket tests/connection-proto

