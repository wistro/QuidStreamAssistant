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
    repaint();
}

double StopWatch::currentTimeSec()
{
    return timer.inSeconds();
}

void StopWatch::paint (Graphics& g)
{
    int minutes = floor(timer.inMinutes());
    int seconds = timer.inSeconds() - (minutes * 60);
    
    String min = "";
    String sec = "";
    
    if ( minutes < 10 )
    {
        min = "0" + String(minutes);
    }
    else
    {
        min = String(minutes);
    }
    
    if ( seconds < 10 )
    {
        sec = "0" + String(seconds);
    }
    else
    {
        sec = String(seconds);
    }
    
    g.setFont(50.0f);
    g.drawText (min + ":" + sec, getLocalBounds(),
                Justification::centred, false);
}

void StopWatch::resized()
{
    
}
