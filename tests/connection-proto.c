#include <stdio.h>
#include <glib.h>
#include <gio/gunixsocketaddress.h>

#include <cairo.h>

#include <gdk/gdk.h>

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

#define wine_broadway gdk_broadway

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

void wine_broadway_display_get_type(void);

GdkDisplay *
wine_broadway_display_open (const gchar *display_name);


int main(void)
{
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

    wine_broadway_display_get_type();

    wine_broadway_display_open(":0");
    printf("wine_broadway_display_open\n");

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


