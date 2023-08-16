/* GDK - The GIMP Drawing Kit
 * gdkdisplay-broadway.c
 * 
 * Copyright 2001 Sun Microsystems Inc.
 * Copyright (C) 2004 Nokia Corporation
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

#include "gdkdisplay-broadway.h"

#include "gdkdisplay.h"
#include "gdkeventsource.h"
#include "gdkscreen-broadway.h"
#include "gdkmonitor-broadway.h"
#include "gdkinternals.h"
#include "gdkdevicemanager-broadway.h"

#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>

static void   wine_broadway_display_dispose            (GObject            *object);
static void   wine_broadway_display_finalize           (GObject            *object);

#if 0
#define DEBUG_WEBSOCKETS 1
#endif

G_DEFINE_TYPE (GdkBroadwayDisplay, wine_broadway_display, GDK_TYPE_DISPLAY)

static void
wine_broadway_display_init (GdkBroadwayDisplay *display)
{
  display->id_ht = g_hash_table_new (NULL, NULL);

  display->monitor = g_object_new (GDK_TYPE_BROADWAY_MONITOR,
                                   "display", display,
                                   NULL);
  // FIXME?
  //gdk_monitor_set_manufacturer (display->monitor, "browser");
  //display->manufacturer = g_strdup ("browser");
  //gdk_monitor_set_model (display->monitor, "0");
  //display->monitor = g_strdup ("0");
}

static void
gdk_event_init (GdkDisplay *display)
{
  GdkBroadwayDisplay *broadway_display;

  broadway_display = GDK_BROADWAY_DISPLAY (display);
  broadway_display->event_source = wine_broadway_event_source_new (display);
}

GdkDisplay *
wine_broadway_display_open (const gchar *display_name)
{
  printf("\n inside wine_broadway_display_open");
  GdkDisplay *display;
  GdkBroadwayDisplay *broadway_display;
  GError *error = NULL;

  display = g_object_new (GDK_TYPE_BROADWAY_DISPLAY, NULL);
  printf("\n New object GDK_TYPE_BROADWAY_DISPLAY,");
  
  broadway_display = GDK_BROADWAY_DISPLAY (display);

  printf("\n open object");

  /* initialize the display's screens */
  broadway_display->screens = g_new (GdkScreen *, 1);
  broadway_display->screens[0] = wine_broadway_screen_new (display, 0);

  printf("\n open screens");
  /* We need to initialize events after we have the screen
   * structures in places
   */
  wine_broadway_screen_events_init (broadway_display->screens[0]);
  printf("\n screen events init");

  /*set the default screen */
  broadway_display->default_screen = broadway_display->screens[0];
  printf("\n set default display");

  display->device_manager = wine_broadway_device_manager_new (display);
  printf("\n New device manager");

  gdk_event_init (display);
  printf("\n Process gdk events");

  wine_broadway_display_init_dnd (display);
  printf("\n display init dnd");

  wine_broadway_screen_setup (broadway_display->screens[0]);
  printf("\n screen setup");

  if (display_name == NULL)
    display_name = g_getenv ("BROADWAY_DISPLAY");

  broadway_display->server = wine_broadway_server_new (display_name, &error);
  if (broadway_display->server == NULL)
    {
      GDK_NOTE (MISC, g_message ("Unable to init Broadway server: %s\n", error->message));
      printf ("Unable to init Broadway server: %s\n", error->message);
      g_error_free (error);
      return NULL;
    }

  g_signal_emit_by_name (display, "opened");

  return display;
}

static const gchar *
wine_broadway_display_get_name (GdkDisplay *display)
{
  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);

  return (gchar *) "Broadway";
}

static GdkScreen *
wine_broadway_display_get_default_screen (GdkDisplay *display)
{
  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);

  return GDK_BROADWAY_DISPLAY (display)->default_screen;
}

static void
wine_broadway_display_beep (GdkDisplay *display)
{
  g_return_if_fail (GDK_IS_DISPLAY (display));
}

static void
wine_broadway_display_sync (GdkDisplay *display)
{
  GdkBroadwayDisplay *broadway_display = GDK_BROADWAY_DISPLAY (display);

  g_return_if_fail (GDK_IS_BROADWAY_DISPLAY (display));

  wine_broadway_server_sync (broadway_display->server);
}

static void
wine_broadway_display_flush (GdkDisplay *display)
{
  GdkBroadwayDisplay *broadway_display = GDK_BROADWAY_DISPLAY (display);

  g_return_if_fail (GDK_IS_BROADWAY_DISPLAY (display));

  wine_broadway_server_flush (broadway_display->server);
}

static gboolean
wine_broadway_display_has_pending (GdkDisplay *display)
{
  return FALSE;
}

static GdkWindow *
wine_broadway_display_get_default_group (GdkDisplay *display)
{
  g_return_val_if_fail (GDK_IS_DISPLAY (display), NULL);

  return NULL;
}

static void
wine_broadway_display_dispose (GObject *object)
{
  GdkBroadwayDisplay *broadway_display = GDK_BROADWAY_DISPLAY (object);

  gdk_window_destroy ((GdkWindow*)broadway_display->screens[0]);

  if (broadway_display->event_source)
    {
      g_source_destroy (broadway_display->event_source);
      g_source_unref (broadway_display->event_source);
      broadway_display->event_source = NULL;
    }

  G_OBJECT_CLASS (wine_broadway_display_parent_class)->dispose (object);
}

static void
wine_broadway_display_finalize (GObject *object)
{
  GdkBroadwayDisplay *broadway_display = GDK_BROADWAY_DISPLAY (object);

  /* Keymap */
  if (broadway_display->keymap)
    g_object_unref (broadway_display->keymap);

  wine_broadway_cursor_display_finalize (GDK_DISPLAY_OBJECT(broadway_display));

  /* Free all GdkScreens */
  g_object_unref (broadway_display->screens[0]);
  g_free (broadway_display->screens);

  g_object_unref (broadway_display->monitor);

  G_OBJECT_CLASS (wine_broadway_display_parent_class)->finalize (object);
}

static void
wine_broadway_display_notify_startup_complete (GdkDisplay  *display,
					      const gchar *startup_id)
{
}

static gboolean
wine_broadway_display_supports_selection_notification (GdkDisplay *display)
{
  return FALSE;
}

static gboolean
wine_broadway_display_request_selection_notification (GdkDisplay *display,
						     GdkAtom     selection)

{
    return FALSE;
}

static gboolean
wine_broadway_display_supports_clipboard_persistence (GdkDisplay *display)
{
  return FALSE;
}

static void
wine_broadway_display_store_clipboard (GdkDisplay    *display,
				      GdkWindow     *clipboard_window,
				      guint32        time_,
				      const GdkAtom *targets,
				      gint           n_targets)
{
}

static gboolean
wine_broadway_display_supports_shapes (GdkDisplay *display)
{
  return FALSE;
}

static gboolean
wine_broadway_display_supports_input_shapes (GdkDisplay *display)
{
  return FALSE;
}

static gboolean
wine_broadway_display_supports_composite (GdkDisplay *display)
{
  return FALSE;
}

static gulong
wine_broadway_display_get_next_serial (GdkDisplay *display)
{
  GdkBroadwayDisplay *broadway_display;
  broadway_display = GDK_BROADWAY_DISPLAY (display);

  return wine_broadway_server_get_next_serial (broadway_display->server);
}

void
wine_broadway_display_show_keyboard (GdkBroadwayDisplay *display)
{
  g_return_if_fail (GDK_IS_BROADWAY_DISPLAY (display));

  wine_broadway_server_set_show_keyboard (display->server, TRUE);
}

void
wine_broadway_display_hide_keyboard (GdkBroadwayDisplay *display)
{
  g_return_if_fail (GDK_IS_BROADWAY_DISPLAY (display));

  wine_broadway_server_set_show_keyboard (display->server, FALSE);
}

static int
wine_broadway_display_get_n_monitors (GdkDisplay *display)
{
  return 1;
}

static GdkMonitor *
wine_broadway_display_get_monitor (GdkDisplay *display,
                                  int         monitor_num)
{
  GdkBroadwayDisplay *broadway_display = GDK_BROADWAY_DISPLAY (display);

  if (monitor_num == 0)
    return broadway_display->monitor;

  return NULL;
}

static GdkMonitor *
wine_broadway_display_get_primary_monitor (GdkDisplay *display)
{
  GdkBroadwayDisplay *broadway_display = GDK_BROADWAY_DISPLAY (display);

  return broadway_display->monitor;
}

static void
wine_broadway_display_class_init (GdkBroadwayDisplayClass * class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  GdkDisplayClass *display_class = GDK_DISPLAY_CLASS (class);

  object_class->dispose = wine_broadway_display_dispose;
  object_class->finalize = wine_broadway_display_finalize;

  display_class->window_type = GDK_TYPE_BROADWAY_WINDOW;

  display_class->get_name = wine_broadway_display_get_name;
  display_class->get_default_screen = wine_broadway_display_get_default_screen;
  display_class->beep = wine_broadway_display_beep;
  display_class->sync = wine_broadway_display_sync;
  display_class->flush = wine_broadway_display_flush;
  display_class->has_pending = wine_broadway_display_has_pending;
  display_class->queue_events = wine_broadway_display_queue_events;
  display_class->get_default_group = wine_broadway_display_get_default_group;
  display_class->supports_selection_notification = wine_broadway_display_supports_selection_notification;
  display_class->request_selection_notification = wine_broadway_display_request_selection_notification;
  display_class->supports_clipboard_persistence = wine_broadway_display_supports_clipboard_persistence;
  display_class->store_clipboard = wine_broadway_display_store_clipboard;
  display_class->supports_shapes = wine_broadway_display_supports_shapes;
  display_class->supports_input_shapes = wine_broadway_display_supports_input_shapes;
  display_class->supports_composite = wine_broadway_display_supports_composite;
  display_class->get_cursor_for_type = wine_broadway_display_get_cursor_for_type;
  display_class->get_cursor_for_name = wine_broadway_display_get_cursor_for_name;
  display_class->get_cursor_for_surface = wine_broadway_display_get_cursor_for_surface;
  display_class->get_default_cursor_size = wine_broadway_display_get_default_cursor_size;
  display_class->get_maximal_cursor_size = wine_broadway_display_get_maximal_cursor_size;
  display_class->supports_cursor_alpha = wine_broadway_display_supports_cursor_alpha;
  display_class->supports_cursor_color = wine_broadway_display_supports_cursor_color;

  display_class->before_process_all_updates = wine_broadway_display_before_process_all_updates;
  display_class->after_process_all_updates = wine_broadway_display_after_process_all_updates;
  display_class->get_next_serial = wine_broadway_display_get_next_serial;
  display_class->notify_startup_complete = wine_broadway_display_notify_startup_complete;
  display_class->create_window_impl = wine_broadway_display_create_window_impl;
  display_class->get_keymap = wine_broadway_display_get_keymap;
  display_class->get_selection_owner = wine_broadway_display_get_selection_owner;
  display_class->set_selection_owner = wine_broadway_display_set_selection_owner;
  display_class->send_selection_notify = wine_broadway_display_send_selection_notify;
  display_class->get_selection_property = wine_broadway_display_get_selection_property;
  display_class->convert_selection = wine_broadway_display_convert_selection;
  display_class->text_property_to_utf8_list = wine_broadway_display_text_property_to_utf8_list;
  display_class->utf8_to_string_target = wine_broadway_display_utf8_to_string_target;

  display_class->get_n_monitors = wine_broadway_display_get_n_monitors;
  display_class->get_monitor = wine_broadway_display_get_monitor;
  display_class->get_primary_monitor = wine_broadway_display_get_primary_monitor;
}

