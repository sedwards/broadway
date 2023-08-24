static cairo_test_status_t
test_cairo_xlib_surface_get_display (cairo_surface_t *surface)
{
    Display *display = cairo_xlib_surface_get_display (surface);
    return display == NULL || surface_has_type (surface, CAIRO_SURFACE_TYPE_XLIB) ? CAIRO_TEST_SUCCESS : CAIRO_TEST_ERROR;
}

