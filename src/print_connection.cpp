// vim:ts=4:sw=4:expandtab
// vim:ts=4:sw=4:expandtab
#include <cctype>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <yajl/yajl_gen.h>


extern "C" {
    #include "i3status.h"
}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>

using namespace std;

extern "C" void print_connection(yajl_gen json_gen, cfg_t* cfg, char *buffer);
void print_connection(yajl_gen json_gen, cfg_t* cfg, char *buffer) 
{
    const char *walk, *last;
    char *outwalk = buffer;

    int cnt = cfg_size(cfg, "interfaces");
    int x = 0;
    bool found = false;
    const char* ip;

    for(x = 0; x < cnt; x++) {
        if((ip = get_ip_addr(cfg_getnstr(cfg, "interfaces", x))) != NULL) {
            found = true;
            break;
        }
    }

    if(found) {
        switch(x) {
            case 0:
                START_COLOR("color_good");
                break;
            case 1:
                START_COLOR("color_degraded");
                break;
            default:
                START_COLOR("color_magic");
                break;
        }
        outwalk += sprintf(outwalk, "%s", ip);   
    } else {
        START_COLOR("color_bad");
        outwalk += sprintf(outwalk, "127.0.0.1");
    }
    END_COLOR;

    *outwalk = '\0';
    OUTPUT_FULL_TEXT(buffer);
}
