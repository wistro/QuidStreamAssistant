/*
  ==============================================================================

    StopWatch.h
    Created: 23 May 2017 6:37:57pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class StopWatch    : public Timer
{
public:
    StopWatch();
    ~StopWatch();

    void timerCallback() override;
    
    double currentTimeSec();

private:
    
    RelativeTime timer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StopWatch)
};
