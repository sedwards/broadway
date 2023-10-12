#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <dlfcn.h>

#include "ntstatus.h"
#define WIN32_NO_STATUS

#include "basetsd.h"
#include "windef.h"
#include "winbase.h"
#include "winreg.h"
#include "wine/server.h"
#include "wine/debug.h"
#include "wine/list.h"

#include <glib.h>
#include <gio/gunixsocketaddress.h>

#include <cairo.h>

#include <gtk/gtk.h>

#include "unixlib.h"

WINE_DEFAULT_DEBUG_CHANNEL(broadwaydrv);

struct _BroadwayServer {
  GObject parent_instance;

  guint32 next_serial;
  GSocketConnection *connection;

  guint32 recv_buffer_size;
  guint8 recv_buffer[1024];

  guint process_input_idle;
  GList *incomming;
};

typedef struct _BroadwayServer BroadwayServer;

BroadwayServer * wine_broadway_server_new (const char *display, GError **error);
guint32 wine_broadway_server_get_last_seen_time (BroadwayServer *server);
void wine_broadway_server_flush (BroadwayServer *server);
void wine_broadway_server_sync (BroadwayServer *server);
void wine_broadway_server_query_mouse (BroadwayServer *server,
                                  guint32            *toplevel,
                                  gint32             *root_x,
                                  gint32             *root_y,
                                  guint32            *mask);

guint32
wine_broadway_server_new_window (BroadwayServer *server,
                                 int x,
                                 int y,
                                 int width,
                                 int height,
                                 gboolean is_temp);


gboolean wine_broadway_server_window_show (BroadwayServer *server, gint id);

cairo_surface_t *
wine_broadway_server_create_surface (int width, int height);

void wine_broadway_server_window_update (BroadwayServer *server, gint id,
                                    cairo_surface_t *surface);


#if 0
int main(){
  // Initialize cairo.
  cairo_surface_t *surface =
    cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                WIDTH, HEIGHT);
  cairo_t *cr = cairo_create (surface);
}
#endif


void test_connection(void)
{
    GdkDisplay *display;
    GtkWidget *window;
#if 0
    int argc;
    char **argv;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_show (window);

    //dm = wine_broadway_device_manager_new (display);

    display = gdk_display_get_default ();

    gtk_main();
#endif
    GError *error;
    error = NULL;
    guint32 id;

    BroadwayServer *broadway_server;
    cairo_surface_t *surface;

    char *client_port;
    client_port = ":0";

    //dir = g_dir_open(target.c_str(), 0, &error);
    broadway_server = wine_broadway_server_new(client_port, &error);
    if( error != NULL )
    {
        //std::cout << error->message << std::endl;
        printf("Some error: %s", error-> message);
        g_clear_error (&error);
    }
    
    //printf("wine_broadway_server_new function did something: %s", error-> message);
    printf("wine_broadway_server_new function did something\n");

    id = wine_broadway_server_new_window(broadway_server,100,100,100,100,0);
    
    printf("wine_broadway_server_new_window function did something\n");

    wine_broadway_server_window_show (broadway_server, id);

    surface = wine_broadway_server_create_surface(100,100);

    wine_broadway_server_window_update (broadway_server, id, surface);

    wine_broadway_server_window_show (broadway_server, id);
    //return id;

}


