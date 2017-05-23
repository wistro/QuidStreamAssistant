/*
  ==============================================================================

    StopWatch.cpp
    Created: 23 May 2017 6:37:57pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "StopWatch.h"

//==============================================================================
StopWatch::StopWatch()
{
    
}

StopWatch::~StopWatch()
{
}

void StopWatch::timerCallback()
{
    timer.operator+=(1);
}

double StopWatch::currentTimeSec()
{
    return timer.inSeconds();
}
