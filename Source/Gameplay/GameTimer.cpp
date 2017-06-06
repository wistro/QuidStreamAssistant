/*
  ==============================================================================

  GameTimer.cpp
  Created: 23 May 2017 6:47:52pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "GameTimer.h"

//==============================================================================
GameTimer::GameTimer()
      : playPause("playPause", DrawableButton::ImageFitted),
        stop("stop", DrawableButton::ImageFitted)
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
  
  //read in stop button images
  stopNormal.setImage( ImageCache::getFromMemory( BinaryData::stop_png, BinaryData::stop_pngSize ));
  
  stopMouse.setImage( ImageCache::getFromMemory( BinaryData::mouseOverStop_png, BinaryData::mouseOverStop_pngSize ));
  
  stopDown.setImage( ImageCache::getFromMemory( BinaryData::downStop_png, BinaryData::downStop_pngSize ));
  
  //pass images to the stop button
  stop.setImages( &stopNormal, &stopMouse, &stopDown );
  
  
  //show the pretty
  addAndMakeVisible(playPause);
  addAndMakeVisible(stop);
  addAndMakeVisible(gameTime);
  
  playPause.addListener(this);
  stop.addListener(this);
  
  nameTheThing.setText("Game Time", dontSendNotification);
  nameTheThing.setJustificationType(Justification::centred);
  nameTheThing.setFont(14.0f);
  addAndMakeVisible(nameTheThing);
}

GameTimer::~GameTimer()
{
  playPause.removeListener(this);
  stop.removeListener(this);
}

void GameTimer::paint (Graphics& g)
{

}

void GameTimer::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
  Rectangle<int> area(getLocalBounds());
  const int margin = 4;
  const int buttonDim = (area.getHeight() / 2) - (margin / 2);
  const int textHeight = 20;

  nameTheThing.setBounds(area.removeFromBottom(textHeight));
  
  Rectangle<int> buttonArea(area.removeFromRight(buttonDim));
  playPause.setBounds(buttonArea.removeFromTop(buttonDim));
  stop.setBounds(buttonArea.removeFromBottom(buttonDim));
  
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
  else if ( button == &stop )
  {
    if ( gameTime.currentTimeSec() != 0)
    {
      gameTime.stopTimer();
      playPause.setToggleState(false, dontSendNotification);
      getLookAndFeel().setUsingNativeAlertWindows(true);
      
      if ( AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
                         "Are You Sure?",
                         "At some point in the future this might let you save the current game time to a file. For now, this is your last chance to write it down before it's gone forever. (Cancel to abort)",
                         String(),
                         String(),
                         &stop, nullptr) )
      {
        gameTime.resetTimer();
      }
    }
  }
}

