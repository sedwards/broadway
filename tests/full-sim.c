#include <stdio.h>

#include <glib.h>

#include <broadway-protocol.h>
#include <cairo.h>

#include "wine/gdkbroadway-server.h"
#include "wine/gdkscreen-broadway.h"
#include "wine/gdkdevicemanager-broadway.h"
#include "gdkwindow-broadway.h"
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

// Initialze the Window class ourselves as we can't use all of the GDK/GTK functionality
GdkWindowImplBroadwayClass *klass;

#define WINDOW1_WIDTH 700
#define WINDOW1_HEIGHT 500

#define WINDOW2_WIDTH 700
#define WINDOW2_HEIGHT 500

#if 0
static void
gdk_window_impl_broadway_class_init (_GdkWindowImplBroadwayClass2 *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GdkWindowImplClass2 *impl_class = GDK_WINDOW_IMPL_CLASS (klass);
  GdkWindowImplClass2 *impl_class = G_OBJECT_CLASS (klass);
  
  GdkWindowImplClass2 *impl_class;

  impl_class = GDK_WINDOW_IMPL_GET_CLASS (object_class);

  object_class->finalize = gdk_window_impl_broadway_finalize;

  impl_class->ref_cairo_surface = wine_window_broadway_ref_cairo_surface;
  impl_class->show = wine_window_broadway_show;
  impl_class->hide = wine_window_broadway_hide;
  impl_class->withdraw = wine_window_broadway_withdraw;
  impl_class->set_events = wine_window_broadway_set_events;
  impl_class->get_events = wine_window_broadway_get_events;
  impl_class->raise = wine_window_broadway_raise;
  impl_class->lower = wine_window_broadway_lower;
  impl_class->restack_under = wine_window_broadway_restack_under;
  impl_class->restack_toplevel = wine_window_broadway_restack_toplevel;
  impl_class->move_resize = wine_window_broadway_move_resize;
  impl_class->set_background = wine_window_broadway_set_background;
  impl_class->reparent = wine_window_broadway_reparent;
  impl_class->set_device_cursor = wine_window_broadway_set_device_cursor;
  impl_class->get_geometry = wine_window_broadway_get_geometry;
  impl_class->get_root_coords = wine_window_broadway_get_root_coords;
  impl_class->get_device_state = wine_window_broadway_get_device_state;
  impl_class->shape_combine_region = wine_window_broadway_shape_combine_region;
  impl_class->input_shape_combine_region = wine_window_broadway_input_shape_combine_region;
  impl_class->destroy = wine_broadway_window_destroy;
  impl_class->destroy_foreign = wine_broadway_window_destroy_foreign;
  impl_class->get_shape = wine_broadway_window_get_shape;
  impl_class->get_input_shape = wine_broadway_window_get_input_shape;
  impl_class->end_paint = wine_broadway_window_end_paint;
  impl_class->beep = wine_broadway_window_beep;

  impl_class->focus = wine_broadway_window_focus;
  impl_class->set_type_hint = wine_broadway_window_set_type_hint;
  impl_class->get_type_hint = wine_broadway_window_get_type_hint;
  impl_class->set_modal_hint = wine_broadway_window_set_modal_hint;
  impl_class->set_skip_taskbar_hint = wine_broadway_window_set_skip_taskbar_hint;
  impl_class->set_skip_pager_hint = wine_broadway_window_set_skip_pager_hint;
  impl_class->set_urgency_hint = wine_broadway_window_set_urgency_hint;
  impl_class->set_geometry_hints = wine_broadway_window_set_geometry_hints;
  impl_class->set_title = wine_broadway_window_set_title;
  impl_class->set_role = wine_broadway_window_set_role;
  impl_class->set_startup_id = wine_broadway_window_set_startup_id;
  impl_class->set_transient_for = wine_broadway_window_set_transient_for;
  impl_class->get_frame_extents = wine_broadway_window_get_frame_extents;
  impl_class->set_override_redirect = wine_broadway_window_set_override_redirect;
  impl_class->set_accept_focus = wine_broadway_window_set_accept_focus;
  impl_class->set_focus_on_map = wine_broadway_window_set_focus_on_map;
  impl_class->set_icon_list = wine_broadway_window_set_icon_list;
  impl_class->set_icon_name = wine_broadway_window_set_icon_name;
  impl_class->iconify = wine_broadway_window_iconify;
  impl_class->deiconify = wine_broadway_window_deiconify;
  impl_class->stick = wine_broadway_window_stick;
  impl_class->unstick = wine_broadway_window_unstick;
  impl_class->maximize = wine_broadway_window_maximize;
  impl_class->unmaximize = wine_broadway_window_unmaximize;
  impl_class->fullscreen = wine_broadway_window_fullscreen;
  impl_class->unfullscreen = wine_broadway_window_unfullscreen;
  impl_class->set_keep_above = wine_broadway_window_set_keep_above;
  impl_class->set_keep_below = wine_broadway_window_set_keep_below;
  impl_class->get_group = wine_broadway_window_get_group;
  impl_class->set_group = wine_broadway_window_set_group;
  impl_class->set_decorations = wine_broadway_window_set_decorations;
  impl_class->get_decorations = wine_broadway_window_get_decorations;
  impl_class->set_functions = wine_broadway_window_set_functions;
  impl_class->begin_resize_drag = wine_broadway_window_begin_resize_drag;
  impl_class->begin_move_drag = wine_broadway_window_begin_move_drag;
  impl_class->set_opacity = wine_broadway_window_set_opacity;
  impl_class->set_composited = wine_broadway_window_set_composited;
  impl_class->destroy_notify = wine_broadway_window_destroy_notify;
  impl_class->register_dnd = wine_broadway_window_register_dnd;
  impl_class->drag_begin = wine_broadway_window_drag_begin;
  impl_class->sync_rendering = wine_broadway_window_sync_rendering;
  impl_class->simulate_key = wine_broadway_window_simulate_key;
  impl_class->simulate_button = wine_broadway_window_simulate_button;
  impl_class->get_property = wine_broadway_window_get_property;
  impl_class->change_property = wine_broadway_window_change_property;
  impl_class->delete_property = wine_broadway_window_delete_property;
  impl_class->get_drag_protocol = wine_broadway_window_get_drag_protocol;
}
#endif

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
  GdkScreen * screen;
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

   wine_broadway_screen_init_root_window(broadway_display->default_screen);
}

void paint_broadway_window_surface(cairo_t *cr) {
  /* Surface for each rendering of the PNG file, in effect, ever pass becomes a frame */
  cairo_surface_t* surface =  cairo_image_surface_create_from_png("moving.png");

  cairo_set_source_surface (cr,surface, 0, 0);

  cairo_paint (cr);

  //cairo_t *cr = cairo_create (surface);
  //cairo_status_t retm1;

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
    wine_broadway_window_class_init(broadway_window_class);

    struct GdkBroadwayWindow *broadway_window;
    wine_broadway_window_init(broadway_window);

    id = wine_broadway_server_new_window(server,0,0,WINDOW1_WIDTH,WINDOW1_HEIGHT,0);
    printf("wine_broadway_server_new_window %d window created\n",id);

    visable = wine_broadway_server_window_show (server, id);

    if(visable == TRUE)
      {
	 printf("wine_broadway_server_window_show is true and function did something\n");
      }
    else
      {
	 printf("wine_broadway_server_window_show is somehow false\n");
      }

    surface = wine_broadway_server_create_surface(WINDOW1_WIDTH,WINDOW1_HEIGHT); 
    printf("wine_broadway_server_create_surface did something\n");

    paint_broadway_window_surface(surface);

    wine_broadway_server_window_update (server, id, surface);
    printf("wine_broadway_server_window_update\n");

    wine_broadway_server_window_show (server, id);

    if(visable == TRUE)
      {printf("wine_broadway_server_window_show - Part 2 is true and function did something\n");}
    else
      {printf("wine_broadway_server_window_show - Part 2 is somehow false\n");}
}

void main()
{
   //gtk_init();

   struct GdkBroadwayDisplay *broadway_display;

   // Initialze the Window class ourselves as we can't use all of the GDK/GTK functionality
   struct GdkWindowImplClass2 *klass;

   init_broadway_connection();
   test_broadway_display_open(display);
   gdk_window_impl_broadway_class_init (klass);

   for (;;)
   {
     // FIXME: Add an arg to switch windows
     test_broadway_window(broadway_display);
     sleep(2);
   }
}

#if 0
/* We are so not here yet */
void test_kbd_and_mouse()
{
  //broadway_device_manager = wine_broadway_device_manager_new ((char) display);
  //printf("Broadway Device Manager Initialized: %d\n", (const char) broadway_device_manager);
}
#endif
