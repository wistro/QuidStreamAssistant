/*
  ==============================================================================

  IntroAlertWindow.cpp
  Created: 28 May 2017 7:35:17pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "IntroAlertWindow.h"
#include "../TopLevel/Application.h"
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
  
  overlayFolderIntro = "Current Overlays Folder: ";
  
  if ( getGlobalProperties().containsKey( StoredSettings::overlaysSettingName ) )
  {
    currOverlaysFile.setText(overlayFolderIntro + getGlobalProperties().getValue(StoredSettings::overlaysSettingName), dontSendNotification);
  }
  else //default value
  {
    currOverlaysFile.setText(overlayFolderIntro + File::getSpecialLocation(File::userHomeDirectory).\
                                    getChildFile("QuidStreamAssistant/Overlays").getFullPathName(), dontSendNotification);
    getGlobalProperties().setValue(StoredSettings::overlaysSettingName, currOverlaysFile.getText());
  }
  
  addAndMakeVisible(currOverlaysFile);
  
  changeOverlaysLoc.setButtonText("Change Overlay Folder");
  changeOverlaysLoc.setTooltip("Click here to change the directory where your JS, CSS and HTML are kept. \
                               Current value is above");
  changeOverlaysLoc.addListener(this);
  addAndMakeVisible(changeOverlaysLoc);

  QuidStreamAssistantApplication::getApp().thisTournament->refreshTournamentList();
  
  tournamentList.addItemList(QuidStreamAssistantApplication::getApp().thisTournament->tournamentList, 1);
  tournamentList.setText("Add New Tournament", dontSendNotification);
  tournamentList.setEditableText(false);
  addAndMakeVisible(tournamentList);
  
  if ( getGlobalProperties().containsKey("lastUsedTournament") )
  {
    tournamentList.setSelectedId(getGlobalProperties().getIntValue("lastUsedTournament"));
  }

}

IntroAlertWindow::~IntroAlertWindow()
{
  quit.removeListener(this);
  select.removeListener(this);
  changeOverlaysLoc.removeListener(this);
}

void IntroAlertWindow::paint (Graphics& g)
{
  g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

  Rectangle<int> area (getLocalBounds());
  const int margin = 10;
  const int buttonHeight = 70;
  
  g.setColour (getLookAndFeel().findColour(Label::textColourId));
  g.setFont (20.0f);
  g.drawText ("What Tournament are you streaming?", area.removeFromTop(area.getHeight() / 3),
        Justification::centred, false);
  
  area.removeFromRight(margin);
  area.removeFromLeft(margin);
  
  Rectangle<int> overlayChange (area.removeFromBottom(buttonHeight * 2));
  currOverlaysFile.setBounds(overlayChange.removeFromTop(buttonHeight).reduced(margin));
  changeOverlaysLoc.setBounds(overlayChange.removeFromRight(overlayChange.getWidth() / 2).reduced(margin));
  
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
  QuidStreamAssistantApplication::getApp().thisTournament->refreshTournamentList();
  tournamentList.clear();
  tournamentList.addItemList(QuidStreamAssistantApplication::getApp().thisTournament->tournamentList, 1);
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
      QuidStreamAssistantApplication::getApp().thisTournament->readFromFile(Tournament::getTournamentsFolder().getChildFile(tournamentList.getText()).withFileExtension(Tournament::getTournamentFileSuffix()));
      
      getGlobalProperties().setValue("lastUsedTournament", tournamentList.getSelectedId());
    }
    MainAppWindow::getMainAppWindow()->closeButtonPressed();
    QuidStreamAssistantApplication::getApp().showEditTournamentWindow();
  }
  else if ( button == &changeOverlaysLoc )
  {
    FileChooser fc ("Choose a directory...",
                    File::getSpecialLocation(File::userHomeDirectory),
                    "*",
                    true);
    
    if (fc.browseForDirectory())
    {
      File chosenDirectory = fc.getResult();
      
      getGlobalProperties().setValue(StoredSettings::overlaysSettingName, chosenDirectory.getFullPathName());
      currOverlaysFile.setText(overlayFolderIntro + chosenDirectory.getFullPathName(), dontSendNotification);
    }
  }
}

