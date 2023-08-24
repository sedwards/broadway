int 
cairo_check_event(cairo_surface_t *sfc, int block) {   

   char keybuf[8];
   KeySym key;
   XEvent e;

   for (;;) {
      if (block || XPending(cairo_xlib_surface_get_display(sfc)))
         XNextEvent(cairo_xlib_surface_get_display(sfc), &e);
      else 
         return 0;

      switch (e.type) {
         case ButtonPress:
            return -e.xbutton.button;
         case KeyPress:
            XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
            return key;
         default:
            fprintf(stderr, "Dropping unhandled XEevent.type = %d.\n", e.type);
      }
   }
}

