/*
  ==============================================================================

  EditTeamWindow.h
  Created: 29 May 2017 3:47:46pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"
#include "RosterListBoxComponent.h"

//==============================================================================
/*
*/
class EditTeamWindow  : public Component,
                public Button::Listener,
                public TextEditor::Listener
{
public:
  EditTeamWindow();
  EditTeamWindow( String teamName );
  ~EditTeamWindow();

  void paint (Graphics&) override;
  void resized() override;
  void buttonClicked (Button* button) override;

private:
  
  void initBasics();
  
  ScopedPointer<Team> thisTeam;
  
  TextButton save;
  TextButton cancel;
  TextButton browse;
  TextButton addPlayer;
  TextButton removePlayer;
  TextButton pi;
  TextButton infinity;
  
  TextEditor teamName;
  TextEditor abvBox;
  TextEditor logoImage;
  
  ScopedPointer<RosterListBoxComponent> roster;
  
  Label team;
  Label abv;
  Label logo;
  Label rosterLabel;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditTeamWindow)
};
