/*
  ==============================================================================

    IntroAlertWindow.cpp
    Created: 28 May 2017 7:35:17pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "IntroAlertWindow.h"
#include "Application.h"
#include "../Settings/Tournament.h"

//==============================================================================
IntroAlertWindow::IntroAlertWindow()
{
    
    quit.setButtonText("Quit");
    quit.addListener(this);
    addAndMakeVisible(quit);
    
    select.setButtonText("Select");
    select.addListener(this);
    addAndMakeVisible(select);
    
    tournament.refreshTournamentList();
    
    tournamentList.addItemList(tournament.tournamentList, 0);
    tournamentList.setText("Add New Tournament", dontSendNotification);
    tournamentList.setEditableText(false);
    addAndMakeVisible(tournamentList);

}

IntroAlertWindow::~IntroAlertWindow()
{
}

void IntroAlertWindow::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    Rectangle<int> area (getLocalBounds());
    const int margin = 10;
    const int buttonHeight = 30;
    const int buttonWidth = 50;
    
    g.setColour (getLookAndFeel().findColour(Label::textColourId));
    g.setFont (14.0f);
    g.drawText ("What Tournament are you streaming?", area.removeFromTop(area.getHeight() / 2),
                Justification::centred, false);
    
    Rectangle<int> buttons (area.removeFromBottom(buttonHeight).reduced(margin));
    select.setBounds(buttons.removeFromRight(buttonWidth).reduced(margin));
    quit.setBounds(buttons);
    
    tournamentList.setBounds(area);
}

void IntroAlertWindow::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void IntroAlertWindow::buttonClicked (Button* button)
{
    if ( button == &quit )
        QuidStreamAssistantApplication::getApp().systemRequestedQuit();
    else if ( button == &select )
    {
        if ( tournamentList.getSelectedId() == -1 )
        {
            //open a window to create a new tournament file
        }
        else
        {
            String tournament = tournamentList.getText();
            //open main app window populated with this tournament's settings
        }
    }
}

