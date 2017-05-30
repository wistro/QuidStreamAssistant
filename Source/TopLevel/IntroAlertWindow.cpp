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
#include "MainAppWindow.h"
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
    
    tournament = new Tournament;
    
    tournament->refreshTournamentList();
    
    tournamentList.addItemList(tournament->tournamentList, 1);
    tournamentList.setText("Add New Tournament", dontSendNotification);
    tournamentList.setEditableText(false);
    addAndMakeVisible(tournamentList);

}

IntroAlertWindow::IntroAlertWindow(Tournament* t)
{
    
    quit.setButtonText("Quit");
    quit.addListener(this);
    addAndMakeVisible(quit);
    
    select.setButtonText("Select");
    select.addListener(this);
    addAndMakeVisible(select);
    
    tournament = t;
    
    tournament->refreshTournamentList();
    
    tournamentList.addItemList(tournament->tournamentList, 1);
    tournamentList.setText("Add New Tournament", dontSendNotification);
    tournamentList.setEditableText(false);
    addAndMakeVisible(tournamentList);
    
}

IntroAlertWindow::~IntroAlertWindow()
{
    quit.removeListener(this);
    select.removeListener(this);
}

void IntroAlertWindow::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    Rectangle<int> area (getLocalBounds());
    const int margin = 10;
    const int buttonHeight = 70;
    
    g.setColour (getLookAndFeel().findColour(Label::textColourId));
    g.setFont (20.0f);
    g.drawText ("What Tournament are you streaming?", area.removeFromTop(area.getHeight() / 2),
                Justification::centred, false);
    
    area.removeFromRight(margin);
    area.removeFromLeft(margin);
    
    Rectangle<int> buttons (area.removeFromBottom(buttonHeight).reduced(margin));
    select.setBounds(buttons.removeFromRight((buttons.getWidth() / 2) - (margin / 2)).reduced(margin));
    quit.setBounds(buttons.removeFromLeft(buttons.getWidth() - (margin / 2)).reduced(margin));
    
    tournamentList.setBounds(area.removeFromBottom(buttonHeight));
}

void IntroAlertWindow::resized()
{
    // never resized so not bothering

}

void IntroAlertWindow::updateTournamentList()
{
    tournament->refreshTournamentList();
    tournamentList.clear();
    tournamentList.addItemList(tournament->tournamentList, 1);
    tournamentList.setText("Add New Tournament", dontSendNotification);
    tournamentList.setEditableText(false);
}

void IntroAlertWindow::buttonClicked (Button* button)
{
    if ( button == &quit )
        QuidStreamAssistantApplication::getApp().systemRequestedQuit();
    else if ( button == &select )
    {
        //changing this to open the edit file no matter what, but load the selected tournament first if one is selected
        if ( tournamentList.getSelectedId() > 0 )
        {
            tournament->readFromFile(tournament->getTournamentsFolder().getChildFile(tournamentList.getText()).withFileExtension(tournament->getTournamentFileSuffix()));
        }
        MainAppWindow::getMainAppWindow()->closeButtonPressed();
        QuidStreamAssistantApplication::getApp().showEditTournamentWindow();
    }
}

