/*
  ==============================================================================

    GameTimer.cpp
    Created: 23 May 2017 6:47:52pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GameTimer.h"

//==============================================================================
GameTimer::GameTimer()
            : playPause("playPause", DrawableButton::ImageFitted)
{
    //make button a toggle button because that's what we need
    playPause.setClickingTogglesState(true);
}

GameTimer::~GameTimer()
{
}

void GameTimer::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("GameTimer", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void GameTimer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
