/*
  ==============================================================================

    Headers.h
    Created: 26 May 2017 10:06:33pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once


//==============================================================================
struct TargetOS
{
    enum OS
    {
        windows = 0,
        osx,
        linux,
        unknown
    };
    
    static OS getThisOS() noexcept
    {
#if JUCE_WINDOWS
        return windows;
#elif JUCE_MAC
        return osx;
#elif JUCE_LINUX
        return linux;
#else
        return unknown;
#endif
    }
};

typedef TargetOS::OS DependencyPathOS;

//==============================================================================

#include "PresetIDs.h"
#include "../JuceLibraryCode/JuceHeader.h"
