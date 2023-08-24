#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <time.h>

#include <time.h>

int random(int range){
    int num;
    num = rand() % range;
    return num;
}

int random1(cairo_t *cr);
int random2(cairo_t *cr);

cairo_status_t
write_png (cairo_surface_t *surface, const char *filename);

#if 0
int main(){
    int number = 0;
    int i;
    int middle = 500;

    // Seed random number generator
    srand(time(NULL));

    //for ( ;; ){
    for ( number=1;number<=1000;number++ ){
        i = random(1000);
        printf("%d\n",i);
	sleep(1);
	printf("We are on loop: %d\n", number);

	if(i >= middle)
	{
	   foo();
	}
	else
	{
	   bar();
	}
    }
    return 0;
}
#endif


//This function should give us a new x11 surface to draw on.
cairo_surface_t* create_x11_surface(Display *d, int x, int y)
{
    Drawable da;
    int screen;
    cairo_surface_t* sfc;

    screen = DefaultScreen(d);
    da = XCreateSimpleWindow(d, DefaultRootWindow(d), 0, 0, x, y, 0, 0, 0);
    XSelectInput(d, da, ButtonPressMask | KeyPressMask);
    XMapWindow(d, da);

    sfc = cairo_xlib_surface_create(d, da, DefaultVisual(d, screen), x, y);

    return sfc;
}

int main(int argc, char** argv)
{
    int number = 0;
    int i;
    int middle = 250;

    // Seed random number generator
    srand(time(NULL));

    Display *d = XOpenDisplay(NULL);
    if (d == NULL) {
        fprintf(stderr, "Failed to open display\n");
        return 1;
    }
    //create a new cairo surface in an x11 window as well as a cairo_t* to draw
    //on the x11 window with.
    cairo_surface_t* surface = create_x11_surface(d, 700, 500);
    cairo_t* cr = cairo_create(surface);

    char *text = argv[1];
    size_t text_len = 0;

    if (argc != 2)
        text = NULL;
    else
        text_len = strlen(text);

    // Clear the background
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    //draw some text
    cairo_select_font_face(cr, "Times", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 32.0);
    cairo_set_source_rgb(cr, 0, 0, 1.0);
    cairo_move_to(cr, 10.0, 25.0);

     // Make a green rectangle.
    cairo_rectangle(cr, 100, 100, 100, 100);
    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_fill(cr);

    //cairo_show_text(cr, text);
    cairo_show_text(cr, "Random Number is");

    cairo_surface_flush(surface);
    cairo_surface_mark_dirty (surface);

    XFlush(d);

    for (;;)
    {
	sleep(2);

	i = random(500);
        printf("%d\n",i);
        printf("We are on loop: %d\n", number);

        if(i >= middle)
        {
           random1(cr);
        }
        else
        {
           random2(cr);
	}
//	cairo_image_surface_create_from_png_stream
  //                             (cairo_read_func_t read_func,
    //                            void *closure);
    }

    // XXX: Lots of other stuff isn't properly destroyed here
    cairo_surface_destroy(surface);
    return 0;
}

/*
 *
 * cairo_surface_t *surface  = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 240, 80);
        cairo_t         *cr       = cairo_create (surface);

	cairo_t         *first_cr;
	cairo_t         *second_cr;
	cairo_t         *third_cr;
	cairo_surface_t *a;
	cairo_surface_t *b;
	cairo_surface_t *c;

	a = cairo_surface_create_similar (cairo_get_target (cr), CAIRO_CONTENT_COLOR_ALPHA, 240, 80);
	b = cairo_surface_create_similar (cairo_get_target (cr), CAIRO_CONTENT_COLOR_ALPHA, 240, 80);
	c = cairo_surface_create_similar (cairo_get_target (cr), CAIRO_CONTENT_COLOR_ALPHA, 240, 80);

	a = cairo_image_surface_create_from_png ("cairo-demo.png");
	b = cairo_image_surface_create_from_png ("box.png");
	c = cairo_image_surface_create_from_png ("base.png");

	first_cr  = cairo_create (a);
	second_cr = cairo_create (b);
	third_cr  = cairo_create (c);

	cairo_stroke (first_cr);
	cairo_stroke (second_cr);
	cairo_stroke (third_cr);

	// Pasting the Map on the base image
	cairo_set_operator       (third_cr, (cairo_operator_t)5);
	cairo_set_source_surface (third_cr, a, 220, 80);
	cairo_paint              (third_cr);

	cairo_set_source_surface (cr, c, 20, 20);
	cairo_paint(cr);

	cairo_surface_write_to_png (c, "different.png");

	// Pasting the Label on the base image
	cairo_set_operator       (third_cr, (cairo_operator_t)5);
	cairo_set_source_surface (third_cr, b, -20, 80);
	cairo_paint              (third_cr);

	cairo_set_source_surface (cr, c, 20, 20);
	cairo_paint(cr);

	cairo_surface_write_to_png (c, "different.png");
*/

