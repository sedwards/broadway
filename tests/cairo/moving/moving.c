/*
 * Red Image or full background in a PNG image with basic shapes
 */

#include <cairo/cairo.h>
#include <math.h>

#define WIDTH 700
#define HEIGHT 500

int main(){
  // Initialize cairo.
  cairo_surface_t *surface =
    cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
                                WIDTH, HEIGHT);
  cairo_t *cr = cairo_create (surface);
	

int random1() {

  // Fill the entire surface with red.
  cairo_set_source_rgb(cr, 1, 0, 0);
  cairo_rectangle(cr, 0, 0, WIDTH, HEIGHT);
  cairo_fill(cr);
  
  // Make a green rectangle.
  cairo_rectangle(cr, 100, 100, 100, 100);
  cairo_set_source_rgb(cr, 0, 1, 0);
  cairo_fill(cr);

  // Put a yellow border on the rectangle.
  cairo_rectangle(cr, 100, 100, 100, 100);
  cairo_set_source_rgb(cr, 1, 1, 0);
  cairo_stroke(cr);

  // Draw a blue triangle.
  cairo_move_to(cr, 300, 100);
  cairo_line_to(cr, 400, 100);
  cairo_line_to(cr, 400, 200);
  cairo_close_path(cr);
  cairo_set_source_rgb(cr, 0, 0, 1);
  cairo_stroke(cr);

  // Draw a black circle.
  cairo_arc(cr, 150, 350, 50, 0, 2 * M_PI);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_stroke(cr);

  // Draw a thick magenta line.
  cairo_set_line_width(cr, 15);
  cairo_move_to(cr, 300, 350);
  cairo_rel_line_to(cr, 100, 0);
  cairo_set_source_rgb(cr, 0.54, 0, 0.54);
  cairo_stroke(cr);

  // Write some text in white, blending with the background.
  cairo_set_font_size(cr, 48);
  cairo_move_to(cr, 500, 150);
  cairo_set_source_rgba(cr, 1, 1, 1, 0.5);
  cairo_show_text(cr, "Cairo Frame Two");
  
  cairo_surface_write_to_png(surface, "cairo.png");
}


int random2() {
  // Fill the entire surface with red.
  cairo_set_source_rgb(cr, 1, 0, 0);
  cairo_rectangle(cr, 0, 0, WIDTH, HEIGHT);
  cairo_fill(cr);
  
  // Make a green rectangle.
  cairo_rectangle(cr, 100, 200, 100, 100);
  cairo_set_source_rgb(cr, 0, 2, 0);
  cairo_fill(cr);

  // Put a yellow border on the rectangle.
  cairo_rectangle(cr, 100, 200, 100, 100);
  cairo_set_source_rgb(cr, 3, 1, 0);
  cairo_stroke(cr);

  // Draw a blue triangle.
  cairo_move_to(cr, 400, 100);
  cairo_line_to(cr, 500, 100);
  cairo_line_to(cr, 500, 200);
  cairo_close_path(cr);
  cairo_set_source_rgb(cr, 0, 0, 1);
  cairo_stroke(cr);

  // Draw a black circle.
  cairo_arc(cr, 150, 350, 50, 0, 2 * M_PI);
  cairo_set_source_rgb(cr, 0, 3, 0);
  cairo_stroke(cr);

  // Draw a thick magenta line.
  cairo_set_line_width(cr, 15);
  cairo_move_to(cr, 300, 350);
  cairo_rel_line_to(cr, 100, 0);
  cairo_set_source_rgb(cr, 0.74, 0, 0.74);
  cairo_stroke(cr);

  // Write some text in white, blending with the background.
  cairo_set_font_size(cr, 48);
  cairo_move_to(cr, 500, 150);
  cairo_set_source_rgba(cr, 1, 1, 1, 0.5);
  cairo_show_text(cr, "Cairo Frame 2");
  
  cairo_surface_write_to_png(surface, "cairo.png");
}

/* write to stdout
 * #include <cstdio> // for stdout

Cairo::ErrorStatus my_write_func(unsigned char* data, unsigned int length)
{
  return length == std::fwrite(data, length, stdout) ? CAIRO_STATUS_SUCCESS : CAIRO_STATUS_WRITE_ERROR;
}
*/

