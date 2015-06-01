// vim:ts=4:sw=4:expandtab
// vim:ts=4:sw=4:expandtab
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <yajl/yajl_gen.h>
#include <fstream>
#include <iostream>
extern "C" {
    #include "i3status.h"
}

using namespace std;

extern "C" void print_brightness(yajl_gen json_gen, char *buffer);
void print_brightness(yajl_gen json_gen, char *buffer)
{
    const char *walk, *last;
    char *outwalk = buffer;

    START_COLOR("color_magic");

    long max_brightness = -1;
    ifstream max_brightness_if("/sys/class/backlight/intel_backlight/max_brightness");
    if(max_brightness_if.good())
    {
        max_brightness_if >> max_brightness;
    }
    max_brightness_if.close();
    
    long brightness = -1;
    fstream brightness_if("/sys/class/backlight/intel_backlight/brightness");
    if(max_brightness_if.good())
    {
        brightness_if >> brightness;
    }
    brightness_if.close();

    const wchar_t delims[] = L"▅▆▁▂▃▄▅▆▇█";
    int val = (int)((float)brightness * 10 / max_brightness);
    if(val == 10)
        val == 9;
    else if(val < 0)
        val = 0;


    // cout << max_brightness << endl;
    // cout << brightness << endl;

    outwalk += sprintf(outwalk, "%c", delims[val]);
    END_COLOR;

    *outwalk = '\0';
    OUTPUT_FULL_TEXT(buffer);
}
