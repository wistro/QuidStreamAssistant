/*
  ==============================================================================

  EditTournamentWindow.h
  Created: 29 May 2017 3:47:46pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"

//==============================================================================
/*
*/
class EditTournamentWindow  : public Component,
                public Button::Listener
{
public:
  EditTournamentWindow();
  ~EditTournamentWindow();

  void paint (Graphics&) override;
  void resized() override;
  void buttonClicked (Button* button) override;

private:
  
  TextButton save;
  TextButton cancel;
  TextButton browse;
  TextButton setDefault;
  TextButton restoreFactory;
  
  TextEditor tournName;
  TextEditor location;
  TextEditor logoImage;
  TextEditor editRounds;
  
  Label tournament;
  Label loc;
  Label logo;
  Label rounds;
  
  ToggleButton consolationBracket;
  String curTournName; //save this in case we are editing a tournament to prompt user on save
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditTournamentWindow)
};
