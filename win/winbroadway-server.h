#ifndef __GDK_BROADWAY_SERVER__
#define __GDK_BROADWAY_SERVER__

//#include <sys/mman.h>

#include <glib-object.h>
#include <cairo.h>

//#include <win/wintypes.h>
#include "broadway-protocol.h"

typedef struct _WinBroadwayServer WinBroadwayServer;
typedef struct _WinBroadwayServerClass WinBroadwayServerClass;

#define GDK_TYPE_BROADWAY_SERVER              (win_broadway_server_get_type())
#define GDK_BROADWAY_SERVER(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_BROADWAY_SERVER, WinBroadwayServer))
#define GDK_BROADWAY_SERVER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_BROADWAY_SERVER, WinBroadwayServerClass))
#define GDK_IS_BROADWAY_SERVER(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_BROADWAY_SERVER))
#define GDK_IS_BROADWAY_SERVER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_BROADWAY_SERVER))
#define GDK_BROADWAY_SERVER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_BROADWAY_SERVER, WinBroadwayServerClass))

WinBroadwayServer *_win_broadway_server_new                      (const char         *display,
								  GError            **error);
void               _win_broadway_server_flush                    (WinBroadwayServer  *server);
void               _win_broadway_server_sync                     (WinBroadwayServer  *server);
gulong             _win_broadway_server_get_next_serial          (WinBroadwayServer  *server);
guint32            _win_broadway_server_get_last_seen_time       (WinBroadwayServer  *server);
gboolean           _win_broadway_server_lookahead_event          (WinBroadwayServer  *server,
								  const char         *types);
void               _win_broadway_server_query_mouse              (WinBroadwayServer  *server,
								  guint32            *toplevel,
								  gint32             *root_x,
								  gint32             *root_y,
								  guint32            *mask);
GrabStatus      _win_broadway_server_grab_pointer             (WinBroadwayServer  *server,
								  gint                id,
								  gboolean            owner_events,
								  guint32             event_mask,
								  guint32             time_);
guint32            _win_broadway_server_ungrab_pointer           (WinBroadwayServer  *server,
								  guint32             time_);
gint32             _win_broadway_server_get_mouse_toplevel       (WinBroadwayServer  *server);
guint32            _win_broadway_server_new_window               (WinBroadwayServer  *server,
								  int                 x,
								  int                 y,
								  int                 width,
								  int                 height,
								  gboolean            is_temp);
void               _win_broadway_server_destroy_window           (WinBroadwayServer  *server,
								  gint                id);
gboolean           _win_broadway_server_window_show              (WinBroadwayServer  *server,
								  gint                id);
gboolean           _win_broadway_server_window_hide              (WinBroadwayServer  *server,
								  gint                id);
void               _win_broadway_server_window_focus             (WinBroadwayServer  *server,
								  gint                id);
void               _win_broadway_server_window_set_transient_for (WinBroadwayServer  *server,
								  gint                id,
								  gint                parent);
void               _win_broadway_server_set_show_keyboard        (WinBroadwayServer  *server,
								  gboolean            show_keyboard);
gboolean           _win_broadway_server_window_translate         (WinBroadwayServer  *server,
								  gint                id,
								  cairo_region_t     *area,
								  gint                dx,
								  gint                dy);
cairo_surface_t   *_win_broadway_server_create_surface           (int                 width,
								  int                 height);
void               _win_broadway_server_window_update            (WinBroadwayServer  *server,
								  gint                id,
								  cairo_surface_t    *surface);
gboolean           _win_broadway_server_window_move_resize       (WinBroadwayServer  *server,
								  gint                id,
								  gboolean            with_move,
								  int                 x,
								  int                 y,
								  int                 width,
								  int                 height);

#endif /* __GDK_BROADWAY_SERVER__ */
