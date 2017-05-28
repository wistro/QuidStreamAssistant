/*
  ==============================================================================

    IntroAlertWindow.h
    Created: 28 May 2017 7:35:17pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"
#include "../Settings/TournamentDatabase.h"

//==============================================================================
/*
*/
class IntroAlertWindow    : public Component,
                               public Button::Listener
{
public:
    IntroAlertWindow();
    ~IntroAlertWindow();

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button* button) override;

private:
    
    StringArray tournaments;
    
    TournamentDatabase theDB;
    
    TextButton quit;
    TextButton select;
    TextButton addTeam;
    ComboBox tournamentList;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntroAlertWindow)
};
