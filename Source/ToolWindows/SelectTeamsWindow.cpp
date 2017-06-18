/*
==============================================================================

  SelectTeamsWindow.cpp
  Created: 31 May 2017 1:28:55pm
  Author:  Willow Rosenberg

==============================================================================
*/

#include "SelectTeamsWindow.h"
#include "../Settings/Team.h"
#include "../TopLevel/Application.h"

//==============================================================================

SelectTeamsWindow::SelectTeamsWindow()
{
  teamList = new TeamListBoxComponent();
  addAndMakeVisible(teamList);
  
  initBasics();
}

SelectTeamsWindow::SelectTeamsWindow( StringArray teams )
{
  teamList = new TeamListBoxComponent( teams );
  addAndMakeVisible(teamList);
  
  initBasics();
}

SelectTeamsWindow::~SelectTeamsWindow()
{
  selectAll.removeListener(this);
  selectNone.removeListener(this);
  saveTournament.removeListener(this);
  cancel.removeListener(this);
  editSelectedTeam.removeListener(this);
  addTeam.removeListener(this);
}

void SelectTeamsWindow::initBasics()
{
  tournamentHeader.setText(QuidStreamAssistantApplication::getApp().thisTournament->getTournamentName(), dontSendNotification);
  tournamentHeader.setFont(20.0f);
  addAndMakeVisible(tournamentHeader);
  
  selectAll.setButtonText("Select All");
  selectAll.addListener(this);
  addAndMakeVisible(selectAll);
  
  selectNone.setButtonText("Select None");
  selectNone.addListener(this);
  addAndMakeVisible(selectNone);
  
  saveTournament.setButtonText("Save");
  saveTournament.setTooltip("Selected teams will be available to show when creating game overlays");
  saveTournament.addListener(this);
  addAndMakeVisible(saveTournament);
  
  cancel.setButtonText("Cancel");
  cancel.setTooltip("Return to Tournament Edit Screen. Does not save changes.");
  cancel.addListener(this);
  addAndMakeVisible(cancel);
  
  editSelectedTeam.setButtonText("Edit Selected Team");
  editSelectedTeam.setTooltip("Opens a new window to change the details of the selected team.");
  editSelectedTeam.addListener(this);
  addAndMakeVisible(editSelectedTeam);
  
  addTeam.setButtonText("Add New Team");
  addTeam.setTooltip("Opens a new window to create a new team from defaults");
  addTeam.addListener(this);
  addAndMakeVisible(addTeam);
  
}

//==============================================================================
void SelectTeamsWindow::resized()
{
  Rectangle<int> area(getLocalBounds());
  const int margin = 2;
  const int textBoxHeight = 30;
  const int buttonHeight = 40;
  
  Rectangle<int> saveCancel (area.removeFromBottom(buttonHeight).reduced(margin));
  saveTournament.setBounds(saveCancel.removeFromRight(proportionOfWidth(0.25f)).reduced(margin));
  cancel.setBounds(saveCancel.removeFromRight(proportionOfWidth(0.25f)).reduced(margin));
  
  Rectangle<int> teamEditButtons (area.removeFromBottom(buttonHeight).reduced(margin));
  addTeam.setBounds(teamEditButtons.removeFromLeft(proportionOfWidth(0.25f)).reduced(margin));
  editSelectedTeam.setBounds(teamEditButtons.removeFromLeft(proportionOfWidth(0.25f)).reduced(margin));
  
  tournamentHeader.setBounds(area.removeFromTop(textBoxHeight).reduced(margin));
  
  Rectangle<int> selectButtons (area.removeFromTop(buttonHeight).reduced(margin));
  selectAll.setBounds(selectButtons.removeFromLeft(proportionOfWidth(0.25f)).reduced(margin));
  selectNone.setBounds(selectButtons.removeFromLeft(proportionOfWidth(0.25f)).reduced(margin));
  
  teamList->setBounds(area);
}

//==============================================================================

void SelectTeamsWindow::buttonClicked(Button* button)
{
  if ( button == &selectAll )
  {
    teamList->toggleAll(true);
  }
  else if (  button == &selectNone )
  {
    teamList->toggleAll(false);
  }
  else if ( button == &saveTournament )
  {
    QuidStreamAssistantApplication::getApp().thisTournament->clearTeamsList();
    for ( int i = 0; i < teamList->getNumRows(); i++ )
    {
      if ( teamList->getToggled(i) )
      {
        QuidStreamAssistantApplication::getApp().thisTournament->addTeam(teamList->getTeamName(i));
      }
      else
      {
        QuidStreamAssistantApplication::getApp().thisTournament->removeTeam(teamList->getTeamName(i));
      }
    }
    const File file ( Tournament::getTournamentsFolder().getChildFile(QuidStreamAssistantApplication::getApp().thisTournament->getTournamentName()).withFileExtension(Tournament::getTournamentFileSuffix()));
    QuidStreamAssistantApplication::getApp().thisTournament->writeToFile(file);
    QuidStreamAssistantApplication::getApp().showStreamingWindow();
    QuidStreamAssistantApplication::getApp().teamSelect = nullptr;
  }
  else if ( button == &cancel )
  {
    QuidStreamAssistantApplication::getApp().showEditTournamentWindow();
    QuidStreamAssistantApplication::getApp().teamSelect = nullptr;
  }
  else if ( button == &editSelectedTeam )
  {
    QuidStreamAssistantApplication::getApp().showEditTeamWindow(teamList->getTeamName(teamList->getSelectedRow()));
    QuidStreamAssistantApplication::getApp().teamSelect = nullptr;
  }
  else if ( button == &addTeam)
  {
    QuidStreamAssistantApplication::getApp().showEditTeamWindow();
    QuidStreamAssistantApplication::getApp().teamSelect = nullptr;
  }
}

//==============================================================================

void Team::writeLogoFiles()
{
  const File overlaysDir (getGlobalProperties().getValue(StoredSettings::overlaysSettingName));
  const File logosCSS (overlaysDir.getChildFile("/scripts"));

  for ( int i = 0; i < QuidStreamAssistantApplication::getApp().thisTournament->teams.size(); i++)
  {
    Team* currTeam = QuidStreamAssistantApplication::getApp().thisTournament->teams[i];
    File output (overlaysDir.getChildFile("/logos/" + currTeam->getTeamAbv() + ".png"));
    FileOutputStream imageData (output);
    
    if (currTeam->logo.isValid())
    {
      if (PNGImageFormat().writeImageToStream (currTeam->logo, imageData))
      {
        imageData.flush();
      }
      
      //figure out how best to output the css data... maybe rely on base c++ file writing, though that's obnoxious
    }
  }
}

//==============================================================================
