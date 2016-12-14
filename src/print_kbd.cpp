// vim:ts=4:sw=4:expandtab
// vim:ts=4:sw=4:expandtab
#include <cctype>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <yajl/yajl_gen.h>

extern "C" {
    #include "../include/i3status.h"
}


#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#if defined(__OpenBSD__)
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <machine/apmvar.h>
#endif

#if defined(__NetBSD__)
#include <fcntl.h>
#include <prop/proplib.h>
#include <sys/envsys.h>
#endif

#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#define MAXGROUPLENGTH 256
#define OUTPUT_LENGTH 3
#define XKB_CTRLS_MASK (XkbAllControlsMask & ~(XkbInternalModsMask | XkbIgnoreLockModsMask))

using namespace std;
typedef std::vector<std::string> string_vector;

class XKeyboard
{
public:
    Display* _display;
    int _deviceId;
    XkbDescRec* _kbdDescPtr;
    int _ngroups;
    char** _groups;

    XKeyboard();
    ~XKeyboard();
    int get_group_id() const;
    int get_gr_num(Display *dpy, XkbDescPtr kb);
};

XKeyboard::XKeyboard()
    : _display(0), _deviceId(XkbUseCoreKbd)
{

    XkbIgnoreExtension(False);

    char* displayName = strdup("");
    int eventCode;
    int errorReturn;
    int major = XkbMajorVersion;
    int minor = XkbMinorVersion;
    int reasonReturn;
    _display = XkbOpenDisplay(displayName, &eventCode, &errorReturn, &major,
        &minor, &reasonReturn);

    _kbdDescPtr = XkbAllocKeyboard();
    if (_kbdDescPtr == NULL) {
        XCloseDisplay(_display);
    }

    _kbdDescPtr->dpy = _display;
    if (_deviceId != XkbUseCoreKbd) {
        _kbdDescPtr->device_spec = _deviceId;
    }


    _ngroups = get_gr_num(_display, _kbdDescPtr);
    _groups = (char**)malloc(sizeof(char*) * _ngroups);
    for (int i = 0; i < _ngroups; i++)
        _groups[i] = (char*)malloc(MAXGROUPLENGTH); 

    char *name = NULL;

    if (XkbGetNames(_display, XkbGroupNamesMask, _kbdDescPtr) != Success);
        // eprint("skb: XkbGetNames() failed");
  
    for (int i = 0; i < _ngroups; i++) {
        if (_kbdDescPtr->names->groups[i])
            if ((name = XGetAtomName(_display, _kbdDescPtr->names->groups[i])))
                snprintf(_groups[i], OUTPUT_LENGTH + 1, name);
    }
    XkbFreeNames(_kbdDescPtr, XkbGroupNamesMask, 0);
}

int XKeyboard::get_gr_num(Display *dpy, XkbDescPtr kb) {
    int rv;

    if (XkbGetControls(dpy, XKB_CTRLS_MASK, kb) != Success);
        // eprint("skb: XkbGetControls() failed.\n");

    rv = kb->ctrls->num_groups;
    XkbFreeControls(kb, XKB_CTRLS_MASK, 0);
    return rv;
}

XKeyboard::~XKeyboard()
{
    if(_kbdDescPtr!=NULL)
        XkbFreeKeyboard(_kbdDescPtr, 0, True);

    XCloseDisplay(_display);

    for(int x = 0; x < _ngroups; x++)
        free(_groups[x]);
    free(_groups);
}

int XKeyboard::get_group_id() const
{
    XkbStateRec xkbState;
    XkbGetState(_display, _deviceId, &xkbState);
    return static_cast<int>(xkbState.group);
}

XKeyboard xkb;

extern "C" void print_kbd_info(yajl_gen json_gen, cfg_t* cfg, char *buffer);
void print_kbd_info(yajl_gen json_gen, cfg_t* cfg, char *buffer) 
{
    // int cnt = cfg_size(cfg, "layouts");
    // int group = xkb.get_group_id();
    
    // const char* layout = "--";
    // if(group >= 0 && group <= cnt)
        // layout = cfg_getnstr(cfg, "layouts", group);

    #define PATH "/tmp/pwld.out"
    char buf[8] = {0};
    strcpy(buf, "--");

    FILE* input;
    input = fopen(PATH, "r");
    if(input) {
        auto result = fread (buf, sizeof(char), 2, input);
        if(result == 2) {
        }
    }

    char* outwalk = buffer;
    outwalk += sprintf(outwalk, buf);
    *outwalk = '\0';
    OUTPUT_FULL_TEXT(buffer);
}
