/*
  ==============================================================================

  SelectTeamsWindow.h
  Created: 31 May 2017 1:28:55pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once
#include "../Settings/OSDependencyThings.h"
#include "TeamListBoxComponent.h"

//==============================================================================

class SelectTeamsWindow  : public Component,
                public Button::Listener
{
public:
  SelectTeamsWindow();
  SelectTeamsWindow( String teamsPipeDelineated );
  ~SelectTeamsWindow();
  
  //==============================================================================
  void resized() override;
  void buttonClicked (Button* button) override;
  
  
private:
  ScopedPointer<TeamListBoxComponent> teamList;
  
  TextButton selectAll;
  TextButton selectNone;
  
  TextButton saveTournament;
  TextButton cancel;
  
  TextButton editSelectedTeam;
  TextButton addTeam;
  
  Label tournamentHeader;
  
  void initBasics();
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SelectTeamsWindow)
};
