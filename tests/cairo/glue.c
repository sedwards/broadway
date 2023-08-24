GdkWindow *clg_gdk_cairo_surface_get_window (cairo_surface_t *surface)
{
  /* If 'surface_info_key' had been public we would have had a
     portable way to find the GdkWindow of a Cairo surface. */
  
#ifdef GDK_WINDOWING_X11
  g_return_if_fail (cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_XLIB);
  
  Display* display = cairo_xlib_surface_get_display (surface);
  if (display) {
    Drawable window = cairo_xlib_surface_get_drawable (surface);
    if (window)
      return gdk_window_lookup_for_display (window, display);
  }

  return NULL;
#elif defined (G_OS_WIN32)
  HDC hdc = (HDC)cairo_win32_surface_get_dc (surface);
  if (hdc)
    return gdk_window_lookup ((GdkNativeWindow)hdc);
  else
    return NULL;
#else
  return NULL;
#endif
}
