###
### All target. Maybe at some point we will have some deps too
###

all: broadwayd-gtk3 broadway-gtk4

###
### Basic GTK Demos
###

OBJS = \
       broadway-server.c \
       broadway-buffer.c \
       broadway-output.c \
       broadwayd.c

broadwayd-gtk3 :
	gcc -o $@ $(OBJS) `pkg-config --libs --cflags gtk+-3.0` -fPIC

broadway-gtk4 :
	gcc -o $@ $(OBJS) `pkg-config --libs --cflags gtk4` -fPIC -DGTK_MAJOR_VERSION=4

###
### Clean target, other useful targets
###

clean:
	rm -f broadwayd-gtk3 broadwayd-gtk4

