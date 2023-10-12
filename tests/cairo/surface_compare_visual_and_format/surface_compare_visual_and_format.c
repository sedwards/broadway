/* Vladimir Vukicevic reported that surfaces were being created with
 * mismatching Visuals and XRenderPictFormats.
 */
static cairo_bool_t
surface_compare_visual_and_format (cairo_surface_t *surface)
{
    Display *dpy;
    Visual *visual;
    XRenderPictFormat *format;

    dpy = cairo_xlib_surface_get_display (surface);

    visual = cairo_xlib_surface_get_visual (surface);
    if (visual == NULL)
        return TRUE;

    format = cairo_xlib_surface_get_xrender_format (surface);
    if (format == NULL)
        return TRUE;

    return format == XRenderFindVisualFormat (dpy, visual);

}
