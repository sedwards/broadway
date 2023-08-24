#include <stdio.h>

#include <glib.h>

#include <broadway-protocol.h>
#include <cairo.h>

#include "wine/gdkbroadway-server.h"
#include "wine/gdkscreen-broadway.h"
#include "wine/gdkdevicemanager-broadway.h"
#include "gdk_structs.h"

GType                   wine_broadway_display_get_type            (void);
GdkBroadwayServer *wine_broadway_server_new                      (const char         *display,
                                                                  GError            **error);

typedef struct BroadwayInput _BroadwayInput;
typedef struct _BroadwayServer BroadwayServer;
typedef struct _BroadwayServerClass BroadwayServerClass;

struct _BroadwayServer {
  GObject parent_instance;

  guint32 next_serial;
  GSocketConnection *connection;

  guint32 recv_buffer_size;
  guint8 recv_buffer[1024];

  guint process_input_idle;
  GList *incomming;

};

struct _BroadwayServerClass
{
  GObjectClass parent_class;
};

// Static def for displau
const gchar *display = ":0";   // This is lazy
			       
// Global server var, still loook for better way to do this
GdkBroadwayServer *server;        // We could change this back to GdkBroadwayServer and save a headache

void init_broadway_connection(void)
{
    GError *error;
    error = NULL;

    const char *client_port = ":0";

    //dir = g_dir_open(target.c_str(), 0, &error);
    server = wine_broadway_server_new(client_port, &error);
    if( error != NULL )
    {
        //std::cout << error->message << std::endl;
        printf("Some error in setting _server_new: %s", error-> message);
        //g_clear_error (&error);
    }

    if( server == NULL )
        printf("init_broadway_connection - For some reason server == NULL which makes no sense\n");
}

void test_broadway_display_open (const char *display)
{
  GdkBroadwayDisplay *broadway_display;
  GdkDeviceManager *broadway_device_manager; 
  const gchar *display_env_var = NULL;
  GError *error;
  error = NULL;

  display = g_object_new (GDK_TYPE_BROADWAY_DISPLAY, NULL);
  broadway_display = GDK_BROADWAY_DISPLAY (display);

  printf("finished class init \n");

  /* initialize the display's screens */
  broadway_display->screens = g_new (GdkScreen *, 1);
  //broadway_display->screens[0] = wine_broadway_screen_new (display, 0);

  printf("Screens configured\n");

  /* We need to initialize events after we have the screen
   * structures in places
   */
  wine_broadway_screen_events_init (broadway_display->screens[0]);
  printf("Screen structures in place\n");

  /*set the default screen */
  broadway_display->default_screen = broadway_display->screens[0];
  printf("Default Screen in place\n");

  //broadway_device_manager = wine_broadway_device_manager_new ((char) display);

  //gtk_init (display);

  //printf("Broadway Device Manager Initialized: %d\n", (const char) broadway_device_manager);

  //wine_broadway_display_init_dnd (*display);

  //wine_broadway_screen_setup (broadway_display->screens[0]);
  wine_broadway_screen_setup (broadway_display->default_screen);
  printf("Screen setup for default_screen\n");

  if (broadway_display->default_screen == NULL)
  {
    printf("broadway_display->default_screen == NULL - Trying to check BROADWAY_DISPLAY\n");
    printf(" This should never happen\n");

    //display_env_var g_getenv("BROADWAY_DISPLAY");
    //broadway_display->screens = display_env_var;
    broadway_display->default_screen = g_getenv("BROADWAY_DISPLAY");
    test_broadway_display_open(display);

    if( broadway_display->default_screen == NULL)
    {
	printf("broadway_display->default_screen still NULL, giving up\n");
        printf(" This should never happen\n");
	exit (0);
    }
    broadway_display->server = wine_broadway_server_new (display, &error);
    //broadway_display->server = server;
    printf("broadway_display->server %d\n", (char) broadway_display->server);
  }

  if (broadway_display->default_screen == NULL)
  {
     printf("BroadwayDisplay structure broadway_display->default_screen == NULL\n");
     printf(" This should never happen\n");
  }
}

void main()
{
   //gtk_init();

   init_broadway_connection();
   test_broadway_display_open(display);
}

