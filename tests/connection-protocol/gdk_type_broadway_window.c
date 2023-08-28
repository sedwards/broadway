#include "common.h"

/* BroadwayServerClass * class -> ????? GdkWindowImplBroadwayClass I think */
/* BroadwayServer  *server -> GdkWindowImplBroadway *impl */
/* gdk_type_broadway_window -> gdk_type_broadway_window */

//static GType gdk_type_broadway_window_get_type (void);

G_DEFINE_TYPE (GdkBroadwayWindow, gdk_type_broadway_window, G_TYPE_OBJECT)

static void
gdk_type_broadway_window_init (GdkWindowImplBroadway *impl)
{
  impl->toplevel_window_type = -1;
  //impl->next_serial = 1;
}


static void
gdk_type_broadway_window_finalize (GObject *object)
{
  G_OBJECT_CLASS (gdk_type_broadway_window_parent_class)->finalize (object);
}

static void
gdk_type_broadway_window_class_init (GdkWindowImplBroadwayClass * class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->finalize = gdk_type_broadway_window_finalize;
}

gboolean
_gdk_type_broadway_window_lookahead_event (GdkWindowImplBroadway *imp,
                                      const char         *types)
{
  return FALSE;
}

gulong
_gdk_type_broadway_window_get_next_serial (GdkWindowImplBroadway *impl)
{
  return FALSE;
}

