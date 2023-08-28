#include "common.h"

typedef struct _BroadwayServer BroadwayServer;

BroadwayServer *broadway_server;

int main (int argc, char *argv[])
{
    GError *error;
    error = NULL;

    gtk_init (&argc, &argv);

    // BroadwayServer *broadway_server;
    //GdkDisplay *display;
    char *client_port;
    client_port = ":0";
    //dir = g_dir_open(target.c_str(), 0, &error);
    broadway_server = broadway_req_server_new(client_port, &error);
    if( error != NULL )
    {
        //std::cout << error->message << std::endl;
	printf("Some error: %s", error-> message);
        g_clear_error (&error);
    }

    gtk_init (&argc, &argv);
    
    run_unit_tests();

#if 0
    //printf("calling broadway_display_open\n");
    //display = broadway_display_open(client_port);
    //printf("broadway_display_open\n");

    //printf("broadway_server_new function did something: %s", error-> message);
    //printf("broadway_server_new function did something\n");

    id = broadway_req_server_new_window(broadway_server,100,100,100,100,0);
    printf("broadway_req_server_new_window function did something\n");

    broadway_req_server_window_show (broadway_server, id);
    printf("broadway_req_server_window_show function did something\n");

    surface = broadway_req_server_create_surface(100,100);
    printf("broadway_req_server_create_surface did something\n");

    broadway_req_server_window_update (broadway_server, id, surface);
    printf("broadway_req_server_window_update\n");

    broadway_req_server_window_show (broadway_server, id);
    printf("broadway_req_server_window_show again\n");
    //return id;
#endif
    gtk_main();

    for ( ;; )
	{
	  printf("We wait here forever\n");
	  sleep(3);
        }
    return 0;
}
