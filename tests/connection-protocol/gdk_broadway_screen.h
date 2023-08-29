/*
 * gdkscreen-broadway.h
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

#ifndef __GDK_GDK_BROADWAY_SCREEN_H__
#define __GDK_GDK_BROADWAY_SCREEN_H__

G_BEGIN_DECLS

typedef struct _GdkBroadwayScreen GdkBroadwayScreen;
typedef struct _GdkBroadwayScreenClass GdkBroadwayScreenClass;

#define BROADWAY_TYPE_SCREEN              (broadway_screen_get_type())
#define BROADWAY_SCREEN(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), BROADWAY_TYPE_SCREEN, BroadwayScreen))
#define BROADWAY_SCREEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BROADWAY_TYPE_SCREEN, BroadwayScreenClass))
#define BROADWAY_IS_SCREEN(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), BROADWAY_TYPE_SCREEN))
#define BROADWAY_IS_SCREEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BROADWAY_TYPE_SCREEN))
#define BROADWAY_SCREEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BROADWAY_TYPE_SCREEN, BroadwayScreenClass))

typedef struct _GdkBroadwayMonitor GdkBroadwayMonitor;

typedef struct _GdkScreenClass GdkScreenClass;

struct _GdkScreen
{
  GObject parent_instance;

  cairo_font_options_t *font_options;
  gdouble resolution; /* pixels/points scale factor for fonts */
  guint resolution_set : 1; /* resolution set through public API */
  guint closed : 1;
};

struct _GdkScreenClass
{
  GObjectClass parent_class;
};

struct _GdkBroadwayScreen
{
  GdkScreen parent_instance;

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

struct _GdkBroadwayScreenClass
{
  GdkScreenClass parent_class;

  //void (* window_manager_changed) (GdkBroadwayScreen *screen);
};

//GType       broadway_screen_get_type (void){};

//GdkScreen * _gdk_broadway_screen_new      (GdkDisplay *display,
//					   gint	  screen_number);
//void _gdk_broadway_screen_setup           (GdkScreen *screen);

G_END_DECLS

#endif /* __GDK_BROADWAY_SCREEN_H__ */
