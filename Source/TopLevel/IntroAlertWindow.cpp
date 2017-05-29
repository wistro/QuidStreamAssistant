/*
  ==============================================================================

    IntroAlertWindow.cpp
    Created: 28 May 2017 7:35:17pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "IntroAlertWindow.h"
#include "../Settings/TeamSettings.h"
#include "Application.h"

//==============================================================================
IntroAlertWindow::IntroAlertWindow()
{
    
    quit.setButtonText("Quit");
    quit.addListener(this);
    addAndMakeVisible(quit);
    
    select.setButtonText("Select");
    select.addListener(this);
    addAndMakeVisible(select);
    
    Tournament::refreshTournamentList();
    
    tournamentList.addItemList(Tournament::tournamentList, 0);
    tournamentList.setText("Add New Tournament", dontSendNotification);
    tournamentList.setEditableText(false);
    addAndMakeVisible(tournamentList);

}

IntroAlertWindow::~IntroAlertWindow()
{
}

void IntroAlertWindow::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("IntroAlertWindow", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
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

