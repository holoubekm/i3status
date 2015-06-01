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

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

using namespace std;
typedef std::vector<std::string> string_vector;

class XKeyboard
{
public:
    Display* _display;
    int _deviceId;
    XkbDescRec* _kbdDescPtr;

    XKeyboard();
    ~XKeyboard();
    int get_group() const;
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
}

XKeyboard::~XKeyboard()
{
    if(_kbdDescPtr!=NULL)
        XkbFreeKeyboard(_kbdDescPtr, 0, True);

    XCloseDisplay(_display);
}

int XKeyboard::get_group() const
{
    XkbStateRec xkbState;
    XkbGetState(_display, _deviceId, &xkbState);
    return static_cast<int>(xkbState.group);
}

XKeyboard xkb;

extern "C" void print_kbd_info(yajl_gen json_gen, char *buffer);
void print_kbd_info(yajl_gen json_gen, char *buffer) 
{
    const char* layouts[] = { "EN", "CS" };
    

    int group = xkb.get_group();
    const char* layout = "XX";
    if(group < sizeof(layouts) / sizeof(layouts[0]))
        layout = layouts[group];

    const char *walk, *last;
    char *outwalk = buffer;
    outwalk += sprintf(outwalk, layout);
    *outwalk = '\0';
    OUTPUT_FULL_TEXT(buffer);
}
