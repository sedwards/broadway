 /*
 * gdkscreen-broadway.c
 * 
 * Copyright 2001 Sun Microsystems Inc. 
 *
 * Erwann Chenede <erwann.chenede@sun.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "gdkscreen-broadway.h"

//#include "gdkscreen.h"
#include "gdkdisplay.h"
#include "gdkdisplay-broadway.h"

#include <glib.h>

#include <stdlib.h>
#include <string.h>

static void   wine_broadway_screen_dispose     (GObject *object);
static void   wine_broadway_screen_finalize    (GObject *object);

G_DEFINE_TYPE (GdkBroadwayScreen, wine_broadway_screen, GDK_TYPE_SCREEN)

static void
wine_broadway_screen_init (GdkBroadwayScreen *screen)
{
  screen->width = 1024;
  screen->height = 768;
}

static GdkDisplay *
wine_broadway_screen_get_display (GdkScreen *screen)
{
  return GDK_BROADWAY_SCREEN (screen)->display;
}

static gint
wine_broadway_screen_get_width (GdkScreen *screen)
{
  return GDK_BROADWAY_SCREEN (screen)->width;
}

static gint
wine_broadway_screen_get_height (GdkScreen *screen)
{
  return GDK_BROADWAY_SCREEN (screen)->height;
}

static gint
wine_broadway_screen_get_width_mm (GdkScreen *screen)
{
  return gdk_screen_get_width (screen) * 25.4 / 96;
}

static gint
wine_broadway_screen_get_height_mm (GdkScreen *screen)
{
  return gdk_screen_get_height (screen) * 25.4 / 96;
}

static gint
wine_broadway_screen_get_number (GdkScreen *screen)
{
  return 0;
}

static GdkWindow *
wine_broadway_screen_get_root_window (GdkScreen *screen)
{
  return GDK_BROADWAY_SCREEN (screen)->root_window;
}

void
wine_broadway_screen_size_changed (GdkScreen                       *screen,
                                   BroadwayInputScreenResizeNotify *msg)
{
  GdkBroadwayScreen *broadway_screen = GDK_BROADWAY_SCREEN (screen);
  GdkMonitor *monitor;
  gint width, height;
  GList *toplevels, *l;

  width = gdk_screen_get_width (screen);
  height = gdk_screen_get_height (screen);

  broadway_screen->width   = msg->width;
  broadway_screen->height  = msg->height;

  if (width == gdk_screen_get_width (screen) &&
      height == gdk_screen_get_height (screen))
    return;

  monitor = GDK_BROADWAY_DISPLAY (broadway_screen->display)->monitor;

  //gdk_monitor_set_size (monitor, msg->width, msg->height);
  gdk_monitor_set_physical_size (monitor, msg->width * 25.4 / 96, msg->height * 25.4 / 96);

  g_signal_emit_by_name (screen, "size-changed");
  toplevels = gdk_screen_get_toplevel_windows (screen);
  for (l = toplevels; l != NULL; l = l->next)
    {
      GdkWindow *toplevel = l->data;
      GdkWindowImplBroadway *toplevel_impl = GDK_WINDOW_IMPL_BROADWAY (toplevel->impl);

      if (toplevel_impl->maximized)
	gdk_window_move_resize (toplevel, 0, 0,
				gdk_screen_get_width (screen),
				gdk_screen_get_height (screen));
    }
}

static void
wine_broadway_screen_dispose (GObject *object)
{
  GdkBroadwayScreen *broadway_screen = GDK_BROADWAY_SCREEN (object);

  if (broadway_screen->root_window)
    //gdk_window_destroy (broadway_screen->root_window, TRUE);
    gdk_window_destroy (broadway_screen->root_window);

  G_OBJECT_CLASS (wine_broadway_screen_parent_class)->dispose (object);
}

static void
wine_broadway_screen_finalize (GObject *object)
{
  GdkBroadwayScreen *broadway_screen = GDK_BROADWAY_SCREEN (object);
  gint          i;

  if (broadway_screen->root_window)
    g_object_unref (broadway_screen->root_window);

  /* Visual Part */
  for (i = 0; i < broadway_screen->nvisuals; i++)
    g_object_unref (broadway_screen->visuals[i]);
  g_free (broadway_screen->visuals);

  G_OBJECT_CLASS (wine_broadway_screen_parent_class)->finalize (object);
}

static GdkVisual *
wine_broadway_screen_get_rgba_visual (GdkScreen *screen)
{
  GdkBroadwayScreen *broadway_screen = GDK_BROADWAY_SCREEN (screen);

  return broadway_screen->rgba_visual;
}

GdkScreen *
wine_broadway_screen_new (GdkDisplay *display,
			  gint	 screen_number)
{
  GdkScreen *screen;
  GdkBroadwayScreen *broadway_screen;

  screen = g_object_new (GDK_TYPE_BROADWAY_SCREEN, NULL);

  broadway_screen = GDK_BROADWAY_SCREEN (screen);
  broadway_screen->display = display;
  wine_broadway_screen_init_visuals (screen);
  wine_broadway_screen_init_root_window (screen);

  return screen;
}

/*
 * It is important that we first request the selection
 * notification, and then setup the initial state of
 * is_composited to avoid a race condition here.
 */
void
wine_broadway_screen_setup (GdkScreen *screen)
{
}

static gboolean
wine_broadway_screen_is_composited (GdkScreen *screen)
{
  return TRUE;
}


static gchar *
wine_broadway_screen_make_display_name (GdkScreen *screen)
{
  return g_strdup ("browser");
}

static GdkWindow *
wine_broadway_screen_get_active_window (GdkScreen *screen)
{
  return NULL;
}

static GList *
wine_broadway_screen_get_window_stack (GdkScreen *screen)
{
  return NULL;
}

static void
wine_broadway_screen_broadcast_client_message (GdkScreen *screen,
					      GdkEvent  *event)
{
}

static gboolean
wine_broadway_screen_get_setting (GdkScreen   *screen,
				 const gchar *name,
				 GValue      *value)
{
  return FALSE;
}

void
wine_broadway_screen_events_init (GdkScreen *screen)
{
}

static void
wine_broadway_screen_class_init (GdkBroadwayScreenClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GdkScreenClass *screen_class = GDK_SCREEN_CLASS (klass);

  object_class->dispose = wine_broadway_screen_dispose;
  object_class->finalize = wine_broadway_screen_finalize;

  screen_class->get_display = wine_broadway_screen_get_display;
  screen_class->get_width = wine_broadway_screen_get_width;
  screen_class->get_height = wine_broadway_screen_get_height;
  screen_class->get_width_mm = wine_broadway_screen_get_width_mm;
  screen_class->get_height_mm = wine_broadway_screen_get_height_mm;
  screen_class->get_number = wine_broadway_screen_get_number;
  screen_class->get_root_window = wine_broadway_screen_get_root_window;
  screen_class->is_composited = wine_broadway_screen_is_composited;
  screen_class->make_display_name = wine_broadway_screen_make_display_name;
  screen_class->get_active_window = wine_broadway_screen_get_active_window;
  screen_class->get_window_stack = wine_broadway_screen_get_window_stack;
  screen_class->broadcast_client_message = wine_broadway_screen_broadcast_client_message;
  screen_class->get_setting = wine_broadway_screen_get_setting;
  screen_class->get_rgba_visual = wine_broadway_screen_get_rgba_visual;
  screen_class->get_system_visual = wine_broadway_screen_get_system_visual;
  screen_class->visual_get_best_depth = wine_broadway_screen_visual_get_best_depth;
  screen_class->visual_get_best_type = wine_broadway_screen_visual_get_best_type;
  screen_class->visual_get_best = wine_broadway_screen_visual_get_best;
  screen_class->visual_get_best_with_depth = wine_broadway_screen_visual_get_best_with_depth;
  screen_class->visual_get_best_with_type = wine_broadway_screen_visual_get_best_with_type;
  screen_class->visual_get_best_with_both = wine_broadway_screen_visual_get_best_with_both;
  screen_class->query_depths = wine_broadway_screen_query_depths;
  screen_class->query_visual_types = wine_broadway_screen_query_visual_types;
  screen_class->list_visuals = wine_broadway_screen_list_visuals;
}

