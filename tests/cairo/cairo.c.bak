#include <cairo.h>
#include <gtk/gtk.h>

static void do_drawing(cairo_t *);

struct {
  int count;
  double coordx[100];
  double coordy[100];
} glob;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{
  do_drawing(cr);

  return FALSE;
}

static void do_drawing(cairo_t *cr)
{
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 0.5);

  int i, j;
  for (i = 0; i <= glob.count - 1; i++ ) {
      for (j = 0; j <= glob.count - 1; j++ ) {
          cairo_move_to(cr, glob.coordx[i], glob.coordy[i]);
          cairo_line_to(cr, glob.coordx[j], glob.coordy[j]);
      }
  }

  glob.count = 0;
  cairo_stroke(cr);    
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;
  
  glob.count = 0;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);
 
  do_drawing("drawing");
    
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300); 
  gtk_window_set_title(GTK_WINDOW(window), "Lines");

  gtk_widget_show_all(window);

  glob.coordx[glob.count] = 10;
  glob.coordy[glob.count++] = 10;

  glob.coordx[glob.count] = 100;
  glob.coordy[glob.count++] = 100;

  gtk_main();

  return 0;
}

