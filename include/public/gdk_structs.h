typedef enum
{
  GDK_SHIFT_MASK    = 1 << 0,
  GDK_LOCK_MASK     = 1 << 1,
  GDK_CONTROL_MASK  = 1 << 2,
  GDK_MOD1_MASK     = 1 << 3,
  GDK_MOD2_MASK     = 1 << 4,
  GDK_MOD3_MASK     = 1 << 5,
  GDK_MOD4_MASK     = 1 << 6,
  GDK_MOD5_MASK     = 1 << 7,
  GDK_BUTTON1_MASK  = 1 << 8,
  GDK_BUTTON2_MASK  = 1 << 9,
  GDK_BUTTON3_MASK  = 1 << 10,
  GDK_BUTTON4_MASK  = 1 << 11,
  GDK_BUTTON5_MASK  = 1 << 12,

  GDK_MODIFIER_RESERVED_13_MASK  = 1 << 13,
  GDK_MODIFIER_RESERVED_14_MASK  = 1 << 14,
  GDK_MODIFIER_RESERVED_15_MASK  = 1 << 15,
  GDK_MODIFIER_RESERVED_16_MASK  = 1 << 16,
  GDK_MODIFIER_RESERVED_17_MASK  = 1 << 17,
  GDK_MODIFIER_RESERVED_18_MASK  = 1 << 18,
  GDK_MODIFIER_RESERVED_19_MASK  = 1 << 19,
  GDK_MODIFIER_RESERVED_20_MASK  = 1 << 20,
  GDK_MODIFIER_RESERVED_21_MASK  = 1 << 21,
  GDK_MODIFIER_RESERVED_22_MASK  = 1 << 22,
  GDK_MODIFIER_RESERVED_23_MASK  = 1 << 23,
  GDK_MODIFIER_RESERVED_24_MASK  = 1 << 24,
  GDK_MODIFIER_RESERVED_25_MASK  = 1 << 25,

  /* The next few modifiers are used by XKB, so we skip to the end.
   * Bits 15 - 25 are currently unused. Bit 29 is used internally.
   */

  GDK_SUPER_MASK    = 1 << 26,
  GDK_HYPER_MASK    = 1 << 27,
  GDK_META_MASK     = 1 << 28,

  GDK_MODIFIER_RESERVED_29_MASK  = 1 << 29,

  GDK_RELEASE_MASK  = 1 << 30,

  /* Combination of GDK_SHIFT_MASK..GDK_BUTTON5_MASK + GDK_SUPER_MASK
     + GDK_HYPER_MASK + GDK_META_MASK + GDK_RELEASE_MASK */
  GDK_MODIFIER_MASK = 0x5c001fff
} GdkModifierType;

struct _GdkDeviceKey
{
  guint keyval;
  GdkModifierType modifiers;
};

typedef enum {
  GDK_DEVICE_TOOL_TYPE_UNKNOWN,
  GDK_DEVICE_TOOL_TYPE_PEN,
  GDK_DEVICE_TOOL_TYPE_ERASER,
  GDK_DEVICE_TOOL_TYPE_BRUSH,
  GDK_DEVICE_TOOL_TYPE_PENCIL,
  GDK_DEVICE_TOOL_TYPE_AIRBRUSH,
  GDK_DEVICE_TOOL_TYPE_MOUSE,
  GDK_DEVICE_TOOL_TYPE_LENS,
} GdkDeviceToolType;

typedef enum
{
  GDK_AXIS_FLAG_X        = 1 << GDK_AXIS_X,
  GDK_AXIS_FLAG_Y        = 1 << GDK_AXIS_Y,
  GDK_AXIS_FLAG_PRESSURE = 1 << GDK_AXIS_PRESSURE,
  GDK_AXIS_FLAG_XTILT    = 1 << GDK_AXIS_XTILT,
  GDK_AXIS_FLAG_YTILT    = 1 << GDK_AXIS_YTILT,
  GDK_AXIS_FLAG_WHEEL    = 1 << GDK_AXIS_WHEEL,
  GDK_AXIS_FLAG_DISTANCE = 1 << GDK_AXIS_DISTANCE,
  GDK_AXIS_FLAG_ROTATION = 1 << GDK_AXIS_ROTATION,
  GDK_AXIS_FLAG_SLIDER   = 1 << GDK_AXIS_SLIDER,
} GdkAxisFlags;

struct _GdkDeviceTool
{
  GObject parent_instance;
  guint64 serial;
  guint64 hw_id;
  GdkDeviceToolType type;
  GdkAxisFlags tool_axes;
};

struct _GdkSeat
{
  GObject parent_instance;
};

struct _GdkDevice
{
  GObject parent_instance;

  gchar *name;
  GdkInputSource source;
  GdkInputMode mode;
  gboolean has_cursor;
  gint num_keys;
  GdkAxisFlags axis_flags;
  GdkDeviceKey *keys;
  GdkDeviceManager *manager;
  GdkDisplay *display;
  /* Paired master for master,
   * associated master for slaves
   */
  GdkDevice *associated;
  GList *slaves;
  GdkDeviceType type;
  GArray *axes;
  guint num_touches;

  gchar *vendor_id;
  gchar *product_id;

  GdkSeat *seat;
  GdkDeviceTool *last_tool;
};


typedef enum {
  GDK_RENDERING_MODE_SIMILAR = 0,
  GDK_RENDERING_MODE_IMAGE,
  GDK_RENDERING_MODE_RECORDING
} GdkRenderingMode;

struct _GdkDeviceManager
{
  GObject parent_instance;

  /*< private >*/
  GdkDisplay *display;
};

struct _GdkDisplay
{
  GObject parent_instance;

  GList *queued_events;
  GList *queued_tail;

  GHashTable *multiple_click_info;

  guint event_pause_count;       /* How many times events are blocked */

  guint closed             : 1;  /* Whether this display has been closed */

  GArray *touch_implicit_grabs;
  GHashTable *device_grabs;
  GHashTable *motion_hint_info;
  GdkDeviceManager *device_manager;
  GList *input_devices; /* Deprecated, only used to keep gdk_display_list_devices working */

  GHashTable *pointers_info;  /* GdkPointerWindowInfo for each device */
  guint32 last_event_time;    /* Last reported event time from server */

  guint double_click_time;  /* Maximum time between clicks in msecs */
  guint double_click_distance;   /* Maximum distance between clicks in pixels */

  guint has_gl_extension_texture_non_power_of_two : 1;
  guint has_gl_extension_texture_rectangle : 1;

  guint debug_updates     : 1;
  guint debug_updates_set : 1;

  GdkRenderingMode rendering_mode;

  GList *seats;
}

typedef enum
{
  GDK_X_CURSOR            = 0,
  GDK_ARROW               = 2,
  GDK_BASED_ARROW_DOWN    = 4,
  GDK_BASED_ARROW_UP      = 6,
  GDK_BOAT                = 8,
  GDK_BOGOSITY            = 10,
  GDK_BOTTOM_LEFT_CORNER  = 12,
  GDK_BOTTOM_RIGHT_CORNER = 14,
  GDK_BOTTOM_SIDE         = 16,
  GDK_BOTTOM_TEE          = 18,
  GDK_BOX_SPIRAL          = 20,
  GDK_CENTER_PTR          = 22,
  GDK_CIRCLE              = 24,
  GDK_CLOCK               = 26,
  GDK_COFFEE_MUG          = 28,
  GDK_CROSS               = 30,
  GDK_CROSS_REVERSE       = 32,
  GDK_CROSSHAIR           = 34,
  GDK_DIAMOND_CROSS       = 36,
  GDK_DOT                 = 38,
  GDK_DOTBOX              = 40,
  GDK_DOUBLE_ARROW        = 42,
  GDK_DRAFT_LARGE         = 44,
  GDK_DRAFT_SMALL         = 46,
  GDK_DRAPED_BOX          = 48,
  GDK_EXCHANGE            = 50,
  GDK_FLEUR               = 52,
  GDK_GOBBLER             = 54,
  GDK_GUMBY               = 56,
  GDK_GUMBY               = 56,
  GDK_HAND1               = 58,
  GDK_HAND2               = 60,
  GDK_HEART               = 62,
  GDK_ICON                = 64,
  GDK_IRON_CROSS          = 66,
  GDK_LEFT_PTR            = 68,
  GDK_LEFT_SIDE           = 70,
  GDK_LEFT_TEE            = 72,
  GDK_LEFTBUTTON          = 74,
  GDK_LL_ANGLE            = 76,
  GDK_LR_ANGLE            = 78,
  GDK_MAN                 = 80,
  GDK_MIDDLEBUTTON        = 82,
  GDK_MOUSE               = 84,
  GDK_PENCIL              = 86,
  GDK_PIRATE              = 88,
  GDK_PLUS                = 90,
  GDK_QUESTION_ARROW      = 92,
  GDK_RIGHT_PTR           = 94,
  GDK_RIGHT_SIDE          = 96,
  GDK_RIGHT_TEE           = 98,
  GDK_RIGHTBUTTON         = 100,
  GDK_RTL_LOGO            = 102,
  GDK_SAILBOAT            = 104,
  GDK_SB_DOWN_ARROW       = 106,
  GDK_SB_H_DOUBLE_ARROW   = 108,
  GDK_SB_LEFT_ARROW       = 110,
  GDK_SB_RIGHT_ARROW      = 112,
  GDK_SB_UP_ARROW         = 114,
  GDK_SB_V_DOUBLE_ARROW   = 116,
  GDK_SHUTTLE             = 118,
  GDK_SIZING              = 120,
  GDK_SPIDER              = 122,
  GDK_SPRAYCAN            = 124,
  GDK_STAR                = 126,
  GDK_TARGET              = 128,
  GDK_TCROSS              = 130,
  GDK_TOP_LEFT_ARROW      = 132,
  GDK_TOP_LEFT_CORNER     = 134,
  GDK_TOP_RIGHT_CORNER    = 136,
  GDK_TOP_SIDE            = 138,
  GDK_TOP_TEE             = 140,
  GDK_TREK                = 142,
  GDK_UL_ANGLE            = 144,
  GDK_UMBRELLA            = 146,
  GDK_UR_ANGLE            = 148,
  GDK_WATCH               = 150,
  GDK_XTERM               = 152,
  GDK_LAST_CURSOR,
  GDK_BLANK_CURSOR        = -2,
  GDK_CURSOR_IS_PIXMAP    = -1
} GdkCursorType;

struct _GdkCursor
{
  GObject parent_instance;

  GdkDisplay *display;
  GdkCursorType type;
};

typedef struct GdkCursor _GdkCursor;

