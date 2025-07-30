// dsd_ncurses_api.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 8096

int api_received_number = 0;  // Global o'zgaruvchi

// Callback funksiyasi
static enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
                   const char *url, const char *method,
                   const char *version, const char *upload_data,
                   size_t *upload_data_size, void **con_cls)
{
    (void)cls;
    (void)url;
    (void)version;
    (void)con_cls;

    static int post_received = 0;

    if (strcmp(method, "POST") != 0)
        return MHD_NO;

    if (*upload_data_size != 0)
    {
        api_received_number = atoi(upload_data);
        printf("Qabul qilingan son: %d\n", api_received_number);

        *upload_data_size = 0;
        post_received = 1;
        return MHD_YES;
    }

    if (post_received)
    {
        const char *response_text = "Son qabul qilindi!\n";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(response_text),
                                                (void *) response_text, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        post_received = 0;
        return ret;
    }

    return MHD_YES;
}

// HTTP serverni ishga tushirish funksiyasi
struct MHD_Daemon *start_api_server()
{
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                              &handle_request, NULL, MHD_OPTION_END);

    if (!daemon)
    {
        fprintf(stderr, "API serverni ishga tushirib bo‘lmadi\n");
        return NULL;
    }

    printf("API server port %d da ishga tushdi\n", PORT);
    return daemon;
}
