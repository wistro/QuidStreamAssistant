/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   willogo_png;
    const int            willogo_pngSize = 63700;

    extern const char*   downStop_png;
    const int            downStop_pngSize = 2120;

    extern const char*   mouseOverPause_png;
    const int            mouseOverPause_pngSize = 2724;

    extern const char*   mouseOverPlay_png;
    const int            mouseOverPlay_pngSize = 6625;

    extern const char*   mouseOverStop_png;
    const int            mouseOverStop_pngSize = 2119;

    extern const char*   pause_png;
    const int            pause_pngSize = 2815;

    extern const char*   play_png;
    const int            play_pngSize = 7167;

    extern const char*   stop_png;
    const int            stop_pngSize = 2117;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 8;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}
