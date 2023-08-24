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

#define WIDTH 700
#define HEIGHT 500

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

  /* There is no DND support
   * this is a stub interface to a stub class 
   */
  //wine_broadway_display_init_dnd (*display);

  /* FIXME: Check why the specific screen fails here */
  //wine_broadway_screen_setup (broadway_display->screens[0]);

  wine_broadway_screen_setup (broadway_display->default_screen);
  printf("Screen setup for default_screen\n");

  if (broadway_display->default_screen == NULL)
  {
    printf("broadway_display->default_screen == NULL - Trying to check BROADWAY_DISPLAY\n");
    printf(" This should never happen\n");

    //display_env_var = g_getenv("BROADWAY_DISPLAY");
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

void test_broadway_window(GdkBroadwayDisplay *broadway_display)
    gunit32 *id;
    cairo_surface_t *surface;

    id = wine_broadway_server_new_window(server,0,0,100,100,0);
    printf("wine_broadway_server_new_window function did something\n");

    wine_broadway_server_window_show (server, id);
    printf("wine_broadway_server_window_show function did something\n");

    surface = wine_broadway_server_create_surface(100,100);
    printf("wine_broadway_server_create_surface did something\n");

    wine_broadway_server_window_update (server, id, surface);
    printf("wine_broadway_server_window_update\n");

    wine_broadway_server_window_show (server, id);
    printf("wine_broadway_server_window_show again\n");
}

void main()
{
   //gtk_init();

   struct GdkBroadwayDisplay *broadway_display;

   init_broadway_connection();
   broadway_display = test_broadway_display_open(display);
   test_broadway_window(GdkBroadwayDisplay *broadway_display);
}

#if 0
int create_window_surface(cairo_t *cr) {
  /* Surface for each rendering of the PNG file, in effect, ever pass becomes a frame */
  cairo_surface_t* surface =  cairo_image_surface_create_from_png("moving.png");

  cairo_set_source_surface (cr,surface, 0, 0);

  cairo_paint (cr);

  //cairo_t *cr = cairo_create (surface);
  cairo_status_t retm1;

  // Fill the entire surface with red.
  cairo_set_source_rgb(cr, 1, 0, 0);
  cairo_rectangle(cr, 0, 0, WIDTH, HEIGHT);
  cairo_fill(cr);
}
#endif

#if 0
/* We are so not here yet */
void test_kbd_and_mouse()
{
  //broadway_device_manager = wine_broadway_device_manager_new ((char) display);
  //printf("Broadway Device Manager Initialized: %d\n", (const char) broadway_device_manager);
}
