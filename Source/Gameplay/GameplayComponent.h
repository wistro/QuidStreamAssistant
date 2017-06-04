/*
  ==============================================================================

  GameplayComponent.h
  Created: 22 May 2017 11:30:54pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"
#include "SnitchSliders.h"
#include "ScoreComponent.h"
#include "GameTimer.h"

//==============================================================================
/*
*/
class GameplayComponent  : public Component,
              public Slider::Listener,
              public Button::Listener,
              public Label::Listener
{
public:
  GameplayComponent();
  ~GameplayComponent();

  void paint (Graphics&) override;
  void resized() override;
  void sliderValueChanged (Slider* slider) override;
  void buttonClicked (Button* button) override;
  void labelTextChanged (Label* label) override;
  
  void checkSnitchMistakes(char period);
  
  void writeToFile (const File& file) const;

private:
  
  static const char reg = 'r';
  static const char ot = 'o';
  static const char dot = 'd';
  
  //team, tournament and round dropdown lists with labels
  Label tournament;
  Label round;
  
  Label outputFile;
  TextEditor outputFileBox;
  TextButton browse;
  
  File writeHere;
  File writeHereDir;
  
  //snitch catch sliders are all in one pretty box here
  SnitchSliders snitchesGetStitches;
  
  ScoreComponent score1;
  ScoreComponent score2;
  
  GameTimer gameTime;
  

  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameplayComponent)
};
