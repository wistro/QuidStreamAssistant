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
    

    play.setImage( ImageCache::getFromMemory( BinaryData::play_png, BinaryData::play_pngSize ));
    
    pause.setImage( ImageCache::getFromMemory( BinaryData::pause_png, BinaryData::pause_pngSize ));
    
    mouseOverPlay.setImage( ImageCache::getFromMemory( BinaryData::mouseOverPlay_png, BinaryData::mouseOverPlay_pngSize ));
    
    mouseOverPause.setImage( ImageCache::getFromMemory( BinaryData::mouseOverPause_png, BinaryData::mouseOverPause_pngSize ));
    
    playPause.setImages( &play, &mouseOverPlay, nullptr, nullptr, &pause,
                        &mouseOverPause, nullptr, nullptr );
    playPause.setColour(DrawableButton::backgroundOnColourId, Colours::transparentWhite);
    
    addAndMakeVisible(playPause);
}

GameTimer::~GameTimer()
{
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

}
