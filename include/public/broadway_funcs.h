//GType 	gdk_broadway_device_get_type (void) G_GNUC_CONST;
//GType		gdk_broadway_visual_get_type (void);
//GType		gdk_broadway_display_get_type            (void);
//GType		gdk_broadway_screen_get_type (void);
//GType		gdk_broadway_cursor_get_type          (void);
//GType		gdk_broadway_monitor_get_type            (void) G_GNUC_CONST;

void           gdk_broadway_display_show_keyboard       (GdkBroadwayDisplay *display);
void           gdk_broadway_display_hide_keyboard       (GdkBroadwayDisplay *display);

GdkScreen * _gdk_broadway_screen_new      (GdkDisplay *display,

void _gdk_broadway_screen_setup           (GdkScreen *screen);
void _gdk_broadway_resync_windows (void);
void     _gdk_broadway_window_register_dnd (GdkWindow      *window);

GdkDragContext * _gdk_broadway_window_drag_begin (GdkWindow *window,

void     _gdk_broadway_window_translate         (GdkWindow *window,

gboolean _gdk_broadway_window_get_property (GdkWindow   *window,

void _gdk_broadway_window_change_property (GdkWindow    *window,

void _gdk_broadway_window_delete_property (GdkWindow *window,

gboolean _gdk_broadway_moveresize_handle_event   (GdkDisplay *display,

gboolean _gdk_broadway_moveresize_configure_done (GdkDisplay *display,

void     _gdk_broadway_selection_window_destroyed (GdkWindow *window);
void     _gdk_broadway_window_grab_check_destroy (GdkWindow *window);

void     _gdk_broadway_window_grab_check_unmap (GdkWindow *window,

gint _gdk_broadway_get_group_for_state (GdkDisplay      *display,

void		_gdk_broadway_screen_events_init   (GdkScreen *screen);
GdkVisual	*_gdk_broadway_screen_get_system_visual (GdkScreen * screen);
gint		_gdk_broadway_screen_visual_get_best_depth (GdkScreen * screen);
GdkVisualType	_gdk_broadway_screen_visual_get_best_type (GdkScreen * screen);
GdkVisual	*_gdk_broadway_screen_get_system_visual (GdkScreen * screen);
GdkVisual	*_gdk_broadway_screen_visual_get_best (GdkScreen * screen);

GdkVisual *_gdk_broadway_screen_visual_get_best_with_depth (GdkScreen * screen,

GdkVisual *_gdk_broadway_screen_visual_get_best_with_type (GdkScreen * screen,

GdkVisual *_gdk_broadway_screen_visual_get_best_with_both (GdkScreen * screen,

void _gdk_broadway_screen_query_depths  (GdkScreen * screen,

void _gdk_broadway_screen_query_visual_types (GdkScreen * screen,

GList *_gdk_broadway_screen_list_visuals (GdkScreen *screen);

void _gdk_broadway_screen_size_changed (GdkScreen *screen, 

void _gdk_broadway_events_got_input      (BroadwayInputMsg *message);
void _gdk_broadway_screen_init_root_window (GdkScreen *screen);
void _gdk_broadway_screen_init_visuals (GdkScreen *screen);
void _gdk_broadway_display_init_dnd (GdkDisplay *display);
GdkDisplay * _gdk_broadway_display_open (const gchar *display_name);
void _gdk_broadway_display_queue_events (GdkDisplay *display);

GdkDragProtocol _gdk_broadway_window_get_drag_protocol (GdkWindow *window,

GdkCursor*_gdk_broadway_display_get_cursor_for_type (GdkDisplay    *display,

GdkCursor*_gdk_broadway_display_get_cursor_for_name (GdkDisplay  *display,

GdkCursor *_gdk_broadway_display_get_cursor_for_surface (GdkDisplay *display,

gboolean _gdk_broadway_display_supports_cursor_alpha (GdkDisplay *display);
gboolean _gdk_broadway_display_supports_cursor_color (GdkDisplay *display);

void _gdk_broadway_display_get_default_cursor_size (GdkDisplay *display,

void _gdk_broadway_display_get_maximal_cursor_size (GdkDisplay *display,

void       _gdk_broadway_display_before_process_all_updates (GdkDisplay *display);
void       _gdk_broadway_display_after_process_all_updates  (GdkDisplay *display);

void       _gdk_broadway_display_create_window_impl     (GdkDisplay    *display,

gboolean _gdk_broadway_display_set_selection_owner (GdkDisplay *display,

GdkWindow * _gdk_broadway_display_get_selection_owner (GdkDisplay *display,

gint _gdk_broadway_display_get_selection_property (GdkDisplay *display,

void _gdk_broadway_display_send_selection_notify (GdkDisplay       *display,

void _gdk_broadway_display_convert_selection (GdkDisplay *display,

gint _gdk_broadway_display_text_property_to_utf8_list (GdkDisplay    *display,

gchar *_gdk_broadway_display_utf8_to_string_target (GdkDisplay  *display,

GdkKeymap* _gdk_broadway_display_get_keymap (GdkDisplay *display);
void _gdk_broadway_display_consume_all_input (GdkDisplay *display);

BroadwayInputMsg * _gdk_broadway_display_block_for_input (GdkDisplay *display,

void     _gdk_broadway_window_sync_rendering    (GdkWindow       *window);

gboolean _gdk_broadway_window_simulate_key      (GdkWindow       *window,

gboolean _gdk_broadway_window_simulate_button   (GdkWindow       *window,

void _gdk_broadway_window_resize_surface        (GdkWindow *window);
void _gdk_broadway_cursor_update_theme (GdkCursor *cursor);
void _gdk_broadway_cursor_display_finalize (GdkDisplay *display);

GType    gdk_broadway_window_get_type          (void);

guint32  gdk_broadway_get_last_seen_time (GdkWindow       *window);

GSource *          _gdk_broadway_event_source_new            (GdkDisplay *display);

GType               gdk_broadway_device_manager_get_type (void) G_GNUC_CONST;

GdkDeviceManager  *_gdk_broadway_device_manager_new (GdkDisplay *display);

GdkBroadwayServer *_gdk_broadway_server_new                      (const char         *display,

void               _gdk_broadway_server_flush                    (GdkBroadwayServer  *server);
void               _gdk_broadway_server_sync                     (GdkBroadwayServer  *server);
gulong             _gdk_broadway_server_get_next_serial          (GdkBroadwayServer  *server);
guint32            _gdk_broadway_server_get_last_seen_time       (GdkBroadwayServer  *server);

gboolean           _gdk_broadway_server_lookahead_event          (GdkBroadwayServer  *server,

void               _gdk_broadway_server_query_mouse              (GdkBroadwayServer  *server,

GdkGrabStatus      _gdk_broadway_server_grab_pointer             (GdkBroadwayServer  *server,

guint32            _gdk_broadway_server_ungrab_pointer           (GdkBroadwayServer  *server,

gint32             _gdk_broadway_server_get_mouse_toplevel       (GdkBroadwayServer  *server);

guint32            _gdk_broadway_server_new_window               (GdkBroadwayServer  *server,

void               _gdk_broadway_server_destroy_window           (GdkBroadwayServer  *server,

gboolean           _gdk_broadway_server_window_show              (GdkBroadwayServer  *server,

gboolean           _gdk_broadway_server_window_hide              (GdkBroadwayServer  *server,

void               _gdk_broadway_server_window_focus             (GdkBroadwayServer  *server,

void               _gdk_broadway_server_window_set_transient_for (GdkBroadwayServer  *server,

void               _gdk_broadway_server_set_show_keyboard        (GdkBroadwayServer  *server,

gboolean           _gdk_broadway_server_window_translate         (GdkBroadwayServer  *server,

cairo_surface_t   *_gdk_broadway_server_create_surface           (int                 width,

void               _gdk_broadway_server_window_update            (GdkBroadwayServer  *server,

gboolean           _gdk_broadway_server_window_move_resize       (GdkBroadwayServer  *server,
