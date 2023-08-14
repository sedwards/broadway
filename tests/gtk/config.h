#ifndef G_OS_UNIX
#define G_OS_UNIX
#endif

#define HAVE_SYS_MMAN_H 1

#define HAVE_UNISTD_H 1

#if GTK_MAJOR_VERSION == 4 
#ifndef GDK_GRAB_ALREADY_GRABBED
#define GDK_GRAB_ALREADY_GRABBED 1
#endif 

#ifndef GDK_GRAB_SUCCESS
#define GDK_GRAB_SUCCESS 1
#endif
#endif

#define GETTEXT_PACKAGE 0

//#ifndef GDK_WINDOWING_X11
//#define GDK_WINDOWING_X11 1
//#endif
