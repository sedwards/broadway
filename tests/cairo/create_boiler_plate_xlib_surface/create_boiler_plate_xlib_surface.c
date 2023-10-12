static cairo_surface_t *
_cairo_boilerplate_xlib_create_similar (cairo_surface_t		*other,
					cairo_content_t		 content,
					int			 width,
					int			 height)
{
    XRenderPictFormat *xrender_format;
    uint32_t format;
    struct similar *similar;
    cairo_surface_t *surface;

    similar = malloc (sizeof (*similar));
    similar->dpy = cairo_xlib_surface_get_display (other);

    switch (content) {
    default:
    case CAIRO_CONTENT_COLOR_ALPHA: format = PictStandardARGB32; break;
    case CAIRO_CONTENT_COLOR: format = PictStandardRGB24; break;
    case CAIRO_CONTENT_ALPHA: format = PictStandardA8; break;
    }

    xrender_format = XRenderFindStandardFormat (similar->dpy, format);
    similar->pixmap = XCreatePixmap (similar->dpy,
				     DefaultRootWindow (similar->dpy),
				     width, height,
				     xrender_format->depth);

    surface =
	    cairo_xlib_surface_create_with_xrender_format (similar->dpy,
							   similar->pixmap,
							   DefaultScreenOfDisplay (similar->dpy),
							   xrender_format,
							   width, height);

    cairo_surface_set_user_data (surface, &key, similar, _destroy_similar);

    return surface;
}

