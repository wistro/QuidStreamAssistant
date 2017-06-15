/*
  ==============================================================================

  GameTimer.h
  Created: 23 May 2017 6:47:52pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"
#include "StopWatch.h"

//==============================================================================
/*
*/
class GameTimer  : public Component,
            public Button::Listener
{
public:
  GameTimer();
  ~GameTimer();

  void paint (Graphics&) override;
  void resized() override;
  void buttonClicked (Button* button) override;
  
  StopWatch gameTime;
  DrawableButton stop;

private:
  
  DrawableButton playPause;

  DrawableImage play;
  DrawableImage pause;
  DrawableImage mouseOverPlay;
  DrawableImage mouseOverPause;
  
  DrawableImage stopNormal;
  DrawableImage stopMouse;
  DrawableImage stopDown;
  
  Label nameTheThing;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameTimer)
};
