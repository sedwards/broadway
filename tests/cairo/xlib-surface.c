static cairo_surface_t *
_create_temp_xlib_surface (cairo_t *cr, Display *dpy, int width, int height,
                           cairo_xlib_drawing_support_t capabilities)
{
    /* base the temp surface on the *screen* surface, not any intermediate
     * group surface, because the screen surface is more likely to have
     * characteristics that the xlib-using code is likely to be happy with */
    cairo_surface_t *target = cairo_get_target (cr);
    Drawable target_drawable = cairo_xlib_surface_get_drawable (target);

    int screen_index = DefaultScreen (dpy);
    Drawable drawable = RootWindow (dpy, screen_index);
    Screen *screen = DefaultScreenOfDisplay (dpy);
    Visual *visual = DefaultVisual (dpy, screen_index);
    int depth = DefaultDepth (dpy, screen_index);

    Pixmap pixmap;
    cairo_surface_t *result;
    pixmap_free_struct *pfs;
    
    /* make the temporary surface match target_drawable to the extent supported
       by the native rendering code */
    if (target_drawable) {
        Screen *target_screen = cairo_xlib_surface_get_screen (target);
        Visual *target_visual = cairo_xlib_surface_get_visual (target);
        if ((target_screen == screen ||
             (capabilities & CAIRO_XLIB_DRAWING_SUPPORTS_ALTERNATE_SCREEN)) &&
            target_visual &&
            (target_visual == DefaultVisualOfScreen (target_screen) ||
             (capabilities & CAIRO_XLIB_DRAWING_SUPPORTS_NONDEFAULT_VISUAL))) {
            drawable = target_drawable;
            dpy = cairo_xlib_surface_get_display (target);
            visual = target_visual;
            depth = cairo_xlib_surface_get_depth (target);
        }
    }
    
    pfs = malloc (sizeof(pixmap_free_struct));
    if (pfs == NULL)
        return NULL;
        
    pixmap = XCreatePixmap (dpy, drawable, width, height, depth);
    if (!pixmap) {
        free (pfs);
        return NULL;
    }
    pfs->dpy = dpy;
    pfs->pixmap = pixmap;
  
    result = cairo_xlib_surface_create (dpy, pixmap, visual, width, height);
    if (cairo_surface_status (result) != CAIRO_STATUS_SUCCESS) {
        pixmap_free_func (pfs);
        return NULL;
    }
    
    cairo_surface_set_user_data (result, &pixmap_free_key, pfs, pixmap_free_func);
    return result;
}
