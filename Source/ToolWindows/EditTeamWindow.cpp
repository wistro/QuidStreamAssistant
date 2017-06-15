/*
  ==============================================================================

  EditTeamWindow.cpp
  Created: 29 May 2017 3:47:46pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "MainAppWindow.h"
#include "EditTeamWindow.h"
#include "../TopLevel/Application.h"

//==============================================================================
EditTeamWindow::EditTeamWindow()
{
  thisTeam = new Team();
  
  initBasics();
}

EditTeamWindow::EditTeamWindow(String teamName)
{
  const File file ( Team::getTeamsFolder().getChildFile(teamName).withFileExtension(Team::getTeamFileSuffix()));
  
  if ( file.existsAsFile() )
  {
    thisTeam = new Team(file);
  }
  else
  {
    thisTeam = new Team();
  }
  
  initBasics();
}

EditTeamWindow::~EditTeamWindow()
{
  save.removeListener(this);
  cancel.removeListener(this);
  browse.removeListener(this);
  addPlayer.removeListener(this);
  removePlayer.removeListener(this);
  pi.removeListener(this);
  infinity.removeListener(this);
}

void EditTeamWindow::initBasics()
{
  save.setButtonText("Save");
  save.addListener(this);
  addAndMakeVisible(save);
  
  cancel.setButtonText("Cancel");
  cancel.addListener(this);
  addAndMakeVisible(cancel);
  
  addPlayer.setButtonText("Add Player");
  addPlayer.setTooltip("Add a new player to this team's roster.");
  addPlayer.addListener(this);
  addAndMakeVisible(addPlayer);
  
  removePlayer.setButtonText("Remove Selected Player");
  removePlayer.setTooltip("Removes the highlighted player from the roster. CANNOT BE UNDONE.");
  removePlayer.addListener(this);
  addAndMakeVisible(removePlayer);
  
  browse.setButtonText("...");
  browse.setTooltip("Browse for Image file");
  browse.addListener(this);
  addAndMakeVisible(browse);
  
  teamName.setTextToShowWhenEmpty("Enter Team Name", Colours::black.withAlpha(0.5f));
  team.setText("Team Name", dontSendNotification);
  team.attachToComponent(&teamName, true);
  
  addAndMakeVisible(teamName);
  addAndMakeVisible(team);
  
  abvBox.setTextToShowWhenEmpty("Enter a Team Abbvreviation (eg. TUK for Team UK)", Colours::black.withAlpha(0.5f));
  abv.setText("Team Abbreviation", dontSendNotification);
  abv.attachToComponent(&abvBox, true);
  
  addAndMakeVisible(abvBox);
  addAndMakeVisible(abv);
  
  logoImage.setTextToShowWhenEmpty("path to logo image (png, jpg or gif)", Colours::black.withAlpha(0.5f));
  logo.setText("Logo Image File", dontSendNotification);
  logo.attachToComponent(&logoImage, true);
  
  addAndMakeVisible(logoImage);
  addAndMakeVisible(logo);
  
  if ( thisTeam->getTeamName() != Team::defaultTeamName )
  {
    teamName.setText(thisTeam->getTeamName());
    abvBox.setText(thisTeam->getTeamAbv());
    
    roster = new RosterListBoxComponent( thisTeam->getTeamName() );
    
    if ( thisTeam->logo.isValid() )
      logoImage.setTextToShowWhenEmpty("logo exists for this Tournament. add file here to change", Colours::black.withAlpha(0.5f));
  }
  else
  {
    roster = new RosterListBoxComponent();
    
  }
  
  rosterLabel.setText("Roster", dontSendNotification);
  rosterLabel.attachToComponent(roster, false);
  addAndMakeVisible(roster);
  addAndMakeVisible(rosterLabel);
  
  pi.setButtonText(Player::pi);
  pi.setTooltip("set selected Player's number to pi");
  pi.addListener(this);
  addAndMakeVisible(pi);
  
  infinity.setButtonText(Player::infinity);
  infinity.setTooltip("set selected Player's number to infinity");
  infinity.addListener(this);
  addAndMakeVisible(infinity);
  
}

//==============================================================================

void EditTeamWindow::paint (Graphics& g)
{
}

void EditTeamWindow::buttonClicked (Button* button)
{
  //cancel and go back to tournament select screen
  if ( button == &cancel )
  {
    QuidStreamAssistantApplication::getApp().showTeamSelectWindow();
    QuidStreamAssistantApplication::getApp().editTeam = nullptr;
  }
  
  //save entered tournament settings as a .tourn setting file
  //next step, this has to close this window and open the AddTeamsWindow
  else if ( button == &save )
  {
    if ( teamName.isEmpty() )
    {
      AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Please Enter a Team Name",
                        "", "OK");
    }
    else
    {
      
      if ( thisTeam->getTeamName() != Team::defaultTeamName && thisTeam->getTeamName() != teamName.getText() )
      {
        if ( AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
                        "Team Name Changed!",
                        "You have edited the name of this Team. Do you want to overwrite the original version (OK), or make a new Team with the new name (keeping both) (Cancel)?",
                        String("Overwrite Existing"),
                        String("Create New")) )
        {
          //alert window true = overwrite existing
          //but really, we're just going to delete it because
          //all of the info from it we need is already in memory
          const File old ( Team::getTeamsFolder().getChildFile(thisTeam->getTeamName()).withFileExtension(Team::getTeamFileSuffix()) );
          old.deleteFile();
        }
        //no else necessary, create new just means ignore the old file and it'll stick around
      }
      
      for ( int i = 0; i < roster->getNumRows(); i++ )
      {
        thisTeam->addPlayer(roster->getPlayer(i));
      }
      
      //some image input file was given, hopefully it was a real one
      if ( ! logoImage.isEmpty() )
      {
        File image ( logoImage.getText() );
        
        //file path given exists and is an image
        if ( image.existsAsFile() && image.hasFileExtension("jpeg;jpg;png;gif;svg"))
        {
          thisTeam->fillThisSucker(teamName.getText(), abvBox.getText(), image);
          QuidStreamAssistantApplication::getApp().showTeamSelectWindow();
          QuidStreamAssistantApplication::getApp().editTeam = nullptr;
        }
        else
        {
          AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "The Logo File Path Is Not Valid",
                            "Please enter a valid file path to an image (JPEG, PNG, GIF) or leave blank for no logo.", "OK");
        }
      }
      else
      {
        thisTeam->fillThisSucker(teamName.getText(), abvBox.getText());
        QuidStreamAssistantApplication::getApp().showTeamSelectWindow();
        QuidStreamAssistantApplication::getApp().editTeam = nullptr;
      }
    }
  }
  
  //open a file browser to get an image
  else if ( button == &browse )
  {
    ImagePreviewComponent imagePreview;
    imagePreview.setSize (200, 200);
    
    FileChooser fc ("Choose the Tournament's Logo Image",
            File::getSpecialLocation (File::userPicturesDirectory),
            "*.jpg;*.jpeg;*.png;*.gif;*.svg",
            true);
    
    if ( fc.browseForFileToOpen() )
    {
      logoImage.setText(fc.getResult().getFullPathName());
    }
  }
  
  else if ( button == &addPlayer )
  {
    roster->addBlankPlayer();
  }
  
  else if ( button == &removePlayer )
  {
    roster->removeSelected();
  }
  
  else if ( button == &pi )
  {
    roster->givePi();
  }
  else if ( button == &infinity )
  {
    roster->infiniteLove();
  }
}

//==============================================================================

void EditTeamWindow::resized()
{
  Rectangle<int> area(getLocalBounds());
  const int margin = 2;
  const int textBoxHeight = 30;
  const int buttonHeight = 40;
  const int buttonWidth = 60;
  
  Rectangle<int> nameInput (area.removeFromTop(textBoxHeight).reduced(margin));
  team.setBounds(nameInput.removeFromLeft(proportionOfWidth(0.2f)));
  teamName.setBounds(nameInput);
  
  Rectangle<int> abvInput (area.removeFromTop(textBoxHeight).reduced(margin));
  abv.setBounds(abvInput.removeFromLeft(proportionOfWidth(0.2f)));
  abvBox.setBounds(abvInput);
  
  Rectangle<int> logoInput (area.removeFromTop(textBoxHeight).reduced(margin));
  logo.setBounds(logoInput.removeFromLeft(proportionOfWidth(0.2f)));
  browse.setBounds(logoInput.removeFromRight(buttonWidth).reduced(margin));
  logoImage.setBounds(logoInput);
  
//  rounds.setBounds(area.removeFromTop(textBoxHeight).reduced(margin));
  Rectangle<int> numberButtons ( area.removeFromTop(textBoxHeight).reduced(margin).removeFromRight(proportionOfWidth(0.76f)).reduced(margin * 2) );
  pi.setBounds(numberButtons.removeFromLeft(25));
  infinity.setBounds(numberButtons.removeFromLeft(25));
  
  Rectangle<int> saveCancel (area.removeFromBottom(buttonHeight).reduced(margin));
  //saveCancel.removeFromLeft(saveCancel.getWidth() / 2);
  save.setBounds(saveCancel.removeFromRight(proportionOfWidth(0.25f)).reduced(margin));
  cancel.setBounds(saveCancel.removeFromRight(proportionOfWidth(0.25f)).reduced(margin));
  
  Rectangle<int> selectButtons (area.removeFromBottom(buttonHeight).reduced(margin));
  addPlayer.setBounds(selectButtons.removeFromLeft(proportionOfWidth(0.25f)).reduced(margin));
  removePlayer.setBounds(selectButtons.removeFromLeft(proportionOfWidth(0.25f)).reduced(margin));
  
  roster->setBounds(area);

}
