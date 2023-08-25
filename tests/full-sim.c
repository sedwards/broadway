#define G_OS_UNIX

#include <stdio.h>
#include <fcntl.h>
#include <glib.h>

#include <broadway-protocol.h>
#include <cairo.h>

#include <sys/mman.h>

#include <gdk/gdk.h>
#include <glib-object.h>

#include "gdk_structs.h"

typedef struct _GdkBroadwayServer GdkBroadwayServer;

struct _GdkBroadwayServer {
  GObject parent_instance;

  guint32 next_serial;
  GSocketConnection *connection;

  guint32 recv_buffer_size;
  guint8 recv_buffer[1024];

  guint process_input_idle;
  GList *incomming;
};

struct _GdkBroadwayServerClass
{
  GObjectClass parent_class;
};

//GType                   broadway_req_display_get_type            (void);

typedef struct BroadwayInput _BroadwayInput;
typedef struct _BroadwayServer BroadwayServer;
typedef struct _BroadwayServerClass BroadwayServerClass;

struct _BroadwayServer {
  GObject parent_instance;

  guint32 next_serial;
  GSocketConnection *connection;

  guint32 recv_buffer_size;
  guint8 recv_buffer[1024];

  guint process_input_idle;
  GList *incomming;
};

struct _BroadwayServerClass
{
  GObjectClass parent_class;
};


struct _GdkWindowImplBroadway
{
  GdkWindowImpl parent_instance;

  GdkWindow *wrapper;
  GdkScreen *screen;

  cairo_surface_t *surface;
  cairo_surface_t *last_surface;
  cairo_surface_t *ref_surface;

  GdkCursor *cursor;
  GHashTable *device_cursor;

  int id;

  gboolean visible;
  gboolean maximized;
  int transient_for;

  int pre_maximize_x;
  int pre_maximize_y;
  int pre_maximize_width;
  int pre_maximize_height;

  gint8 toplevel_window_type;
  gboolean dirty;
  gboolean last_synced;

  GdkGeometry geometry_hints;
  GdkWindowHints geometry_hints_mask;
};

struct _GdkWindowImplBroadwayClass
{
  GdkWindowImplClass parent_class;
};

//typedef _GdkBroadwayDisplay GdkBroadwayDisplay;
struct _GdkBroadwayDisplay
{
  //GdkDisplay parent_instance;
  GdkScreen *default_screen;
  GdkScreen **screens;

  GHashTable *id_ht;
  GList *toplevels;

  GSource *event_source;

  /* Keyboard related information */
  GdkKeymap *keymap;

  /* drag and drop information */
  GdkDragContext *current_dest_drag;

  /* The offscreen window that has the pointer in it (if any) */
  GdkWindow *active_offscreen_window;

  GdkBroadwayServer *server;

  gpointer move_resize_data;

  GdkMonitor *monitor;
};

struct _GdkDisplayClass GdkDisplayClass;
struct _GdkDisplayClass
{
   int something;	// FIXME
}; 

//struct _GdkDisplayClass GdkDisplayClass;

struct _GdkBroadwayDisplayClass
{
// FIXME
//  GdkDisplayClass parent_class;
};

typedef struct _GdkBroadwayDisplay GdkBroadwayDisplay;

struct _GdkBroadwayWindow {
//  GdkWindow parent;
   int something;	// FIXME
};

struct _GdkBroadwayWindowClass {
  GdkWindowClass parent_class;
};

typedef struct _GdkBroadwayWindowClas GdkBroadwayWindowClass;

typedef struct _GdkBroadwayWindow GdkBroadwayWindow;

// Static def for displau
const gchar *display = ":0";   // This is lazy
			       
// Global server var, still loook for better way to do this
GdkBroadwayServer *server;        // We could change this back to GdkBroadwayServer and save a headache

#define WINDOW1_WIDTH 700
#define WINDOW1_HEIGHT 500

#define WINDOW2_WIDTH 700
#define WINDOW2_HEIGHT 500

cairo_surface_t *
broadway_req_server_create_surface (int                 width,
                                     int                 height);


guint32
broadway_req_server_new_window (GdkBroadwayServer *server,
                                 int x,
                                 int y,
                                 int width,
                                 int height,
                                 gboolean is_temp);

static BroadwayReply *
broadway_req_server_wait_for_reply (GdkBroadwayServer *server,
                                    guint32 serial);

///// Initialize a bunch of classes we are not really going to init /////////
void
broadway_req_screen_setup (GdkScreen *screen)
{
// This does nothing, for reasons...
}

static void
broadway_req_window_class_init (GdkBroadwayWindowClass *broadway_window_class)
{
// This does nothing, for reasons...
}

static void
broadway_req_window_init (GdkBroadwayWindow *broadway_window)
{
}

void
broadway_req_screen_events_init (GdkScreen *screen)
{
// This does nothing, for reasons...
}

static gboolean
input_available_cb (gpointer stream, gpointer user_data)
{
  GdkBroadwayServer *server = user_data;

  read_some_input_nonblocking (server);
  parse_all_input (server);

  process_input_messages (server);

  return G_SOURCE_CONTINUE;
}

static GType gdk_broadway_server_get_type (void);
#define GDK_TYPE_BROADWAY_SERVER              (gdk_broadway_server_get_type())

GType gdk_broadway_server_get_type(void)
{
}

GdkBroadwayServer *
broadway_req_server_new (const char *display, GError **error)
{
  GdkBroadwayServer *server;
  GSocketClient *client;
  GSocketConnection *connection;
  GInetAddress *inet;
  GSocketAddress *address;
  GPollableInputStream *pollable;
  GInputStream *in;
  GSource *source;
  char *local_socket_type = NULL;
  int port;

  if (display == NULL)
    {
#ifdef G_OS_UNIX
      if (g_unix_socket_address_abstract_names_supported ())
        display = ":0";
      else
#endif
        display = ":tcp";
    }

  if (g_str_has_prefix (display, ":tcp"))
    {
      port = 9090 + strtol (display + strlen (":tcp"), NULL, 10);

      inet = g_inet_address_new_from_string ("127.0.0.1");
      address = g_inet_socket_address_new (inet, port);
      g_object_unref (inet);
    }
#ifdef G_OS_UNIX
  else if (display[0] == ':' && g_ascii_isdigit(display[1]))
    {
      char *path, *basename;

      port = strtol (display + strlen (":"), NULL, 10);
      basename = g_strdup_printf ("broadway%d.socket", port + 1);
      path = g_build_filename (g_get_user_runtime_dir (), basename, NULL);
      g_free (basename);

      address = g_unix_socket_address_new_with_type (path, -1,
                                                     G_UNIX_SOCKET_ADDRESS_ABSTRACT);
      g_free (path);
    }
#endif
  else
    {
      g_set_error (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
                   "Broadway display type not supported: %s", display);
      return NULL;
    }

  g_free (local_socket_type);

  client = g_socket_client_new ();

  connection = g_socket_client_connect (client, G_SOCKET_CONNECTABLE (address), NULL, error);

  g_object_unref (address);
  g_object_unref (client);

  if (connection == NULL)
    return NULL;

  server = g_object_new (GDK_TYPE_BROADWAY_SERVER, NULL);
  server->connection = connection;

  in = g_io_stream_get_input_stream (G_IO_STREAM (server->connection));
  pollable = G_POLLABLE_INPUT_STREAM (in);

  source = g_pollable_input_stream_create_source (pollable, NULL);
  g_source_attach (source, NULL);
  g_source_set_callback (source, (GSourceFunc)input_available_cb, server, NULL);

  return server;
}

static guint32
broadway_req_server_send_message_with_size (GdkBroadwayServer *server, BroadwayRequestBase *base,
                                            gsize size, guint32 type)
{
  GOutputStream *out;
  gsize written;

  base->size = size;
  base->type = type;
  base->serial = server->next_serial++;

  out = g_io_stream_get_output_stream (G_IO_STREAM (server->connection));

  if (!g_output_stream_write_all (out, base, size, &written, NULL, NULL))
    {
      g_printerr ("Unable to write to server\n");
      exit (1);
    }

  g_assert (written == size);

  return base->serial;
}

static guint32
broadway_req_server_send_message(GdkBroadwayServer *server, BroadwayRequestBase *msg, guint32 type)
{
    broadway_req_server_send_message_with_size(server, &msg, sizeof (msg), type);
}

static guint32
broadway_req_server_send_message1(GdkBroadwayServer *server, BroadwayRequestUpdate msg, guint32 type)
{
    broadway_req_server_send_message_with_size(server, &msg, sizeof (msg), type);
}

static guint32
broadway_req_server_send_message2(GdkBroadwayServer *server, BroadwayRequestShowWindow msg, guint32 type)
{
    broadway_req_server_send_message_with_size(server, &msg, sizeof (msg), type);
}

static guint32
broadway_req_server_send_message3(GdkBroadwayServer *server, BroadwayRequestNewWindow msg, guint32 type)
{
    broadway_req_server_send_message_with_size(server, &msg, sizeof (msg), type);
}

//#define broadway_req_server_send_message(_server, _msg, _type) \
 // broadway_req_server_send_message_with_size(_server, (BroadwayRequestBase *)&_msg, sizeof (_msg), _type)

typedef struct {
  char name[36];
  void *data;
  gsize data_size;
  gboolean is_shm;
} BroadwayShmSurfaceData;

static const cairo_user_data_key_t gdk_broadway_shm_cairo_key;

void
broadway_req_server_window_update (GdkBroadwayServer *server,
                                    gint id,
                                    cairo_surface_t *surface)
{
  BroadwayRequestUpdate msg;
  BroadwayShmSurfaceData *data;

  if (surface == NULL)
    return;

  data = cairo_surface_get_user_data (surface, &gdk_broadway_shm_cairo_key);
  g_assert (data != NULL);

  msg.id = id;
  memcpy (msg.name, data->name, 36);
  msg.width = cairo_image_surface_get_width (surface);
  msg.height = cairo_image_surface_get_height (surface);

  broadway_req_server_send_message1 (server, msg,
                                    BROADWAY_REQUEST_UPDATE);
}

void init_broadway_connection(void)
{
    GError *error;
    error = NULL;

    const char *client_port = ":0";

    //dir = g_dir_open(target.c_str(), 0, &error);
    server = broadway_req_server_new(client_port, &error);
    if( error != NULL )
    {
        //std::cout << error->message << std::endl;
        printf("Some error in setting _server_new: %s", error-> message);
        //g_clear_error (&error);
    }

    if( server == NULL )
        printf("init_broadway_connection - For some reason server == NULL which makes no sense\n");
}

void test_broadway_display_open (const char *display)
{
  GdkBroadwayDisplay *broadway_display;
  GdkDeviceManager *broadway_device_manager;
  GdkScreen * screen;
  const gchar *display_env_var = NULL;
  GError *error;
  error = NULL;

  //display = g_object_new (GDK_TYPE_BROADWAY_DISPLAY, NULL);
  //broadway_display = GDK_BROADWAY_DISPLAY (display);

  printf("finished class init \n");

  /* initialize the display's screens */
  broadway_display->screens = g_new (GdkScreen *, 1);
  //broadway_display->screens[0] = broadway_req_screen_new (display, 0);

  printf("Screens configured\n");

  /* We need to initialize events after we have the screen
   * structures in places
   */
  broadway_req_screen_events_init (broadway_display->screens[0]);
  printf("Screen structures in place\n");

  /*set the default screen */
  broadway_display->default_screen = broadway_display->screens[0];
  printf("Default Screen in place\n");

  /* There is no DND support
   * this is a stub interface to a stub class 
   */
  //broadway_req_display_init_dnd (*display);

  /* FIXME: Check why the specific screen fails here */
  //broadway_req_screen_setup (broadway_display->screens[0]);

  broadway_req_screen_setup (broadway_display->default_screen);
  printf("Screen setup for default_screen\n");

  if (broadway_display->default_screen == NULL)
  {
    printf("broadway_display->default_screen == NULL - Trying to check BROADWAY_DISPLAY\n");
    printf(" This should never happen\n");

    //display_env_var = g_getenv("BROADWAY_DISPLAY");
    //broadway_display->screens = display_env_var;
    broadway_display->default_screen = g_getenv("BROADWAY_DISPLAY");
    test_broadway_display_open(display);

    if( broadway_display->default_screen == NULL)
    {
	printf("broadway_display->default_screen still NULL, giving up\n");
        printf(" This should never happen\n");
	exit (0);
    }
    broadway_display->server = broadway_req_server_new (display, &error);
    //broadway_display->server = server;
    printf("broadway_display->server %d\n", (char) broadway_display->server);
  }

  if (broadway_display->default_screen == NULL)
  {
     printf("BroadwayDisplay structure broadway_display->default_screen == NULL\n");
     printf(" This should never happen\n");
  }

   //broadway_req_screen_init_root_window(broadway_display->default_screen);
}

void paint_broadway_window_surface(cairo_t *cr) {
  /* Surface for each rendering of the PNG file, in effect, ever pass becomes a frame */
  cairo_surface_t* surface =  cairo_image_surface_create_from_png("moving.png");

  cairo_set_source_surface (cr,surface, 0, 0);

  cairo_paint (cr);

  //cairo_t *cr = cairo_create (surface);
  //cairo_status_t retm1;

  // Fill the entire surface with red.
  cairo_set_source_rgb(cr, 1, 0, 0);
  cairo_rectangle(cr, 0, 0, WINDOW1_WIDTH, WINDOW1_HEIGHT);
  cairo_fill(cr);
}

void test_broadway_window(GdkBroadwayDisplay *broadway_display)
{
    guint32 id;
    cairo_surface_t *surface;
    gboolean *visable;

    struct GdkBroadwayWindowClass *broadway_window_class;
    broadway_req_window_class_init(broadway_window_class);

    struct GdkBroadwayWindow *broadway_window;
    broadway_req_window_init(broadway_window);

    id = broadway_req_server_new_window(server,0,0,WINDOW1_WIDTH,WINDOW1_HEIGHT,0);
    printf("broadway_req_server_new_window %d window created\n",id);

    visable = broadway_req_server_window_show (server, id);

    if(visable == TRUE)
      {
	 printf("broadway_req_server_window_show is true and function did something\n");
      }
    else
      {
	 printf("broadway_req_server_window_show is somehow false\n");
      }

    surface = broadway_req_server_create_surface(WINDOW1_WIDTH,WINDOW1_HEIGHT); 
    printf("broadway_req_server_create_surface did something\n");

    paint_broadway_window_surface(surface);

    broadway_req_server_window_update (server, id, surface);
    printf("broadway_req_server_window_update\n");

    broadway_req_server_window_show (server, id);

    if(visable == TRUE)
      {printf("broadway_req_server_window_show - Part 2 is true and function did something\n");}
    else
      {printf("broadway_req_server_window_show - Part 2 is somehow false\n");}
}

void main()
{
   gtk_init();

   struct GdkBroadwayDisplay *broadway_display;

   // Initialze the Window class ourselves as we can't use all of the GDK/GTK functionality
   struct GdkWindowImplClass2 *klass;

   init_broadway_connection();
   test_broadway_display_open(display);
   //gdk_window_impl_broadway_class_init (klass);

   for (;;)
   {
     // FIXME: Add an arg to switch windows
     test_broadway_window(broadway_display);
     sleep(2);
   }
}

#if 0
/* We are so not here yet */
void test_kbd_and_mouse()
{
  //broadway_device_manager = broadway_req_device_manager_new ((char) display);
  //printf("Broadway Device Manager Initialized: %d\n", (const char) broadway_device_manager);
}
#endif

static void shm_data_destroy (void *_data);
static void *map_named_shm (char *name, gsize size, gboolean *is_shm);

static const cairo_user_data_key_t broadway_shm_cairo_key;

static char
make_valid_fs_char (char c)
{
  char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";

  return chars[c % (sizeof (chars) - 1)];
}

/* name must have at least space for 34 bytes */
static gpointer
create_random_shm (char *name, gsize size, gboolean *is_shm)
{
  guint32 r;
  int i, o;
  gpointer ptr;

  while (TRUE)
    {
      o = 0;
      name[o++] = '/';
      name[o++] = 'b';
      name[o++] = 'd';
      name[o++] = 'w';
      name[o++] = '-';
      for (i = 0; i < 32/4 - 1; i++)
        {
          r = g_random_int ();
          name[o++] = make_valid_fs_char ((r >> 0) & 0xff);
          name[o++] = make_valid_fs_char ((r >> 8) & 0xff);
          name[o++] = make_valid_fs_char ((r >> 16) & 0xff);
          name[o++] = make_valid_fs_char ((r >> 24) & 0xff);
        }
      name[o++] = 0;

      ptr = map_named_shm (name, size, is_shm);
      if (ptr)
        return ptr;
    }
}

cairo_surface_t *
broadway_req_server_create_surface (int                 width,
                                     int                 height)
{
  BroadwayShmSurfaceData *data;
  cairo_surface_t *surface;

  data = g_new (BroadwayShmSurfaceData, 1);
  data->data_size = width * height * sizeof (guint32);
  data->data = create_random_shm (data->name, data->data_size, &data->is_shm);

  surface = cairo_image_surface_create_for_data ((guchar *)data->data,
                                                 CAIRO_FORMAT_ARGB32, width, height, width * sizeof (guint32));
  g_assert (surface != NULL);

  cairo_surface_set_user_data (surface, &broadway_shm_cairo_key,
                               data, shm_data_destroy);

  return surface;
}

static void
shm_data_destroy (void *_data)
{
  BroadwayShmSurfaceData *data = _data;

#ifdef G_OS_UNIX

  munmap (data->data, data->data_size);
  if (data->is_shm)
    shm_unlink (data->name);
  else
    {
      char *filename = g_build_filename (g_get_tmp_dir (), data->name, NULL);
      unlink (filename);
      g_free (filename);
    }

#elif defined(G_OS_WIN32)
#if 0
  char *name = data->name;
  char *shmpath;

  if (*name == '/')
    ++name;

  shmpath = g_build_filename (g_get_tmp_dir (), name, NULL);
  UnmapViewOfFile (data->data);
  remove (shmpath);
  g_free (shmpath);
#endif
#endif
  g_free (data);
}

static void *
map_named_shm (char *name, gsize size, gboolean *is_shm)
{
#ifdef G_OS_UNIX

  char *filename = NULL;
  int fd;
  void *ptr;
  int res;

  fd = shm_open(name, O_RDWR|O_CREAT|O_EXCL, 0600);
  if (fd == -1)
    {
      if (errno == EEXIST)
        return NULL;

      filename = g_build_filename (g_get_tmp_dir (), name, NULL);

      fd = open (filename, O_RDWR | O_CREAT | O_EXCL, 0600);
      g_free (filename);
      if (fd == -1)
        {
          if (errno != EEXIST)
            g_error ("Unable to allocate shared mem for window");
          return NULL;
        }
      else
        *is_shm = FALSE;
    }
  else
    *is_shm = TRUE;

  res = ftruncate (fd, size);
  g_assert (res != -1);

//#ifdef HAVE_POSIX_FALLOCATE
  res = posix_fallocate (fd, 0, size);
  if (res != 0 && errno == ENOSPC)
    {
      if (filename)
        unlink (filename);
      else
        shm_unlink (name);
      g_error ("Not enough shared memory for window surface");
    }
//#endif

  ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  (void) close(fd);

  return ptr;


#else
#error "No shm mapping supported"
  return NULL;
#endif
}

#if 0
static guint32
broadway_req_server_send_message_with_size (GdkBroadwayServer *server, BroadwayRequestBase *base,
                                            gsize size, guint32 type)
{
  GOutputStream *out;
  gsize written;

  base->size = size;
  base->type = type;
  base->serial = server->next_serial++;

  out = g_io_stream_get_output_stream (G_IO_STREAM (server->connection));

  if (!g_output_stream_write_all (out, base, size, &written, NULL, NULL))
    {
      g_printerr ("Unable to write to server\n");
      exit (1);
    }

  g_assert (written == size);

  return base->serial;
}

static guint32
broadway_req_server_send_message(GdkBroadwayServer *server, BroadwayRequestShowWindow msg, guint32 type)
{
    broadway_req_server_send_message_with_size(server, (BroadwayRequestBase *)&msg, sizeof (msg), type);
}
//#define broadway_req_server_send_message(_server, _msg, _type) \
 // broadway_req_server_send_message_with_size(_server, (BroadwayRequestBase *)&_msg, sizeof (_msg), _type)
#endif

gboolean
broadway_req_server_window_show (GdkBroadwayServer *server,
                                  gint id)
{
  BroadwayRequestShowWindow msg;

  msg.id = id;
  broadway_req_server_send_message2 (server, msg,
                                    BROADWAY_REQUEST_SHOW_WINDOW);

  return TRUE;
}

void
parse_all_input (GdkBroadwayServer *server)
{
  guint8 *p, *end;
  guint32 size;
  BroadwayReply *reply;

  p = server->recv_buffer;
  end = p + server->recv_buffer_size;

  while (p + sizeof (guint32) <= end)
    {
      memcpy (&size, p, sizeof (guint32));
      if (p + size > end)
        break;

      reply = g_memdup2 (p, size);
      p += size;

      server->incomming = g_list_append (server->incomming, reply);
    }

  if (p < end)
    memmove (server->recv_buffer, p, end - p);
  server->recv_buffer_size = end - p;
}

void
read_some_input_nonblocking (GdkBroadwayServer *server)
{
  GInputStream *in;
  GPollableInputStream *pollable;
  gssize res;
  GError *error;

  in = g_io_stream_get_input_stream (G_IO_STREAM (server->connection));
  pollable = G_POLLABLE_INPUT_STREAM (in);

  g_assert (server->recv_buffer_size < sizeof (server->recv_buffer));
  error = NULL;
  res = g_pollable_input_stream_read_nonblocking (pollable, &server->recv_buffer[server->recv_buffer_size],
                                                  sizeof (server->recv_buffer) - server->recv_buffer_size,
                                                  NULL, &error);

  if (res < 0 && g_error_matches (error, G_IO_ERROR, G_IO_ERROR_WOULD_BLOCK))
    {
      g_error_free (error);
      res = 0;
    }
  else if (res <= 0)
    {
      g_printerr ("Unable to read from broadway server: %s\n", error ? error->message : "eof");
      exit (1);
    }

  server->recv_buffer_size += res;
}

static BroadwayReply *
find_response_by_serial (GdkBroadwayServer *server, guint32 serial)
{
  GList *l;

  for (l = server->incomming; l != NULL; l = l->next)
    {
      BroadwayReply *reply = l->data;

      if (reply->base.in_reply_to == serial)
        return reply;
    }

  return NULL;
}

void
process_input_messages (GdkBroadwayServer *server)
{
  BroadwayReply *reply;

  if (server->process_input_idle != 0)
    {
      g_source_remove (server->process_input_idle);
      server->process_input_idle = 0;
    }

  while (server->incomming)
    {
      reply = server->incomming->data;
      server->incomming =
        g_list_delete_link (server->incomming,
                            server->incomming);

      if (reply->base.type == BROADWAY_REPLY_EVENT)
	  printf("NEXT WE HAVE TO SUPPORT EVENTS...SIGH\n");
        //_gdk_broadway_events_got_input (&reply->event.msg);
      else
        g_warning ("Unhandled reply type %d", reply->base.type);
      g_free (reply);
    }
}

static gboolean
process_input_idle_cb (GdkBroadwayServer *server)
{
  server->process_input_idle = 0;
  process_input_messages (server);
  return G_SOURCE_REMOVE;
}

static void
queue_process_input_at_idle (GdkBroadwayServer *server)
{
  if (server->process_input_idle == 0)
    server->process_input_idle =
      g_idle_add_full (G_PRIORITY_DEFAULT, (GSourceFunc)process_input_idle_cb, server, NULL);
}

#if 0
static gboolean
input_available_cb (gpointer stream, gpointer user_data)
{
  GdkBroadwayServer *server = user_data;

  read_some_input_nonblocking (server);
  parse_all_input (server);

  process_input_messages (server);

  return G_SOURCE_CONTINUE;
}

static GType gdk_broadway_server_get_type (void);
#define GDK_TYPE_BROADWAY_SERVER              (gdk_broadway_server_get_type())

GType gdk_broadway_server_get_type(void)
{
}

GdkBroadwayServer *
broadway_req_server_new (const char *display, GError **error)
{
  GdkBroadwayServer *server;
  GSocketClient *client;
  GSocketConnection *connection;
  GInetAddress *inet;
  GSocketAddress *address;
  GPollableInputStream *pollable;
  GInputStream *in;
  GSource *source;
  char *local_socket_type = NULL;
  int port;

  if (display == NULL)
    {
#ifdef G_OS_UNIX
      if (g_unix_socket_address_abstract_names_supported ())
        display = ":0";
      else
#endif
        display = ":tcp";
    }

  if (g_str_has_prefix (display, ":tcp"))
    {
      port = 9090 + strtol (display + strlen (":tcp"), NULL, 10);

      inet = g_inet_address_new_from_string ("127.0.0.1");
      address = g_inet_socket_address_new (inet, port);
      g_object_unref (inet);
    }
#ifdef G_OS_UNIX
  else if (display[0] == ':' && g_ascii_isdigit(display[1]))
    {
      char *path, *basename;

      port = strtol (display + strlen (":"), NULL, 10);
      basename = g_strdup_printf ("broadway%d.socket", port + 1);
      path = g_build_filename (g_get_user_runtime_dir (), basename, NULL);
      g_free (basename);

      address = g_unix_socket_address_new_with_type (path, -1,
                                                     G_UNIX_SOCKET_ADDRESS_ABSTRACT);
      g_free (path);
    }
#endif
  else
    {
      g_set_error (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
                   "Broadway display type not supported: %s", display);
      return NULL;
    }

  g_free (local_socket_type);

  client = g_socket_client_new ();

  connection = g_socket_client_connect (client, G_SOCKET_CONNECTABLE (address), NULL, error);

  g_object_unref (address);
  g_object_unref (client);

  if (connection == NULL)
    return NULL;

  server = g_object_new (GDK_TYPE_BROADWAY_SERVER, NULL);
  server->connection = connection;

  in = g_io_stream_get_input_stream (G_IO_STREAM (server->connection));
  pollable = G_POLLABLE_INPUT_STREAM (in);

  source = g_pollable_input_stream_create_source (pollable, NULL);
  g_source_attach (source, NULL);
  g_source_set_callback (source, (GSourceFunc)input_available_cb, server, NULL);

  return server;
}
#endif

guint32
broadway_req_server_new_window (GdkBroadwayServer *server,
                                 int x,
                                 int y,
                                 int width,
                                 int height,
                                 gboolean is_temp)
{
  BroadwayRequestNewWindow msg;
  guint32 serial, id;
  BroadwayReply *reply;

  msg.x = x;
  msg.y = y;
  msg.width = width;
  msg.height = height;
  msg.is_temp = is_temp;

  serial = broadway_req_server_send_message3 (server, msg,
                                             BROADWAY_REQUEST_NEW_WINDOW);
  reply = broadway_req_server_wait_for_reply (server, serial);

  g_assert (reply->base.type == BROADWAY_REPLY_NEW_WINDOW);

  id = reply->new_window.id;

  g_free (reply);

  return id;
}

static void
read_some_input_blocking (GdkBroadwayServer *server)
{
  GInputStream *in;
  gssize res;

  in = g_io_stream_get_input_stream (G_IO_STREAM (server->connection));

  g_assert (server->recv_buffer_size < sizeof (server->recv_buffer));
  res = g_input_stream_read (in, &server->recv_buffer[server->recv_buffer_size],
                             sizeof (server->recv_buffer) - server->recv_buffer_size,
                             NULL, NULL);

  if (res <= 0)
    {
      g_printerr ("Unable to read from broadway server\n");
      exit (1);
    }

  server->recv_buffer_size += res;
}

static BroadwayReply *
broadway_req_server_wait_for_reply (GdkBroadwayServer *server,
                                    guint32 serial)
{
  BroadwayReply *reply;

  while (TRUE)
    {
      reply = find_response_by_serial (server, serial);
      if (reply)
        {
          server->incomming = g_list_remove (server->incomming, reply);
          break;
        }

      read_some_input_blocking (server);
      parse_all_input (server);
    }

  queue_process_input_at_idle (server);
  return reply;
}

