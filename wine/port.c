
	
static void
wine_broadway_window_clear_old_updated_area (GdkWindow *window)
{
  int i;

  for (i = 0; i < 2; i++)
    {
      if (window->old_updated_area[i])
        {
          cairo_region_destroy (window->old_updated_area[i]);
          window->old_updated_area[i] = NULL;
        }
    }
}

void
wine_broadway_window_update_size (GdkWindow *window)
{
  gdk_window_clear_old_updated_area (window);
  recompute_visible_regions (window, FALSE);
}

