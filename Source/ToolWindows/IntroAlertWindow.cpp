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
  select.setTooltip("This may take a while if your tournament has a logo saved. Don't panic.");
  select.addListener(this);
  addAndMakeVisible(select);
  
  overlayFolderIntro.setText("Current Overlays Folder: ", dontSendNotification);
  addAndMakeVisible(overlayFolderIntro);
  
  if ( getGlobalProperties().containsKey( StoredSettings::overlaysSettingName ) )
  {
    currOverlaysFile.setText(getGlobalProperties().getValue(StoredSettings::overlaysSettingName), dontSendNotification);
  }
  else //default value
  {
    currOverlaysFile.setText(File::getSpecialLocation(File::userHomeDirectory).\
                                    getChildFile("QuidStreamAssistant/Overlays").getFullPathName(), dontSendNotification);
    getGlobalProperties().setValue(StoredSettings::overlaysSettingName, currOverlaysFile.getText());
  }
  
  addAndMakeVisible(currOverlaysFile);
  
  changeOverlaysLoc.setButtonText("Change Overlay Folder \nWARNING IN MOUSEOVER");
  changeOverlaysLoc.setTooltip("Click here to change the directory where your JS, CSS and HTML are kept. \
                               Current value is above\nWARNING: WILL OVERWRITE THE SELECTED FOLDER IF WE HAVE OUTPUT OVERLAY FILES ALREADY AT ANY TIME");
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
  overlayFolderIntro.setBounds(overlayChange.removeFromTop(buttonHeight / 2).reduced(margin));
  currOverlaysFile.setBounds(overlayChange.removeFromTop(buttonHeight / 2).reduced(margin));
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
    //if the user has never output the overlays files, ask if they want to
    //gives a "never ask again" option for those who want to make their own
    if ( ! getGlobalProperties().getBoolValue("overlays") )
    {
      #if JUCE_MODAL_LOOPS_PERMITTED
      AlertWindow w("It appears you haven't created the overlay files.", 
                  "Okay will create the default files in your Output Directory. Feel free to modify them as you wish. WARNING: THIS WILL TAKE A WHILE. MAYBE GET A CUP OF COFFEE OR SOMETHING. BUT DON'T PANIC OR CLOSE THE APP.", 
                  AlertWindow::QuestionIcon);

      w.addButton("OK", 1, KeyPress(KeyPress::returnKey, 0, 0));
      w.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
      w.addButton("Never Ask Again", -1);

      int catchButton = w.runModalLoop();

      if (catchButton == 1) // is they picked 'ok'
      {
        outputOverlays();
        getGlobalProperties().setValue("overlays", "true");
      }
      else if (catchButton == -1) // is they picked 'never again'
      {
        getGlobalProperties().setValue("overlays", "NEVER");
      }
      #endif
    }
    //if a tournament has been selected, we're going to go straight to the streaming window
    //other windows can be accessed through the menus
    if ( tournamentList.getSelectedId() > 0 )
    {
      QuidStreamAssistantApplication::getApp().thisTournament->readFromFile(Tournament::getTournamentsFolder().getChildFile(tournamentList.getText()).withFileExtension(Tournament::getTournamentFileSuffix()));
      
      getGlobalProperties().setValue("lastUsedTournament", tournamentList.getSelectedId());
      
      MainAppWindow::getMainAppWindow()->closeButtonPressed();
      QuidStreamAssistantApplication::getApp().showStreamingWindow();
    }
    //if no tournament has been selected, then we have to create a new one so we go to the edit tournament window
    else
    {
      MainAppWindow::getMainAppWindow()->closeButtonPressed();
      QuidStreamAssistantApplication::getApp().showEditTournamentWindow();
    }
    
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
      
      chosenDirectory.createDirectory();

      //if we've already output the overlay files, and the directory is changed
      //we're going to move everything from the old directory to the new one
      if (chosenDirectory != currOverlaysFile.getText() && getGlobalProperties().getValue("overlays") == "true")
      {
        File prevOverlays (currOverlaysFile.getText());

        if (chosenDirectory.getFileName() == prevOverlays.getFileName())
        {
          //returns true if successful
          if (prevOverlays.copyDirectoryTo(chosenDirectory.getParentDirectory()))
          {
            prevOverlays.deleteRecursively(); //once successfully copied, delete the original
          }
        }
        else //new folder has a different name
        {
          //this is really messy, but here's what it does:
          //Step 1: create a copy of the old Overlays folder in the same directory that it is currently in,
          //    but with the FOLDER NAME CHANGED to the NEW overlays directory name
          //Step 2: if Step 1 succeeded, copy the newly renamed Overlays folder to the new location
          //Step 3: if Step 2 succeeded, delete BOTH old versions (the original and the temp copy made in Step 1
          if (prevOverlays.copyDirectoryTo(prevOverlays.getParentDirectory().getChildFile(chosenDirectory.getFileName())))
          {
            if (prevOverlays.getParentDirectory().getChildFile(chosenDirectory.getFileName()).copyDirectoryTo(chosenDirectory.getParentDirectory()))
            {
              prevOverlays.getParentDirectory().getChildFile(chosenDirectory.getFileName()).deleteRecursively();
              prevOverlays.deleteRecursively();
            }
          }
        }
      }

      getGlobalProperties().setValue(StoredSettings::overlaysSettingName, chosenDirectory.getFullPathName());
      currOverlaysFile.setText(chosenDirectory.getFullPathName(), dontSendNotification);
    }
  }
}

//==============================================================================

void IntroAlertWindow::outputOverlays()
{
  File overlaysFolder (currOverlaysFile.getText());
  overlaysFolder.createDirectory();

  File icons (overlaysFolder.getChildFile("icons"));
  icons.createDirectory();

  File scripts (overlaysFolder.getChildFile("scripts"));
  scripts.createDirectory();

  FileOutputStream red (icons.getChildFile("redcard.png"));
  red.write(BinaryData::redcard_png, BinaryData::redcard_pngSize);
  red.flush();

  FileOutputStream yellow (icons.getChildFile("yellowcard.png"));
  yellow.write(BinaryData::yellowcard_png, BinaryData::yellowcard_pngSize);
  yellow.flush();

  FileOutputStream doubleYellow (icons.getChildFile("doubleyellow.png"));
  doubleYellow.write(BinaryData::doubleyellow_png, BinaryData::doubleyellow_pngSize);
  doubleYellow.flush();

  FileOutputStream blue (icons.getChildFile("bluecard.png"));
  blue.write(BinaryData::bluecard_png, BinaryData::bluecard_pngSize);
  blue.flush();

  FileOutputStream quaffle (icons.getChildFile("quaffle.png"));
  quaffle.write(BinaryData::quaffle_png, BinaryData::quaffle_pngSize);
  quaffle.flush();

  FileOutputStream html (overlaysFolder.getChildFile("inGame.html"));
  FileOutputStream global (scripts.getChildFile("_global.css"));
  FileOutputStream corner (scripts.getChildFile("corner.css"));
  FileOutputStream end (scripts.getChildFile("endscreen.css"));
  FileOutputStream lower (scripts.getChildFile("lowerthird.css"));
  FileOutputStream js (scripts.getChildFile("inGame.js"));
  FileOutputStream jq (scripts.getChildFile("jquery.min.js"));

  #if JUCE_WINDOWS
  html.write(BinaryData::wininGame_html, BinaryData::wininGame_htmlSize);
  html.flush();
  global.write(BinaryData::win_global_css, BinaryData::win_global_cssSize);
  global.flush();
  corner.write(BinaryData::wincorner_css, BinaryData::wincorner_cssSize);
  corner.flush();
  end.write(BinaryData::winendscreen_css, BinaryData::winendscreen_cssSize);
  end.flush();
  lower.write(BinaryData::lowerthird_css, BinaryData::lowerthird_cssSize);
  lower.flush();
  js.write(BinaryData::inGame_js, BinaryData::inGame_jsSize);
  js.flush();
  jq.write(BinaryData::jquery_min_js, BinaryData::jquery_min_jsSize);
  jq.flush();

  #else
  html.write(BinaryData::inGame_html, BinaryData::inGame_htmlSize);
  html.flush();
  global.write(BinaryData::_global_css, BinaryData::_global_cssSize);
  global.flush();
  corner.write(BinaryData::corner_css, BinaryData::corner_cssSize);
  corner.flush();
  end.write(BinaryData::endscreen_css, BinaryData::endscreen_cssSize);
  end.flush();
  lower.write(BinaryData::lowerthird_css, BinaryData::lowerthird_cssSize);
  lower.flush();
  js.write(BinaryData::inGame_js, BinaryData::inGame_jsSize);
  js.flush();
  jq.write(BinaryData::jquery_min_js, BinaryData::jquery_min_jsSize);
  jq.flush();
  #endif

}
