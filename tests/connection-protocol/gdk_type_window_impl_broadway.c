#include "common.h"

/* BroadwayServerClass * class -> ????? GdkWindowImplBroadwayClass I think */
/* BroadwayServer  *server -> GdkWindowImplBroadway *impl */
/* gdk_window_impl_broadway -> gdk_window_impl_broadway */

//static GType gdk_window_impl_broadway_get_type (void);

G_DEFINE_TYPE (GdkWindowImplBroadway, gdk_window_impl_broadway, G_TYPE_OBJECT)

static void
gdk_window_impl_broadway_init (GdkWindowImplBroadway *impl)
{
  impl->toplevel_window_type = -1;
  //impl->next_serial = 1;
}


static void
gdk_window_impl_broadway_finalize (GObject *object)
{
  G_OBJECT_CLASS (gdk_window_impl_broadway_parent_class)->finalize (object);
}

static void
gdk_window_impl_broadway_class_init (GdkWindowImplBroadwayClass * class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->finalize = gdk_window_impl_broadway_finalize;
}

gboolean
_gdk_window_impl_broadway_lookahead_event (GdkWindowImplBroadway *imp,
                                      const char         *types)
{
  return FALSE;
}

gulong
_gdk_window_impl_broadway_get_next_serial (GdkWindowImplBroadway *impl)
{
  return FALSE;
}

