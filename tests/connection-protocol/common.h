#ifndef _COMMON_H
#define _COMMON_H

#include "config.h" 
 
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
 
#include <glib.h> 
#include <glib/gprintf.h> 
#include <glib-object.h> 
#include <gio/gunixsocketaddress.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <unistd.h> 
#include <sys/types.h> 
 
#include <broadway-protocol.h> 
#include <broadway-client.h> 
 
#include <cairo.h> 
 
#include <glib.h> 
#include <gtk/gtk.h> 
#include "gdk_structs.h" 

#if 0

#define G_OS_UNIX

#include <stdio.h>
#include <sys/mman.h>

#include <glib.h>
#include <glib-object.h>

#include <broadway-protocol.h>
#include <cairo.h>

#include <gio/gunixsocketaddress.h>

#include <gdk/gdk.h>

#include "gdk_structs.h"

#include <gtk/gtk.h>

#include <broadway-server.h>

#endif

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

typedef struct _BroadwayScreen BroadwayScreen;
struct _BroadwayScreen
{ 
  GObject parent_instance;
  //GdkScreen parent_instance;
  
  GdkDisplay *display;
  GdkWindow *root_window;
  
  int width;
  int height;
};

typedef struct _GdkWindowImplBroadway GdkWindowImplBroadway;

struct _GdkWindowImplBroadway
{
  GdkWindowImpl parent_instance;

  GdkWindow *wrapper;
  GdkScreen *screen;

  cairo_surface_t *surface;
  cairo_surface_t *last_surface;
  cairo_surface_t *ref_surface;

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

typedef struct _GdkBroadwayScreen GdkBroadwayScreen;

struct _GdkBroadwayScreen
{
  // GdkScreen parent_instance;

  GdkDisplay *display;
  GdkWindow *root_window;

  int width;
  int height;

  /* Visual Part */
  GdkVisual **visuals;
  gint nvisuals;
  GdkVisual *system_visual;
  GdkVisual *rgba_visual;
  gint available_depths[7];
  gint navailable_depths;
  GdkVisualType available_types[6];
  gint navailable_types;
};


static inline GType common_class_init()
{
  printf("Initialized common class\n");
}

#if 0
static void
broadway_req_class_init (BroadwayServerClass * class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->finalize = broadway_req_finalize;
}
#endif

GType common_class_init (void);
#define GDK_BROADWAY_SCREEN                   (common_class_init())
#define GDK_TYPE_BROADWAY_SERVER              (common_class_init())
#define GDK_TYPE_BROADWAY_DISPLAY             (common_class_init())
#define GDK_TYPE_BROADWAY_SCREEN              (common_class_init())
#define GDK_TYPE_BROADWAY_WINDOW              (common_class_init())
#define GDK_TYPE_WINDOW_IMPL_BROADWAY         (common_class_init())

BroadwayServer     *broadway_req_server_on_unix_socket_new       (char             *address,
							      GError          **error);

gboolean            broadway_req_server_has_client               (BroadwayServer   *server);
void                broadway_req_server_flush                    (BroadwayServer   *server);
void                broadway_req_server_sync                     (BroadwayServer   *server);
void                broadway_req_server_get_screen_size          (BroadwayServer   *server,
							      guint32          *width,
							      guint32          *height);
guint32             broadway_req_server_get_next_serial          (BroadwayServer   *server);
guint32             broadway_req_server_get_last_seen_time       (BroadwayServer   *server);
gboolean            broadway_req_server_lookahead_event          (BroadwayServer   *server,
							      const char       *types);
void                broadway_req_server_query_mouse              (BroadwayServer   *server,
							      guint32          *toplevel,
							      gint32           *root_x,
							      gint32           *root_y,
							      guint32          *mask);
guint32             broadway_req_server_grab_pointer             (BroadwayServer   *server,
							      gint              client_id,
							      gint              id,
							      gboolean          owner_events,
							      guint32           event_mask,
							      guint32           time_);
guint32             broadway_req_server_ungrab_pointer           (BroadwayServer   *server,
							      guint32           time_);
gint32              broadway_req_server_get_mouse_toplevel       (BroadwayServer   *server);
void                broadway_req_server_set_show_keyboard        (BroadwayServer   *server,
                                                              gboolean          show);

guint32             broadway_req_server_new_window               (BroadwayServer   *server,
							      int               x,
							      int               y,
							      int               width,
							      int               height,
							      gboolean          is_temp);

void                broadway_req_server_destroy_window           (BroadwayServer   *server,
							      gint              id);
gboolean            broadway_req_server_window_show              (BroadwayServer   *server,
							      gint              id);
gboolean            broadway_req_server_window_hide              (BroadwayServer   *server,
							      gint              id);
void                broadway_req_server_window_raise             (BroadwayServer   *server,
							      gint              id);
void                broadway_req_server_window_lower             (BroadwayServer   *server,
							      gint              id);
void                broadway_req_server_window_set_transient_for (BroadwayServer   *server,
							      gint              id,
							      gint              parent);
gboolean            broadway_req_server_window_translate         (BroadwayServer   *server,
							      gint              id,
							      cairo_region_t   *area,
							      gint              dx,
							      gint              dy);
cairo_surface_t   * broadway_req_server_create_surface           (int               width,
							      int               height);
void                broadway_req_server_window_update            (BroadwayServer   *server,
							      gint              id,
							      cairo_surface_t  *surface);
gboolean            broadway_req_server_window_move_resize       (BroadwayServer   *server,
							      gint              id,
							      gboolean          with_move,
							      int               x,
							      int               y,
							      int               width,
							      int               height);
void                broadway_req_server_focus_window             (BroadwayServer   *server,
                                                              gint              new_focused_window);
cairo_surface_t * broadway_req_server_open_surface (BroadwayServer *server,
						guint32 id,
						char *name,
						int width,
						int height);


BroadwayServer *
broadway_req_server_new (const char *display, GError **error);


void run_unit_tests(void);
#endif /* _COMMON_H */
