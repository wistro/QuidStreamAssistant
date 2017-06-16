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

const StringArray GameplayComponent::FLAGS =
{
  "Snitch on Pitch in: ",
  "Seekers Released in: ",
  "1st Handicap in: ",
  "2nd Handicap in: ",
  "3rd Handicap in: "
};

//==============================================================================
GameplayComponent::GameplayComponent() : score2(false), sopTimer(sopInSec)
{
  //default locations
  writeHereDir = File::getSpecialLocation(File::userHomeDirectory).getChildFile("Dropbox/Livestreaming/QuidStreamAssistant/Overlays/output");
  writeHere = writeHereDir.getChildFile("output.xml");
  writeHere.create(); //for now, just make them, we'll let people choose later
  writeHere.getChildFile("tournament.png").deleteFile(); //on init, delete any old tournament logo file that exists
  
  tournamentName = QuidStreamAssistantApplication::getApp().thisTournament->getTournamentName();
  teamList.addArray(QuidStreamAssistantApplication::getApp().thisTournament->getTeamList());
  teamAbvs.addArray(QuidStreamAssistantApplication::getApp().thisTournament->getTeamAbvList());
  
  hasLogoTourn = QuidStreamAssistantApplication::getApp().thisTournament->logo.isValid();
  hasLogoT1 = false;
  hasLogoT2 = false;
  
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
  
//  outputFileBox.setTextToShowWhenEmpty("This will be the file or folder that your Javascript is looking at",
//                                       Colours::black.withAlpha(0.5f));
//  outputFile.setText("Select an Output File", dontSendNotification);
//  outputFile.attachToComponent(&outputFileBox, true);
//  
//  browse.setButtonText("...");
//  browse.setTooltip("Browse for Output File Location");
//  browse.addListener(this);
//  addAndMakeVisible(browse);
  
  gameSetup.setButtonText("Setup\nNext\nGame");
  gameSetup.setTooltip("Creates/Updates logo image files \"tournament.png\", \"t1.png\", and \"t2.png\" for currently selected teams");
  gameSetup.addListener(this);
  addAndMakeVisible(gameSetup);
  
  switchEnds.setButtonText("Switch\nEnds");
  switchEnds.addListener(this);
  addAndMakeVisible(switchEnds);
  
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
  gameTime.stop.addListener(this);
  team1.addListener(this);
  team2.addListener(this);
  
  sopShow.setButtonText("Show Snitch On Pitch/Seekers/Handicap Countdown");
  sopShow.addListener(this);
  addAndMakeVisible(sopShow);
  
  countdownFlag = 0; //0 = Snitch on pitch countdown
  showCorner = false;
  showLowerThird = false;
  showEndScreen = false;
  showSOP = false;
  
  t1logo.setAlpha(0.5f);
  t2logo.setAlpha(0.5f);
  addAndMakeVisible(t1logo);
  addAndMakeVisible(t2logo);

}

GameplayComponent::~GameplayComponent()
{
  snitchesGetStitches.snitchReg.removeListener(this);
  snitchesGetStitches.snitchOT.removeListener(this);
  snitchesGetStitches.snitch2OT.removeListener(this);
  gameTime.gameTime.currentTime.removeListener(this);
  gameTime.stop.removeListener(this);
  corner.removeListener(this);
  lowerthird.removeListener(this);
  endScreen.removeListener(this);
  gameSetup.removeListener(this);
  switchEnds.removeListener(this);
  sopShow.removeListener(this);
  team1.removeListener(this);
  team2.removeListener(this);
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
  team1Side.removeFromBottom(textHeight);
  tournament.setBounds(team1Side.removeFromTop(textHeight).reduced(margin));
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
  
  Rectangle<int> bottomBar ( area.removeFromBottom(textHeight) );
  corner.setBounds(bottomBar.removeFromLeft(fullWidth / 3).reduced(margin));
  lowerthird.setBounds(bottomBar.removeFromLeft(fullWidth / 3).reduced(margin));
  endScreen.setBounds(bottomBar.reduced(margin));
  
//  outputFile.setBounds(bottomBar.removeFromLeft(timeWidth).reduced(margin));
//  browse.setBounds(bottomBar.removeFromRight(buttonWidth).reduced(margin));
//  outputFileBox.setBounds(bottomBar.reduced(margin));
  
  //if the math checks out, these two lines leave a box of width scoresWidth
  //at the centre of the window
  Rectangle<int> logoAreaLeft (area.removeFromLeft( ( fullWidth - scoresWidth ) / 2));
  logoAreaLeft.removeFromBottom(textHeight*2);
  t1logo.setBounds(logoAreaLeft.reduced(margin));
  
  Rectangle<int> logoAreaRight (area.removeFromRight( ( fullWidth - scoresWidth ) / 2));
  Rectangle<int> cornerButton (logoAreaRight.removeFromBottom(textHeight * 2));
  gameSetup.setBounds(cornerButton.removeFromRight(buttonWidth));
  t2logo.setBounds(logoAreaRight.reduced(margin));
  
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
  if ( button == &corner )
  {
    showCorner = corner.getToggleState();
  }
  else if ( button == &lowerthird )
  {
    showLowerThird = lowerthird.getToggleState();
  }
  else if ( button == &endScreen )
  {
    showEndScreen = endScreen.getToggleState();
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
  else if ( button == &sopShow )
  {
    showSOP = sopShow.getToggleState();
  }
  else if ( button == &gameTime.stop )
  {
    snitchesGetStitches.reset();
    score1.setScore(0);
    score2.setScore(0);
  }
}

void GameplayComponent::labelTextChanged (Label* label)
{
  if ( label == &gameTime.gameTime.currentTime );
  {
    sopTimer.operator-=(1);
    if ( sopTimer.inSeconds() == 0 )
    {
      countdownFlag++; //1 = seekers; 2 = 1st HC; 3 = 2nd HC; 4 = 3rd HC
      if ( countdownFlag == 1 )
      {
        sopTimer.operator+=(seekersInSec);
      }
      else
      {
        sopTimer.operator+=(handicapsInSec);
      }
    }
    
    writeToFile ();
  }
}

void GameplayComponent::comboBoxChanged (ComboBox* box)
{
  if ( box == &team1 )
  {
    if ( QuidStreamAssistantApplication::getApp().thisTournament->teams[team1.getSelectedId() - 1]->logo.isValid() )
    {
      t1logo.setImage(QuidStreamAssistantApplication::getApp().thisTournament->teams[team1.getSelectedId() - 1]->logo);
      hasLogoT1 = true;
    }
  }
  else if ( box == &team2 )
  {
    if ( QuidStreamAssistantApplication::getApp().thisTournament->teams[team2.getSelectedId() - 1]->logo.isValid() )
    {
      t2logo.setImage(QuidStreamAssistantApplication::getApp().thisTournament->teams[team2.getSelectedId() - 1]->logo);
      hasLogoT2 = true;
    }
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
  xml->createNewChildElement("t1")->addTextElement(team1.getText());
  xml->createNewChildElement("t2")->addTextElement(team2.getText());
  xml->createNewChildElement("t1short")->addTextElement(teamAbvs[team1.getSelectedId() - 1]);
  xml->createNewChildElement("t2short")->addTextElement(teamAbvs[team2.getSelectedId() - 1]);
  xml->createNewChildElement("gt")->addTextElement(gameTime.gameTime.currentTime.getText());
  xml->createNewChildElement("corner")->addTextElement(showCorner ? "true" : "false");
  xml->createNewChildElement("lowerthird")->addTextElement(showLowerThird ? "true" : "false");
  xml->createNewChildElement("endscreen")->addTextElement(showEndScreen ? "true" : "false");
  xml->createNewChildElement("sop")->addTextElement(showSOP ? "true" : "false");
  xml->createNewChildElement("tournlogo")->addTextElement(hasLogoTourn ? "true" : "false");
  xml->createNewChildElement("t1logo")->addTextElement(hasLogoT1 ? "true" : "false");
  xml->createNewChildElement("t2logo")->addTextElement(hasLogoT2 ? "true" : "false");
  
  if ( showSOP )
  {
    int minutes = floor(sopTimer.inMinutes());
    int seconds = sopTimer.inSeconds() - (minutes * 60);
    
    String min = "";
    String sec = "";
    
    if ( minutes < 10 )
    {
      min = "0" + String(minutes);
    }
    else
    {
      min = String(minutes);
    }
    
    if ( seconds < 10 )
    {
      sec = "0" + String(seconds);
    }
    else
    {
      sec = String(seconds);
    }
    
    xml->createNewChildElement("countdown")->addTextElement(GameplayComponent::FLAGS[countdownFlag] + min + ":" + sec);
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
    if ( ! tourn.existsAsFile() && hasLogoTourn )
    {
      tourn.deleteFile();
      FileOutputStream imageData (tourn);

      if (PNGImageFormat().writeImageToStream (QuidStreamAssistantApplication::getApp().thisTournament->logo, imageData))
        imageData.flush();
    }
    
    if ( hasLogoT1 )
    {
      FileOutputStream imageData (t1);
      
      if (PNGImageFormat().writeImageToStream (QuidStreamAssistantApplication::getApp().\
                                               thisTournament->teams[team1.getSelectedId() - 1]->logo, imageData))
        imageData.flush();
    }
    
    if ( hasLogoT2 )
    {
      FileOutputStream imageData (t2);
      
      if (PNGImageFormat().writeImageToStream (QuidStreamAssistantApplication::getApp().\
                                               thisTournament->teams[team2.getSelectedId() - 1]->logo, imageData))
        imageData.flush();
    }
  }
  
  xml->writeToFile (writeHere, String());
}
