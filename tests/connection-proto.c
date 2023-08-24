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

#include <gtk/gtk.h>

int main (int argc, char *argv[])
{
    GError *error;
    error = NULL;
    guint32 id;

    GdkDisplay *display;
    GdkDeviceManager *dm;
    GdkScreen *screen; 
    GdkWindow *root_window;
    GdkWindow *gdk_window;
    GtkWindow *gtk_window;
    struct _GdkWindowAttr window_attributes;



    window_attributes.title="Broadway";
    window_attributes.event_mask=0;
    window_attributes.x=1000;
    window_attributes.y=1000;
    window_attributes.width=1000;
    window_attributes.height=1000;
  //GdkWindowWindowClass wclass;
 //GdkVisual *visual;
    window_attributes.window_type=GDK_WINDOW_TOPLEVEL;
  //GdkCursor *cursor;
    //window_attributes.wmclass_name;
    //window_attributes.wmclass_class="wm_class_type_fixme"
    window_attributes.override_redirect=0;
  //GdkWindowTypeHint type_hint;


     BroadwayServer *broadway_server;
    cairo_surface_t *surface;
    //GdkDisplay *display;
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
    printf("New broadway server\n");

    //gtk_init (&argc, &argv);
    gdk_init (&argc, &argv);

    printf("gtk init\n\n\n\n");
   // screen = gdk_screen_get_default ();
   
    //dm = wine_broadway_device_manager_new (display);

//    display = gdk_display_get_default ();
printf("display_get_default\n\n\n\n\n");
// gchar *
//gdk_get_display_arg_name (void)

//GdkDisplay *
//gdk_display_open_default (void)


//    if(wine_broadway_display_supports_cursor_alpha (display))
//	    printf("Broadway says it supports alpha channel cursor\n");

//    if(wine_broadway_display_supports_cursor_color(display))
//	    printf("Display supports cursor color\n");

//    wine_broadway_display_get_type();
//    printf("wine_broadway_display_get_type\n");

#if 0
    printf("calling wine_broadway_display_open\n");
    display = wine_broadway_display_open(client_port);
    printf("wine_broadway_display_open\n");

    //printf("wine_broadway_server_new function did something: %s", error-> message);
    //printf("wine_broadway_server_new function did something\n");

    id = wine_broadway_server_new_window(broadway_server,100,100,100,100,0);
    printf("wine_broadway_server_new_window function did something\n");

    wine_broadway_server_window_show (broadway_server, id);
    printf("wine_broadway_server_window_show function did something\n");

    surface = wine_broadway_server_create_surface(100,100);
    printf("wine_broadway_server_create_surface did something\n");

    wine_broadway_server_window_update (broadway_server, id, surface);
    printf("wine_broadway_server_window_update\n");

    wine_broadway_server_window_show (broadway_server, id);
    printf("wine_broadway_server_window_show again\n");
    //return id;
#endif
//    gtk_main();
    gtk_main();

    return 0;
}

