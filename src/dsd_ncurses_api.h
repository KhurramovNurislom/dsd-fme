
// dsd_ncurses_api.h
#ifndef DSD_NCURSES_API_H
#define DSD_NCURSES_API_H

#include <microhttpd.h>

extern int api_received_number;
struct MHD_Daemon *start_api_server(void);

#endif
