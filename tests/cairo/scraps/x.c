// Respond properly to events.
int
check_x_event(cairo_surface_t *sfc, int *position, int block)
{
    XEvent event;

    for (;;)
    {
        // If there is nothing in the event cue, XNextEvent will block the program.
        // If we want to block, then we can.
        if (block || XPending(cairo_xlib_surface_get_display(sfc)))
            XNextEvent(cairo_xlib_surface_get_display(sfc), &event);
        else
            return 0;

        // Check it out.
        switch (event.type)
        {
            // This event shouldn't really happen that much (we should always be exposed).
            case Expose:
                return -3053; // 3xp053
            // -event.xbutton.button -- less likely to be taken?
            case ButtonPress:
                //fprintf(stderr, "the button is: %d\n", -event.xbutton.button);
                *position = event.xbutton.y;
                return -event.xbutton.button;
            case KeyPress:
                return event.xkey.keycode;
            // This event is a bit weird: https://lists.cairographics.org/archives/cairo/2014-August/025534.html
            case 65:
                break;
            // We don't know the event, drop.
            default:
                //fprintf(stderr, "Dropping unhandled XEvent.type = %d.\n", event.type);
                break;
        }
    }
}

