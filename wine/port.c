#include <gdk/gdk.h>
	
static void
wine_broadway_window_clear_old_updated_area (GdkWindow *window)
{
  printf("stub - wine_broadway_window_clear_old_updated_area\n");
}

void
wine_broadway_window_update_size (GdkWindow *window)
{
  printf("stub - wine_broadway_window_update_size\n");
}


void
_gdk_windowing_got_event (GdkDisplay *display,
                          GList      *event_link,
                          GdkEvent   *event,
                          gulong      serial)
{
  printf("stub - _gdk_windowing_got_event\n");
}

/*
 _gdk_device_add_slave'
 _gdk_display_device_grab_update'
 _gdk_display_end_device_grab'
 _gdk_display_get_next_serial'
 _gdk_event_emit'
 _gdk_event_queue_find_first'
 gdk_event_set_pointer_emulated'
 gdk_event_set_scancode'
 GDK_SEAT_DEFAULT'
 gdk_seat_default_add_slave'
 gdk_seat_default_new_for_master_pair'
 _gdk_window_clear_update_area'
 gdk_window_impl_get_type'
*/

void                gdk_display_add_seat              (GdkDisplay       *display,
                                                       GdkSeat          *seat)
{
  printf("stub - gdk_display_add_seat\n");
}

void   _gdk_event_emit               (GdkEvent   *event)
{
  printf("stub - _gdk_event_emit\n");
}

void     gdk_event_set_scancode        (GdkEvent *event,
                                        guint16 scancode)
{
  printf("stub - gdk_event_set_scancode\n");
}

void                _gdk_display_device_grab_update   (GdkDisplay *display,
                                                       GdkDevice  *device,
                                                       GdkDevice  *source_device,
                                                       gulong      current_serial)
{
  printf("stub - _gdk_display_device_grab_update\n");
}


