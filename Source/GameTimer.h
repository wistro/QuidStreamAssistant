/*
  ==============================================================================

    GameTimer.h
    Created: 23 May 2017 6:47:52pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "StopWatch.h"

//==============================================================================
/*
*/
class GameTimer    : public Component
{
public:
    GameTimer();
    ~GameTimer();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    DrawableButton playPause;
    DrawableImage play;
    DrawableImage pause;
    DrawableImage mouseOverPlay;
    DrawableImage mouseOverPause;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameTimer)
};
