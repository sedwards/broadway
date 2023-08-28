#ifndef __BROADWAY_SERVER__
#define __BROADWAY_SERVER__

#include <sys/mman.h>

#include "broadway-protocol.h"
#include <glib-object.h>
#include <cairo.h>

typedef struct _BroadwayServer BroadwayServer;
typedef struct _BroadwayServerClass BroadwayServerClass;

#define BROADWAY_TYPE_SERVER              (broadway_server_get_type())
#define BROADWAY_SERVER(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), BROADWAY_TYPE_SERVER, BroadwayServer))
#define BROADWAY_SERVER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BROADWAY_TYPE_SERVER, BroadwayServerClass))
#define BROADWAY_IS_SERVER(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), BROADWAY_TYPE_SERVER))
#define BROADWAY_IS_SERVER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BROADWAY_TYPE_SERVER))
#define BROADWAY_SERVER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BROADWAY_TYPE_SERVER, BroadwayServerClass))

#endif /* __BROADWAY_SERVER__ */
