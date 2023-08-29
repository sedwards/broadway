#include "common.h"

G_DEFINE_TYPE (GdkBroadwayScreen, broadway_screen, G_TYPE_OBJECT)

static void
broadway_screen_init (GdkBroadwayScreen *screen)
{
  screen->width = 1024;
  screen->height = 768;
}

static void
broadway_screen_finalize (GObject *object)
{
  G_OBJECT_CLASS (broadway_screen_parent_class)->finalize (object);
}

static void
broadway_screen_class_init (GdkBroadwayScreenClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  //GdkScreenClass *screen_class = GDK_SCREEN_CLASS (klass);

//  object_class->dispose = gdk_broadway_screen_dispose;
  object_class->finalize = broadway_screen_finalize;
#if 0
  screen_class->get_display = gdk_broadway_screen_get_display;
  screen_class->get_width = gdk_broadway_screen_get_width;
  screen_class->get_height = gdk_broadway_screen_get_height;
#endif
}	
