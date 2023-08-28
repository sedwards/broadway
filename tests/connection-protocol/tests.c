
#include "common.h"

extern BroadwayServer *broadway_server;

typedef struct _GdkBroadwayServer GdkBroadwayServer;

struct _GdkBroadwayServer {
  GObject parent_instance;

  guint32 next_serial;
  GSocketConnection *connection;

  guint32 recv_buffer_size;
  guint8 recv_buffer[1024];

  guint process_input_idle;
  GList *incomming;
};

struct _GdkBroadwayServerClass
{
  GObjectClass parent_class;
};


typedef struct _GdkBroadwayScreen GdkBroadwayScreen;
typedef struct _GdkBroadwayScreenClass GdkBroadwayScreenClass;

typedef struct BroadwayInput _BroadwayInput;
typedef struct _BroadwayServerClass BroadwayServerClass;

struct _BroadwayServerClass
{
  GObjectClass parent_class;
};

/*
struct _GdkWindowImplBroadway
{
  GdkWindowImpl parent_instance;

  GdkWindow *wrapper;
  GdkScreen *screen;

  cairo_surface_t *surface;
  cairo_surface_t *last_surface;
  cairo_surface_t *ref_surface;

  GdkCursor *cursor;
  GHashTable *device_cursor;

  int id;

  gboolean visible;
  gboolean maximized;
  int transient_for;

  int pre_maximize_x;
  int pre_maximize_y;
  int pre_maximize_width;
  int pre_maximize_height;

  gint8 toplevel_window_type;
  gboolean dirty;
  gboolean last_synced;

  GdkGeometry geometry_hints;
  GdkWindowHints geometry_hints_mask;
};
*/

struct _GdkWindowImplBroadwayClass
{
  GdkWindowImplClass parent_class;
};

//typedef _GdkBroadwayDisplay GdkBroadwayDisplay;
struct _GdkBroadwayDisplay
{
  //GdkDisplay parent_instance;
  GdkScreen *default_screen;
  GdkScreen **screens;

  GHashTable *id_ht;
  GList *toplevels;

  GSource *event_source;

  /* Keyboard related information */
  GdkKeymap *keymap;

  /* drag and drop information */
  GdkDragContext *current_dest_drag;

  /* The offscreen window that has the pointer in it (if any) */
  GdkWindow *active_offscreen_window;

  GdkBroadwayServer *server;

  gpointer move_resize_data;

  GdkMonitor *monitor;
};

struct _GdkDisplayClass GdkDisplayClass;
struct _GdkDisplayClass
{
   int something;	// FIXME
}; 

//struct _GdkDisplayClass GdkDisplayClass;

struct _GdkBroadwayDisplayClass
{
// FIXME
//  GdkDisplayClass parent_class;
};

typedef struct _GdkBroadwayDisplay GdkBroadwayDisplay;

struct _GdkBroadwayWindow {
//  GdkWindow parent;
   int something;	// FIXME
};

struct _GdkBroadwayWindowClass {
  GdkWindowClass parent_class;
};

typedef struct _GdkBroadwayWindowClas GdkBroadwayWindowClass;
typedef struct _GdkBroadwayWindow GdkBroadwayWindow;

// Static def for displau
const gchar *display = ":0";   // This is lazy
			       
// Global server var, still loook for better way to do this
GdkBroadwayServer *server;        // We could change this back to GdkBroadwayServer and save a headache

#define WINDOW1_WIDTH 700
#define WINDOW1_HEIGHT 500

#define WINDOW2_WIDTH 700
#define WINDOW2_HEIGHT 500

///// Initialize a bunch of classes we are not really going to init /////////
void
broadway_req_screen_setup (GdkScreen *screen)
{
}

void
broadway_req_screen_events_init (GdkScreen *screen)
{
// This does nothing, for reasons...
}

GdkScreen *
broadway_screen_new (GdkDisplay *display,
                          gint   screen_number)
{
  GdkScreen *screen;

  GdkBroadwayScreen *broadway_screen;

  screen = g_object_new (GDK_TYPE_BROADWAY_SCREEN, NULL);

  broadway_screen = g_object_new (GDK_BROADWAY_SCREEN, NULL);
  broadway_screen->display = display;

  return screen;
}

void test_broadway_display_open (const char *display)
{
  GdkBroadwayDisplay *broadway_display;
  GError *error;
  error = NULL;

  display = g_object_new (GDK_TYPE_BROADWAY_DISPLAY, NULL);
  //broadway_display = GDK_BROADWAY_DISPLAY (display);

  printf("finished class init \n");

  /* initialize the display's screens */
  broadway_display->screens = g_new (GdkScreen *, 1);
  broadway_display->screens[0] = broadway_screen_new (display, 0);

  printf("Screens configured\n");

  /* We need to initialize events after we have the screen
   * structures in places
   */
  broadway_req_screen_events_init (broadway_display->screens[0]);
  printf("Screen structures in place\n");

  /*set the default screen */
  broadway_display->default_screen = broadway_display->screens[0];
  printf("Default Screen in place\n");

  /* There is no DND support
   * this is a stub interface to a stub class 
   */
  //broadway_req_display_init_dnd (*display);

  /* FIXME: Check why the specific screen fails here */
  //broadway_req_screen_setup (broadway_display->screens[0]);

  broadway_req_screen_setup (broadway_display->default_screen);
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
    broadway_display->server = broadway_req_server_new (display, &error);
    broadway_display->server = server;
    printf("broadway_display->server %d\n", (char) broadway_display->server);
  }

  if (broadway_display->default_screen == NULL)
  {
     printf("BroadwayDisplay structure broadway_display->default_screen == NULL\n");
     printf(" This should never happen\n");
  }

  broadway_req_screen_init_root_window(broadway_display->default_screen);
  printf("test_broadway_display_open - completed\n");
}

void paint_broadway_window_surface(cairo_t *cr) {
  /* Surface for each rendering of the PNG file, in effect, ever pass becomes a frame */
  cairo_surface_t* surface =  cairo_image_surface_create_from_png("moving.png");

  cairo_set_source_surface (cr,surface, 0, 0);

  cairo_paint (cr);

  // Fill the entire surface with red.
  cairo_set_source_rgb(cr, 1, 0, 0);
  cairo_rectangle(cr, 0, 0, WINDOW1_WIDTH, WINDOW1_HEIGHT);
  cairo_fill(cr);
}

void test_broadway_window(GdkBroadwayDisplay *broadway_display)
{
    guint32 id;
    cairo_surface_t *surface;
    gboolean *visable;
    struct GdkBroadwayWindowClass *broadway_window_class;
    struct GdkBroadwayWindow *broadway_window;

    id = broadway_req_server_new_window(broadway_server,0,0,WINDOW1_WIDTH,WINDOW1_HEIGHT,0);
    
    printf("broadway_req_server_new_window %d window created\n",id);

    visable = broadway_req_server_window_show (broadway_server, id);

    if(visable == TRUE)
      {
	 printf("broadway_req_server_window_show is true and function did something\n");
      }
    else
      {
	 printf("broadway_req_server_window_show is somehow false\n");
      }

    surface = broadway_req_server_create_surface(WINDOW1_WIDTH,WINDOW1_HEIGHT); 

    printf("broadway_req_server_create_surface did something\n");

    paint_broadway_window_surface(surface);

    broadway_req_server_window_update (broadway_server, id, surface);

    printf("broadway_req_server_window_update\n");

    broadway_req_server_window_show (broadway_server, id);

    if(visable == TRUE)
      {printf("broadway_req_server_window_show - Part 2 is true and function did something\n");}
    else
      {printf("broadway_req_server_window_show - Part 2 is somehow false\n");}
}

void run_unit_tests(void)
{
   struct GdkBroadwayDisplay *broadway_display;

   test_broadway_display_open(display);

   for (;;)
   {
     // FIXME: Add an arg to switch windows
     test_broadway_window(broadway_display);
     sleep(2);
   }
}

