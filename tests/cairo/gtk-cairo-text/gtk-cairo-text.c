/* Snitched from http://zetcode.com/gfx/cairo/cairobackends/ on 13 Jan 2014 */
#include <cairo.h>
#include <cairo-xlib.h>
#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;
  cairo_surface_t *surface;
  cairo_t *cr;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(window,          "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 90);
  gtk_window_set_title(GTK_WINDOW(window), "GTK window");

  surface = cairo_xlib_surface_create(
      gtk_widget_get_display(window),   /* must be Display *, not struct GdkDisplay * */
      window,                           /* must be Drawable (no star) */
      gtk_widget_get_visual(window),    /* must be Visual * */
      gtk_widget_get_width(window),     /* int */
      gtk_widget_get_height(window)     /* int */
      );
  cr = cairo_create(surface);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, 
     CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 40.0);

  cairo_move_to(cr, 10.0, 50.0);
  cairo_show_text(cr, "Discipline is power.");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}

