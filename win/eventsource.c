/* GDK - The GIMP Drawing Kit
 * Copyright (C) 2009 Carlos Garnacho <carlosg@gnome.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <glib.h>
#include <glib-object.h>
#include <cairo.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "port.h"
#include "broadway-protocol.h"


//static gboolean gdk_event_source_prepare  (GSource     *source,
  //                                         gint        *timeout);
//static gboolean gdk_event_source_check    (GSource     *source);
//static gboolean gdk_event_source_dispatch (GSource     *source,
  //                                         GSourceFunc  callback,
  //                                         gpointer     user_data);
//static void     gdk_event_source_finalize (GSource     *source);

#define HAS_FOCUS(toplevel)                           \
  ((toplevel)->has_focus || (toplevel)->has_pointer_focus)

struct _GdkEventSource
{
  GSource source;

  //GdkDisplay *display;
  GPollFD event_poll_fd;
};

typedef struct _GdkEventSource GdkEventSource;

//static GSourceFuncs event_funcs = {
  //gdk_event_source_prepare,
  //gdk_event_source_check,
  //gdk_event_source_dispatch,
  //gdk_event_source_finalize
//};

//static GList *event_sources = NULL;
#if 0
static gboolean
gdk_event_source_prepare (GSource *source,
                          gint    *timeout)
{
  //GdkDisplay *display = ((GdkEventSource*) source)->display;
  gboolean retval;
#if 0
  gdk_threads_enter ();

  *timeout = -1;

  retval = (_win_event_queue_find_first (display) != NULL);

  gdk_threads_leave ();
#endif
  return retval;
}

static gboolean
gdk_event_source_check (GSource *source)
{
  //GdkEventSource *event_source = (GdkEventSource*) source;
  gboolean retval;

#if 0
  gdk_threads_enter ();

  if (event_source->display->event_pause_count > 0 ||
      event_source->event_poll_fd.revents & G_IO_IN)
    retval = (_win_event_queue_find_first (event_source->display) != NULL);
  else
    retval = FALSE;

  gdk_threads_leave ();
#endif

  return retval;
}
#endif
void
_win_broadway_events_got_input (BroadwayInputMsg *message)
{
#if 0
  GList *node;
  GSList *list, *d;

  display = NULL;

  list = gdk_display_manager_list_displays (gdk_display_manager_get ());
  for (d = list; d; d = d->next)
    {
      if (GDK_IS_BROADWAY_DISPLAY (d->data))
        {
          display = d->data;
          break;
        }
    }
  g_slist_free (list);

  g_assert (display != NULL);
#endif

  switch (message->base.type) {
  case BROADWAY_EVENT_ENTER:
  case BROADWAY_EVENT_LEAVE:
  case BROADWAY_EVENT_POINTER_MOVE:
  case BROADWAY_EVENT_BUTTON_PRESS:
  case BROADWAY_EVENT_BUTTON_RELEASE:
  case BROADWAY_EVENT_SCROLL:
  case BROADWAY_EVENT_TOUCH:
  case BROADWAY_EVENT_KEY_PRESS:
  case BROADWAY_EVENT_KEY_RELEASE:
  case BROADWAY_EVENT_GRAB_NOTIFY:
  case BROADWAY_EVENT_UNGRAB_NOTIFY:
  case BROADWAY_EVENT_CONFIGURE_NOTIFY:
  case BROADWAY_EVENT_DELETE_NOTIFY:
  case BROADWAY_EVENT_SCREEN_SIZE_CHANGED:
  case BROADWAY_EVENT_FOCUS:
    break;

  default:
    g_printerr ("_win_broadway_events_got_input - Unknown input command %c\n", message->base.type);
    break;
  }
}

void
_win_broadway_display_queue_events (void) //GdkDisplay *display)
{
}

#if 0
static gboolean
gdk_event_source_dispatch (GSource     *source,
                           GSourceFunc  callback,
                           gpointer     user_data)
{
#if 0
  //GdkDisplay *display = ((GdkEventSource*) source)->display;
  GdkEvent *event;

  gdk_threads_enter ();

  event = gdk_display_get_event (display);

  if (event)
    {
      _win_event_emit (event);

      gdk_event_free (event);
    }

  gdk_threads_leave ();
#endif
  return TRUE;
}

static void
gdk_event_source_finalize (GSource *source)
{
  GdkEventSource *event_source = (GdkEventSource *)source;

  event_sources = g_list_remove (event_sources, event_source);
}
#endif

GSource *
_win_broadway_event_source_new (void)
		//Gdk_Display *display)
{
  GSource *source = NULL;
#if 0
  GdkEventSource *event_source;
  char *name;

  source = g_source_new (&event_funcs, sizeof (GdkEventSource));
  name = g_strdup_printf ("GDK Broadway Event source (%s)",
			  gdk_display_get_name (display));
  g_source_set_name (source, name);
  g_free (name);
  event_source = (GdkEventSource *) source;
  event_source->display = display;

  g_source_set_priority (source, GDK_PRIORITY_EVENTS);
  g_source_set_can_recurse (source, TRUE);
  g_source_attach (source, NULL);

  event_sources = g_list_prepend (event_sources, source);
#endif
  return source;
}

