/*
  ==============================================================================

  GameplayComponent.cpp
  Created: 22 May 2017 11:30:54pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "../TopLevel/Application.h"
#include "GameplayComponent.h"
#include "ScoreComponent.h"
#include "../Settings/Tournament.h"

//==============================================================================
GameplayComponent::GameplayComponent() : score2(false), sopTimer(sopInSec)
{
  //default locations
  writeHereDir = File::getSpecialLocation(File::userHomeDirectory).getChildFile("QuidStreamAssistant/Overlays/output");
  writeHere = writeHereDir.getChildFile("output.xml");
  writeHere.create(); //for now, just make them, we'll let people choose later
  
  tournamentName = QuidStreamAssistantApplication::getApp().thisTournament->getTournamentName();
  teamList.addArray(QuidStreamAssistantApplication::getApp().thisTournament->getTeamList());
  teamAbvs.addArray(QuidStreamAssistantApplication::getApp().thisTournament->getTeamAbvList());
  
  team1.addItemList(teamList, 1);
  team1.setSelectedId(1);
  teamOne.setText("Team 1", dontSendNotification);
  addAndMakeVisible(team1);
  addAndMakeVisible(teamOne);
  
  teamTwo.setText("Team 2", dontSendNotification);
  team2.addItemList(teamList, 1);
  team2.setSelectedId(1);
  addAndMakeVisible(team2);
  addAndMakeVisible(teamTwo);
  
  tournament.setText(tournamentName, dontSendNotification);
  tournament.setFont(35.0f);
  addAndMakeVisible(tournament);
  
  round.setText("Round:", dontSendNotification);
  roundList.addItemList(QuidStreamAssistantApplication::getApp().thisTournament->getRoundsList(), 1);
  roundList.setSelectedId(1);
  addAndMakeVisible(round);
  addAndMakeVisible(roundList);
  
  addAndMakeVisible(snitchesGetStitches);
  addAndMakeVisible(score1);
  addAndMakeVisible(score2);
  addAndMakeVisible(gameTime);
  
  outputFileBox.setTextToShowWhenEmpty("This will be the file or folder that your Javascript is looking at",
                                       Colours::black.withAlpha(0.5f));
  outputFile.setText("Select an Output File", dontSendNotification);
  outputFile.attachToComponent(&outputFileBox, true);
  
  browse.setButtonText("...");
  browse.setTooltip("Browse for Output File Location");
  browse.addListener(this);
  addAndMakeVisible(browse);
  
  gameSetup.setButtonText("Setup\nNext\nGame");
  gameSetup.setTooltip("Creates/Updates logo image files \"tournament.png\", \"t1.png\", and \"t2.png\" for currently selected teams");
  gameSetup.addListener(this);
  addAndMakeVisible(gameSetup);
  
  switchEnds.setButtonText("Switch\nEnds");
  switchEnds.addListener(this);
  addAndMakeVisible(switchEnds);
  
  useAbvs.setButtonText("Use Team Abbvreviations in Stream");
  addAndMakeVisible(useAbvs);
  
  corner.setButtonText("Show Corner Display");
  corner.addListener(this);
  addAndMakeVisible(corner);
  
  lowerthird.setButtonText("Show Lower Third Display");
  lowerthird.addListener(this);
  addAndMakeVisible(lowerthird);
  
  endScreen.setButtonText("Show End of Game Graphic");
  endScreen.addListener(this);
  addAndMakeVisible(endScreen);
  
  snitchesGetStitches.snitchReg.addListener(this);
  snitchesGetStitches.snitchOT.addListener(this);
  snitchesGetStitches.snitch2OT.addListener(this);
  gameTime.gameTime.currentTime.addListener(this);
  
  sopShow.setButtonText("Show Snitch On Pitch/Handicap Countdown (" + sopTimer.getDescription() + ")");
//  sopShow.addListener(this);
  addAndMakeVisible(sopShow);

}

GameplayComponent::~GameplayComponent()
{
  snitchesGetStitches.snitchReg.removeListener(this);
  snitchesGetStitches.snitchOT.removeListener(this);
  snitchesGetStitches.snitch2OT.removeListener(this);
  gameTime.gameTime.currentTime.removeListener(this);
  corner.removeListener(this);
  lowerthird.removeListener(this);
  endScreen.removeListener(this);
  gameSetup.removeListener(this);
  switchEnds.removeListener(this);
}

//==============================================================================

void GameplayComponent::paint (Graphics& g)
{
}

void GameplayComponent::resized()
{
  const int timeWidth = 170;
  const int textHeight = 40;
  const int buttonWidth = 70;
  const int margin = 4;
  const int scoresWidth = 240;
  const int snitchHeight = 170;
  
  Rectangle<int> area(getLocalBounds());
  
  const int fullWidth = area.getWidth();
  
  Rectangle<int> topBar(area.removeFromTop(textHeight * 3));
  Rectangle<int> team1Side ( topBar.removeFromLeft( (fullWidth - timeWidth) / 2 ));
  tournament.setBounds(team1Side.removeFromTop(textHeight).reduced(margin));
  useAbvs.setBounds(team1Side.removeFromBottom(textHeight).reduced(margin));
  teamOne.setBounds(team1Side.removeFromLeft(buttonWidth).reduced(margin));
  team1.setBounds(team1Side.reduced(margin));

  Rectangle<int> timeDisplay (topBar.removeFromLeft(timeWidth));
  timeDisplay.removeFromBottom(textHeight);
  gameTime.setBounds(timeDisplay);
  
  Rectangle<int> roundBit ( topBar.removeFromTop(textHeight) );
  round.setBounds(roundBit.removeFromLeft(buttonWidth).reduced(margin));
  roundList.setBounds(roundBit.reduced(margin));
  
  sopShow.setBounds(topBar.removeFromBottom(textHeight).reduced(margin));
  teamTwo.setBounds(topBar.removeFromLeft(buttonWidth).reduced(margin));
  team2.setBounds(topBar.reduced(margin));
  
//  outputFile.setBounds(bottomBar.removeFromLeft(timeWidth).reduced(margin));
//  browse.setBounds(bottomBar.removeFromRight(buttonWidth).reduced(margin));
//  outputFileBox.setBounds(bottomBar.reduced(margin));
  
  //if the math checks out, these two lines leave a box of width scoresWidth
  //at the centre of the window
  area.removeFromLeft( ( fullWidth - scoresWidth ) / 2);
  Rectangle<int> bottomBar (area.removeFromRight( ( fullWidth - scoresWidth ) / 2));
  bottomBar = bottomBar.removeFromBottom(textHeight * 2);
  gameSetup.setBounds(bottomBar.removeFromRight(buttonWidth));
  
  snitchesGetStitches.setBounds(area.removeFromBottom(snitchHeight).reduced(margin));
  score1.setBounds(area.removeFromLeft(scoresWidth / 3).reduced(margin));
  switchEnds.setBounds(area.removeFromLeft(scoresWidth / 3).reduced(margin));
  score2.setBounds(area.reduced(margin));
}

//==============================================================================

void GameplayComponent::sliderValueChanged (Slider* slider)
{
  //unless everything is awful, this will be a 0, 1 or 2
  const int sliderValue = slider->getValue();

  
  if (slider == &snitchesGetStitches.snitchReg)
  {
    if (sliderValue == 0)
    {
      score1.addSnitchCatch(ScoreComponent::reg);
    }
    else if (sliderValue == 2)
    {
      score2.addSnitchCatch(ScoreComponent::reg);
    }
    else if (sliderValue == 1)
    {
      checkSnitchMistakes(ScoreComponent::reg);
    }
  }
  else if (slider == &snitchesGetStitches.snitchOT)
  {
    if (sliderValue == 0)
    {
      score1.addSnitchCatch(ScoreComponent::ot);
    }
    else if (sliderValue == 2)
    {
      score2.addSnitchCatch(ScoreComponent::ot);
    }
    else if (sliderValue == 1)
    {
      checkSnitchMistakes(ScoreComponent::ot);
    }
  }
  else if (slider == &snitchesGetStitches.snitch2OT)
  {
    if (sliderValue == 0)
    {
      score1.addSnitchCatch(ScoreComponent::dot);
    }
    else if (sliderValue == 2)
    {
      score2.addSnitchCatch(ScoreComponent::dot);
    }
    else if (sliderValue == 1)
    {
      checkSnitchMistakes(ScoreComponent::dot);
    }
  }
  
}

void GameplayComponent::buttonClicked (Button* button)
{
  //open a file browser to get an image
  if ( button == &browse )
  {
    FileChooser fc ("Choose the output file (.xml) or folder (default output.xml will be used)...",
        File::getCurrentWorkingDirectory(),
        "*", true);
    
    if ( fc.browseForFileToOpen() )
    {
      File f = fc.getResult();
      
      if ( f.existsAsFile() )
      {
      if ( f.hasFileExtension("xml") )
      {
        writeHere = f;
        writeHereDir = writeHere.getParentDirectory();
        writeHere.create();
      }
      else //non xml file selected, use that file's directory and default output file name
      {
        writeHereDir = f.getParentDirectory();
        writeHere = writeHereDir.getChildFile("output.xml");
        writeHere.create();
      }
      }
      else if ( f.isDirectory() )
      {
        writeHereDir = f;
        writeHere = writeHereDir.getChildFile("output.xml");
        writeHere.create();
      }
    }
    outputFileBox.setText(writeHere.getFullPathName());
  }
  else if ( button == &corner )
  {
    //find a way to signal to the JS to show/hide the thing without rewriting an entire XML file
  }
  else if ( button == &lowerthird )
  {
    //same as above
  }
  else if ( button == &endScreen )
  {
    //and again, do the thing!
    //maybe new files that either exist or don't?
  }
  else if ( button == &gameSetup )
  {
    //true means that it will output the image files as well as the xml
    //...in theory
    writeToFile( true );
  }
  else if ( button == &switchEnds )
  {
    int tempTeam = team1.getSelectedId();
    int tempScore = score1.getScore();
    String tempSnitchMarkers = score1.getSnitchMarkers();
    
    team1.setSelectedId(team2.getSelectedId());
    team2.setSelectedId(tempTeam);
    
    score1.setScore(score2.getScore(), score2.getSnitchMarkers());
    score2.setScore(tempScore, tempSnitchMarkers);
    
    if ( snitchesGetStitches.snitchReg.getValue() != 1 )
    {
      //slider.getValue() will either be 0 or 2.
      //if it is 2, we want it to be 0, if it's 0 we want it to be 2.
      //so, we subtract 2 from the value and then take the absolute value of the result
      //giving either 0 or 2
      snitchesGetStitches.snitchReg.setValue(abs(int(snitchesGetStitches.snitchReg.getValue() - 2)), dontSendNotification);
    }
    
    if ( snitchesGetStitches.snitchOT.getValue() != 1 )
    {
      //slider.getValue() will either be 0 or 2.
      //if it is 2, we want it to be 0, if it's 0 we want it to be 2.
      //so, we subtract 2 from the value and then take the absolute value of the result
      //giving either 0 or 2
      snitchesGetStitches.snitchOT.setValue(abs(int(snitchesGetStitches.snitchOT.getValue() - 2)), dontSendNotification);
    }
    
    if ( snitchesGetStitches.snitch2OT.getValue() != 1 )
    {
      //slider.getValue() will either be 0 or 2.
      //if it is 2, we want it to be 0, if it's 0 we want it to be 2.
      //so, we subtract 2 from the value and then take the absolute value of the result
      //giving either 0 or 2
      snitchesGetStitches.snitch2OT.setValue(abs(int(snitchesGetStitches.snitch2OT.getValue() - 2)), dontSendNotification);
    }
  }
}

void GameplayComponent::labelTextChanged (Label* label)
{
  if ( label == &gameTime.gameTime.currentTime );
  {
    writeToFile ();
    sopTimer.operator-=(1);
    sopShow.setButtonText("Show Snitch On Pitch/Handicap Countdown (" + sopTimer.getDescription() + ")");
  }
}

//==============================================================================

//called only when a snitch slider is moved to the 1 position
//this function will check both scores for snitch catches
//corresponding to the correct slider and will then
//remove the snitch marker character and 30 points from that team's total
void GameplayComponent::checkSnitchMistakes(char period)
{
  if ( period == ScoreComponent::reg)
  {
  if ( score1.getSnitchCatchState(ScoreComponent::reg) )
  {
    score1.removeSnitchCatch(ScoreComponent::reg);
  }
  else if ( score2.getSnitchCatchState(ScoreComponent::reg) )
  {
    score2.removeSnitchCatch(ScoreComponent::reg);
  }
  }
  else if ( period == ScoreComponent::ot)
  {
  if ( score1.getSnitchCatchState(ScoreComponent::ot) )
  {
    score1.removeSnitchCatch(ScoreComponent::ot);
  }
  else if ( score2.getSnitchCatchState(ScoreComponent::ot) )
  {
    score2.removeSnitchCatch(ScoreComponent::ot);
  }
  }
  if ( period == ScoreComponent::dot)
  {
  if ( score1.getSnitchCatchState(ScoreComponent::dot) )
  {
    score1.removeSnitchCatch(ScoreComponent::dot);
  }
  else if ( score2.getSnitchCatchState(ScoreComponent::dot) )
  {
    score2.removeSnitchCatch(ScoreComponent::dot);
  }
  }
  
}

void GameplayComponent::writeToFile (bool gameSetup) const
{
  ScopedPointer<XmlElement> xml = new XmlElement ("output");
  
  xml->createNewChildElement("tournament")->addTextElement(tournamentName);
  xml->createNewChildElement("round")->addTextElement(roundList.getText());
  xml->createNewChildElement("s1")->addTextElement(score1.getScoreWithSnitchMarks());
  xml->createNewChildElement("s2")->addTextElement(score2.getScoreWithSnitchMarks());
  xml->createNewChildElement("gt")->addTextElement(gameTime.gameTime.currentTime.getText());
  
  if ( ! useAbvs.getToggleState() )
  {
    xml->createNewChildElement("t1")->addTextElement(team1.getText());
    xml->createNewChildElement("t2")->addTextElement(team2.getText());
  }
  else //use abbreviations, index is (combobox - 1) because combobox indicies start at 1 while stringarray starts at 0
  {
    xml->createNewChildElement("t1")->addTextElement(teamAbvs[team1.getSelectedId() - 1]);
    xml->createNewChildElement("t2")->addTextElement(teamAbvs[team2.getSelectedId() - 1]);
  }

  if ( gameSetup ) //create the logo files in the output directory; files will be called tournament.png, and  t[1|2].png
  {
    const File tourn (writeHereDir.getChildFile("tournament.png"));
    const File t1 (writeHereDir.getChildFile("t1.png"));
    const File t2 (writeHereDir.getChildFile("t2.png"));
    
    //delete files before rewriting them because otherwise weird things happen
    t1.deleteFile();
    t2.deleteFile();
    

    //only output the tournament image file once so it doesn't keep getting rewritten all the time
    if ( ! tourn.existsAsFile() && QuidStreamAssistantApplication::getApp().thisTournament->logo.isValid() )
    {
      tourn.deleteFile();
      FileOutputStream imageData (tourn);

      if (PNGImageFormat().writeImageToStream (QuidStreamAssistantApplication::getApp().thisTournament->logo, imageData))
        imageData.flush();
//      else if (JPEGImageFormat().writeImageToStream(QuidStreamAssistant::getapp().thisTournament->logo, imageData))
//        imageData.flush();
//      else if (GIFImageFormat().writeImageToStream(QuidStreamAssistantApplication::getApp().thisTournament->logo, imageData))
//        imageData.flush();
    }
    
    if ( QuidStreamAssistantApplication::getApp().thisTournament->teams[team1.getSelectedId() - 1]->logo.isValid() )
    {
      FileOutputStream imageData (t1);
      
      if (PNGImageFormat().writeImageToStream (QuidStreamAssistantApplication::getApp().thisTournament->logo, imageData))
        imageData.flush();
//      else if (JPEGImageFormat().writeImageToStream(QuidStreamAssistantApplication::getApp().thisTournament->logo, imageData))
//        imageData.flush();
//      else if (GIFImageFormat().writeImageToStream(QuidStreamAssistantApplication::getApp().thisTournament->logo, imageData))
//        imageData.flush();
    }
    
    if ( QuidStreamAssistantApplication::getApp().thisTournament->teams[team2.getSelectedId() - 1]->logo.isValid() )
    {
      FileOutputStream imageData (t2);
      
      if (PNGImageFormat().writeImageToStream (QuidStreamAssistantApplication::getApp().thisTournament->logo, imageData))
        imageData.flush();
      //      else if (JPEGImageFormat().writeImageToStream(QuidStreamAssistantApplication::getApp().thisTournament->logo, imageData))
      //        imageData.flush();
      //      else if (GIFImageFormat().writeImageToStream(QuidStreamAssistantApplication::getApp().thisTournament->logo, imageData))
      //        imageData.flush();
    }
  }
  
  xml->writeToFile (writeHere, String());
}
