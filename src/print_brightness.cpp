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
#include <glob.h>

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
    ifstream brightness_if("/sys/class/backlight/intel_backlight/brightness");
    if(brightness_if.good())
    {
        brightness_if >> brightness;
    }
    brightness_if.close();

    // const char* used_char = "⬛";
    const char* used_char = "▮";
    // const char* free_char = "⬜";
    const char* free_char = " ";
    const int cnt = 10;

    const char* delims[] = {" ", "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█"};
    int val = (int)((float)brightness * (cnt + 1) / max_brightness);
    if(val > cnt)
        val = cnt;
    else if(val < 0)
        val = 0;

    for(int x = 0; x <= val; x++)
        outwalk += sprintf(outwalk, "%s", used_char);

    for(int x = val; x < cnt; x++)
        outwalk += sprintf(outwalk, "%s", free_char);

    *outwalk = '\0';
    END_COLOR;
    OUTPUT_FULL_TEXT(buffer);
}
