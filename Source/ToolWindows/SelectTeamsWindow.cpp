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
  saveAndOutput.removeListener(this);
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
  
  saveAndOutput.setButtonText("Save & Output");
  saveAndOutput.setTooltip("Output selected teams' logo files to Overlays/icons/<TEAM ABV>.png. THIS WILL TAKE A WHILE. Don't Panic. If you have already done this previously, use the \"save\" button below.");
  saveAndOutput.addListener(this);
  addAndMakeVisible(saveAndOutput);
  
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
  saveAndOutput.setBounds(teamEditButtons.removeFromRight(proportionOfWidth(0.25f)).reduced(margin));
  
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
  else if ( button == &saveAndOutput )
  {
    QuidStreamAssistantApplication::getApp().thisTournament->clearTeamsList();
    for ( int i = 0; i < teamList->getNumRows(); i++ )
    {
      if ( teamList->getToggled(i) )
      {
        QuidStreamAssistantApplication::getApp().thisTournament->addTeam(teamList->getTeamName(i), true);
      }
      else
      {
        QuidStreamAssistantApplication::getApp().thisTournament->removeTeam(teamList->getTeamName(i));
      }
    }
    const File file ( Tournament::getTournamentsFolder().getChildFile(QuidStreamAssistantApplication::getApp().thisTournament->getTournamentName()).withFileExtension(Tournament::getTournamentFileSuffix()));
    QuidStreamAssistantApplication::getApp().thisTournament->writeToFile(file);
    
    Team::writeLogoCSSFile();
    
    QuidStreamAssistantApplication::getApp().showStreamingWindow();
    QuidStreamAssistantApplication::getApp().teamSelect = nullptr;
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
    QuidStreamAssistantApplication::getApp().showStreamingWindow();
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

//static public Team member function (found in ../Settings/Team.h)
//iterates through the list of teams selected for the current Tournament
//and outputs their logo files to individual PNG image files in
// <OVERLAYS FOLDER>/logos
//each team's logo is named <team abbreviation>.png
//also creates a CSS file with class descriptions for each logo file
//to display a given team's logo on the overlay:
// -create an HTML object (eg. a <div>) with class="logo <team abbreviation>"
//replacing <team abbreviation> with the abbreviation given for the team in question
//to help with this, the abbreviations for teams 1 and 2 in a game are output in the
//streaming XML file as "t1short" and "t2short" respectively
//see ../Gameplay/GameplayComponent.cpp::writeToFile() for details on that XML output
void Team::writeLogoCSSFile()
{
  const File overlaysDir (getGlobalProperties().getValue(StoredSettings::overlaysSettingName));
  const File logosCSS (overlaysDir.getFullPathName() + "/scripts/logos.css");
  Team* currTeam;
  
  String css = "";
  logosCSS.deleteFile();

  for ( int i = 0; i < QuidStreamAssistantApplication::getApp().thisTournament->teams.size(); i++)
  {
    currTeam = QuidStreamAssistantApplication::getApp().thisTournament->teams[i];
    
    //this is a CSS entry that is compatible with the included JavaScript for creating and managing the overlays
    css += ".logo." + currTeam->getTeamAbv() + "{ background: url('../icons/" + currTeam->getTeamAbv() + ".png'); background-position: center; background-size: 100% 100%; }\n";
  }
  
  FileOutputStream outCSS (logosCSS);
  outCSS.write(css.toUTF8(), CharPointer_UTF8::getBytesRequiredFor( css.getCharPointer() ));
  outCSS.flush();
}

//==============================================================================
