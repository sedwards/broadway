void 
cairo_close_x11_surface(cairo_surface_t *sfc) {
   Display *dsp = cairo_xlib_surface_get_display(sfc);

   cairo_surface_destroy(sfc);
   XCloseDisplay(dsp);
}

