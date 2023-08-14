#include <stdio.h>
#include <glib.h>
#include <gio/gunixsocketaddress.h>

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

BroadwayServer * _win_broadway_server_new (const char *display, GError **error);
guint32 _win_broadway_server_get_last_seen_time (BroadwayServer *server);
void _win_broadway_server_flush (BroadwayServer *server);
void _win_broadway_server_sync (BroadwayServer *server);
void _win_broadway_server_query_mouse (BroadwayServer *server,
                                  guint32            *toplevel,
                                  gint32             *root_x,
                                  gint32             *root_y,
                                  guint32            *mask);

guint32
_win_broadway_server_new_window (BroadwayServer *server,
                                 int x,
                                 int y,
                                 int width,
                                 int height,
                                 gboolean is_temp);


gboolean _win_broadway_server_window_show (BroadwayServer *server, gint id);


int main(void)
{
    GError *error;
    error = NULL;
    guint32 retval;

    BroadwayServer *broadway_server;

    char *client_port;
    client_port = ":1";

    //dir = g_dir_open(target.c_str(), 0, &error);
    broadway_server = _win_broadway_server_new(client_port, &error);
    if( error != NULL )
    {
        //std::cout << error->message << std::endl;
	printf("Some error: %s", error-> message);
        g_clear_error (&error);
    }
    
    //printf("_win_broadway_server_new function did something: %s", error-> message);
    printf("_win_broadway_server_new function did something\n");

    retval = _win_broadway_server_new_window(broadway_server,100,100,100,100,0);
    
    printf("_win_broadway_server_new_window function did something\n");

    _win_broadway_server_window_show (broadway_server, retval);
    
    return retval;
}


