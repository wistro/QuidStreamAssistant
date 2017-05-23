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
#include "ConfirmClick.h"

//==============================================================================
/*
*/
class GameTimer    : public Component,
                        public Button::Listener
{
public:
    GameTimer();
    ~GameTimer();

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button* button) override;

private:
    
    DrawableButton playPause;
    DrawableButton stop;
    
    DrawableImage play;
    DrawableImage pause;
    DrawableImage mouseOverPlay;
    DrawableImage mouseOverPause;
    
    DrawableImage stopNormal;
    DrawableImage stopMouse;
    DrawableImage stopDown;
    
    StopWatch gameTime;
    
    Label nameTheThing;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameTimer)
};
