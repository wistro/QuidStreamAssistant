/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   mouseOverPause_png;
    const int            mouseOverPause_pngSize = 2724;

    extern const char*   mouseOverPlay_png;
    const int            mouseOverPlay_pngSize = 6625;

    extern const char*   pause_png;
    const int            pause_pngSize = 2815;

    extern const char*   play_png;
    const int            play_pngSize = 7167;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 4;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}
