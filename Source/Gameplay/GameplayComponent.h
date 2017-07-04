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
                            public Label::Listener,
                            public ComboBox::Listener
{
public:
  GameplayComponent();
  ~GameplayComponent();

  void paint (Graphics&) override;
  void resized() override;
  void sliderValueChanged (Slider* slider) override;
  void buttonClicked (Button* button) override;
  void labelTextChanged (Label* label) override;
  void comboBoxChanged (ComboBox* box) override;
  
  void checkSnitchMistakes(char period);
  
  void writeToFile ( bool gameSetup = false );
  
  //technically this is 17 min and 1 second because
  //we update the sop timer every time the gametime value changes
  //and so it gets updated when the initial 00:00 gets set
  //so at that point it will remove the extra second and the timer will be accurate
  static const int sopInSec = 1021; //17 minutes in seconds
  static const int seekersInSec = 60; //1 minute after snitch released
  static const int handicapsInSec = 300; //after seekers released, each handicap is 5 minutes
  static const StringArray FLAGS;
  static const String goalFlag;
  static const String blueFlag;
  static const String yellowFlag;
  static const String redFlag;
  static const String dblYellowFlag;

private:
    
  //team, tournament and round dropdown lists with labels
  Label tournament;
  Label round;
  Label teamOne;
  Label teamTwo;
  
  ImageComponent t1logo;
  ImageComponent t2logo;
  
  ComboBox roundList;
  ComboBox team1;
  ComboBox team2;
  
  TextButton gameSetup;
  TextButton switchEnds;
  
  TextButton blue1;
  TextButton yellow1;
  TextButton red1;
  int indexOfGoal1;
  bool showG1;
  RelativeTime sinceDisplayGoal1;
//  Array<RelativeTime> cardTimers1;
  
  TextButton blue2;
  TextButton yellow2;
  TextButton red2;
  int indexOfGoal2;
  bool showG2;
  RelativeTime sinceDisplayGoal2;
//  Array<RelativeTime> cardTimers2;
  //timers for cards have too many moving parts for right now,
  //just going to take the stats and call it a day
  //i'll figure out displaying it later
  
  File writeHere;
  File writeHereDir;
  
  Label hr;
  ComboBox hrList;
  
  Label snitch;
  ComboBox snitchList;
  
  Label streamer;
  
  //snitch catch sliders are all in one pretty box here
  SnitchSliders snitchesGetStitches;
  
  ScoreComponent score1;
  ScoreComponent score2;
  
  GameTimer gameTime;
  
  String tournamentName;
  StringArray teamList;
  StringArray teamAbvs;
  
  StringArray team1Stats;
  StringArray team2Stats;
  
  RelativeTime sopTimer;
  ToggleButton sopShow;
  
  //probably going to move these to their own window
  ToggleButton corner;
  ToggleButton lowerthird;
  ToggleButton endScreen;
  
  int countdownFlag;
  bool showCorner;
  bool showLowerThird;
  bool showEndScreen;
  bool hasLogoTourn;
  bool hasLogoT1;
  bool hasLogoT2;
  bool showSOP;
  bool isFirstGame;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameplayComponent)
};
