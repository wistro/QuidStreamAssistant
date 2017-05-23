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
    

    //take the image files from memory and map them to local DrawableImage variables
    play.setImage( ImageCache::getFromMemory( BinaryData::play_png, BinaryData::play_pngSize ));
    
    pause.setImage( ImageCache::getFromMemory( BinaryData::pause_png, BinaryData::pause_pngSize ));
    
    mouseOverPlay.setImage( ImageCache::getFromMemory( BinaryData::mouseOverPlay_png, BinaryData::mouseOverPlay_pngSize ));
    
    mouseOverPause.setImage( ImageCache::getFromMemory( BinaryData::mouseOverPause_png, BinaryData::mouseOverPause_pngSize ));
    
    //then take those DrawableImages and copy them into the DrawableButton
    playPause.setImages( &play, &mouseOverPlay, nullptr, nullptr, &pause,
                        &mouseOverPause, nullptr, nullptr );
    
    //get rid of weird colour masking that happens when the button is toggled
    playPause.setColour(DrawableButton::backgroundOnColourId, Colours::transparentWhite);
    
    //show the pretty
    addAndMakeVisible(playPause);
    addAndMakeVisible(gameTime);
    
    playPause.addListener(this);
}

GameTimer::~GameTimer()
{
    playPause.removeListener(this);
}

void GameTimer::paint (Graphics& g)
{

}

void GameTimer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    Rectangle<int> area(getLocalBounds());
    const int buttonWidth = 50;
    
    playPause.setBounds(area.removeFromRight(buttonWidth));
    gameTime.setBounds(area);

}

void GameTimer::buttonClicked (Button* button)
{
    const int interval = 1000; //number of milliseconds in a second
    
    if ( button == &playPause )
    {
        if ( playPause.getToggleState() )
        {
            gameTime.startTimer(interval);
        }
        else
        {
            gameTime.stopTimer();
        }
    }
}
