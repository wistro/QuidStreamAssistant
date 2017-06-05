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
  
  void writeToFile ( bool gameSetup = false ) const;

private:
    
  //team, tournament and round dropdown lists with labels
  Label tournament;
  Label round;
  Label teamOne;
  Label teamTwo;
  
  ComboBox roundList;
  ComboBox team1;
  ComboBox team2;
  
  Label outputFile;
  TextEditor outputFileBox;
  TextButton browse;
  TextButton gameSetup;
  
  File writeHere;
  File writeHereDir;
  
  //snitch catch sliders are all in one pretty box here
  SnitchSliders snitchesGetStitches;
  
  ScoreComponent score1;
  ScoreComponent score2;
  
  GameTimer gameTime;
  
  String tournamentName;
  StringArray teamList;
  StringArray teamAbvs;
  ToggleButton useAbvs;
  
  ToggleButton corner;
  ToggleButton lowerthird;
  ToggleButton endScreen;

  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameplayComponent)
};
