// vim:ts=4:sw=4:expandtab
// vim:ts=4:sw=4:expandtab
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <yajl/yajl_gen.h>


extern "C" {
    #include "i3status.h"
}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

using namespace std;

extern "C" void print_connection(yajl_gen json_gen, char *buffer);
void print_connection(yajl_gen json_gen, char *buffer) 
{
    const char *walk, *last;
    char *outwalk = buffer;

    const char* ip;
    if((ip = get_ip_addr("eno16777736")) != NULL)
    {
        START_COLOR("color_good");
        outwalk += sprintf(outwalk, "%s", ip);
    }
    else if((ip = get_ip_addr("eno16777736")) != NULL)
    {
        START_COLOR("color_degraded");
        outwalk += sprintf(outwalk, "%s", ip);
    }
    else
    {
        START_COLOR("color_bad");
        outwalk += sprintf(outwalk, "127.0.0.1");
    }
    END_COLOR;

    *outwalk = '\0';
    OUTPUT_FULL_TEXT(buffer);
}
