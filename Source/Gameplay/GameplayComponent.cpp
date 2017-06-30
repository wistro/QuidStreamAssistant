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

const String GameplayComponent::goalFlag = "{{GG}}";
const String GameplayComponent::blueFlag = "{{BC}}";
const String GameplayComponent::yellowFlag = "{{YC}}";
const String GameplayComponent::redFlag = "{{RC}}";

//==============================================================================
GameplayComponent::GameplayComponent() :
          writeHereDir(getGlobalProperties().getValue(StoredSettings::overlaysSettingName) + "/output"),
          score2(false), sopTimer(sopInSec)
{
  //locations set on opening screen
  writeHere = writeHereDir.getChildFile("output.xml");
  writeHere.create();
  isFirstGame = true; //on init, we will delete old tournament picture file and write new one (if it exists)
  
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
  
  gameSetup.setButtonText("Setup\nFirst\nGame");
  gameSetup.setTooltip("Creates tournament image file \"tournament.png\"");
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
  
  hrList.setEditableText(true);
  hrList.addItemList(QuidStreamAssistantApplication::getApp().thisTournament->getHRList(), 1);
  addAndMakeVisible(hrList);
  
  hr.setText("HR", dontSendNotification);
  addAndMakeVisible(hr);
  
  snitchList.setEditableText(true);
  snitchList.addItemList(QuidStreamAssistantApplication::getApp().thisTournament->getSnitchList(), 1);
  addAndMakeVisible(snitchList);
  
  snitch.setText("Snitch", dontSendNotification);
  addAndMakeVisible(snitch);
  
  streamer.setEditable(true);
  streamer.setText( getGlobalProperties().containsKey( StoredSettings::streamerSettingName ) ? getGlobalProperties().getValue(StoredSettings::streamerSettingName) : "Streamer Name (click to change)", dontSendNotification);
  streamer.setJustificationType(Justification::centred);
  addAndMakeVisible(streamer);
  
  snitchesGetStitches.snitchReg.addListener(this);
  snitchesGetStitches.snitchOT.addListener(this);
  snitchesGetStitches.snitch2OT.addListener(this);
  gameTime.gameTime.currentTime.addListener(this);
  gameTime.stop.addListener(this);
  gameTime.playPause.addListener(this);
  team1.addListener(this);
  team2.addListener(this);
  streamer.addListener(this);
  
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
  
  team1Stats = {};
  score1.increase.addListener(this);
  score1.decrease.addListener(this);
  
  team2Stats = {};
  score2.increase.addListener(this);
  score2.decrease.addListener(this);
  
  blue1.setButtonText("Blue Card");
  blue1.addListener(this);
  addAndMakeVisible(blue1);
  
  yellow1.setButtonText("Yellow Card");
  yellow1.addListener(this);
  addAndMakeVisible(yellow1);
  
  red1.setButtonText("Red Card");
  red1.addListener(this);
  addAndMakeVisible(red1);
  
  blue2.setButtonText("Blue Card");
  blue2.addListener(this);
  addAndMakeVisible(blue2);
  
  yellow2.setButtonText("Yellow Card");
  yellow2.addListener(this);
  addAndMakeVisible(yellow2);
  
  red2.setButtonText("Red Card");
  red2.addListener(this);
  addAndMakeVisible(red2);

}

GameplayComponent::~GameplayComponent()
{
  snitchesGetStitches.snitchReg.removeListener(this);
  snitchesGetStitches.snitchOT.removeListener(this);
  snitchesGetStitches.snitch2OT.removeListener(this);
  gameTime.gameTime.currentTime.removeListener(this);
  gameTime.stop.removeListener(this);
  gameTime.playPause.removeListener(this);
  corner.removeListener(this);
  lowerthird.removeListener(this);
  endScreen.removeListener(this);
  gameSetup.removeListener(this);
  switchEnds.removeListener(this);
  sopShow.removeListener(this);
  team1.removeListener(this);
  team2.removeListener(this);
  streamer.removeListener(this);
  blue1.removeListener(this);
  yellow1.removeListener(this);
  red1.removeListener(this);
  blue2.removeListener(this);
  yellow2.removeListener(this);
  red2.removeListener(this);
  score1.increase.removeListener(this);
  score1.decrease.removeListener(this);
  score2.increase.removeListener(this);
  score2.decrease.removeListener(this);
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
  
  Rectangle<int> bottomBar ( area.removeFromBottom(textHeight * 2 ));
  gameSetup.setBounds(bottomBar.removeFromRight(buttonWidth).reduced(margin));
  
  Rectangle<int> checkBoxs ( bottomBar.removeFromBottom(textHeight) );
  corner.setBounds(checkBoxs.removeFromLeft(fullWidth / 3).reduced(margin));
  lowerthird.setBounds(checkBoxs.removeFromLeft(fullWidth / 3).reduced(margin));
  endScreen.setBounds(checkBoxs.reduced(margin));
  
  hr.setBounds(bottomBar.removeFromLeft(textHeight).reduced(margin));
  hrList.setBounds(bottomBar.removeFromLeft((fullWidth / 3) - textHeight).reduced(margin));
  snitch.setBounds(bottomBar.removeFromLeft(textHeight*2).reduced(margin));
  snitchList.setBounds(bottomBar.removeFromLeft((fullWidth / 3) - (textHeight * 2)).reduced(margin));
  streamer.setBounds(bottomBar.reduced(margin));
  
  Rectangle<int> logoAreaLeft (area.removeFromLeft( ( fullWidth - scoresWidth ) / 2));
  logoAreaLeft.removeFromBottom(textHeight);
  
  Rectangle<int> cardbuttons1 (logoAreaLeft.removeFromTop(textHeight));
  blue1.setBounds(cardbuttons1.removeFromLeft(proportionOfWidth(0.333f)).reduced(margin));
  yellow1.setBounds(cardbuttons1.removeFromLeft(proportionOfWidth(0.333f)).reduced(margin));
  red1.setBounds(cardbuttons1.removeFromLeft(proportionOfWidth(0.333f)).reduced(margin));
  
  t1logo.setBounds(logoAreaLeft.reduced(margin));
  
  Rectangle<int> logoAreaRight (area.removeFromRight( ( fullWidth - scoresWidth ) / 2));
  logoAreaRight.removeFromBottom(textHeight);
  
  Rectangle<int> cardbuttons2 (logoAreaRight.removeFromTop(textHeight));
  blue2.setBounds(cardbuttons2.removeFromLeft(proportionOfWidth(0.333f)).reduced(margin));
  yellow2.setBounds(cardbuttons2.removeFromLeft(proportionOfWidth(0.333f)).reduced(margin));
  red2.setBounds(cardbuttons2.removeFromLeft(proportionOfWidth(0.333f)).reduced(margin));
  
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
  bool tempCorner, tempLower, tempEnd, tempL1, tempL2;
  
  if ( button == &corner )
  {
    showCorner = corner.getToggleState();
    writeToFile();
  }
  else if ( button == &lowerthird )
  {
    showLowerThird = lowerthird.getToggleState();
    writeToFile();
  }
  else if ( button == &endScreen )
  {
    showEndScreen = endScreen.getToggleState();
    writeToFile();
  }
  else if ( button == &gameSetup )
  {
    tempCorner = showCorner;
    tempLower = showLowerThird;
    tempEnd = showEndScreen;
    tempL1 = hasLogoT1;
    tempL2 = hasLogoT2;
    
    //make all the things disappear before we update stuff
    //also push the stop button to reset scores, sliders and timer
    showCorner = false;
    showLowerThird = false;
    showEndScreen = false;
    hasLogoT1 = false;
    hasLogoT2 = false;
    gameTime.stop.triggerClick();
    
    //true means that it will output the image files as well as the xml
    //...in theory
    writeToFile( true );
    
    //set these back how they were
    showCorner = tempCorner;
    showLowerThird = tempLower;
    showEndScreen = tempEnd;
    hasLogoT1 = tempL1;
    hasLogoT2 = tempL2;
  }
  else if ( button == &switchEnds )
  {
    int tempTeam = team1.getSelectedId();
    int tempScore = score1.getScore();
    String tempSnitchMarkers = score1.getSnitchMarkers();
    
    //save current values
    tempCorner = showCorner;
    tempLower = showLowerThird;
    tempEnd = showEndScreen;
    tempL1 = hasLogoT1;
    tempL2 = hasLogoT2;
    
    //remove all displays and logos
    showCorner = false;
    showLowerThird = false;
    showEndScreen = false;
    hasLogoT1 = false;
    hasLogoT2 = false;
    
    writeToFile();
    
    //restore prev values
    showCorner = tempCorner;
    showLowerThird = tempLower;
    showEndScreen = tempEnd;
    hasLogoT1 = tempL1;
    hasLogoT2 = tempL2;
    
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
    
    //reset countdown flag to 0 (which means SOP)
    //reset sopTimer to 17min (+1 second)
    countdownFlag = 0;
    sopTimer.seconds(sopInSec);
  }
  else if ( button == &gameTime.playPause )
  {
    writeToFile();
  }
  else if ( button == &score1.increase )
  {
    #if JUCE_MODAL_LOOPS_PERMITTED
    AlertWindow w ("Which Player Scored?", "", AlertWindow::QuestionIcon);
    int timestamp = gameTime.gameTime.timer.inMinutes();
    
    w.addComboBox ("player",
                   QuidStreamAssistantApplication::getApp().thisTournament->teams[team1.getSelectedId() - 1]->getRoster(),
                   team1.getText() + " Roster");
    
    w.addButton ("OK",     1, KeyPress (KeyPress::returnKey, 0, 0));
    w.addButton ("Cancel", 0, KeyPress (KeyPress::escapeKey, 0, 0));
    
    if (w.runModalLoop() != 0) // is they picked 'ok'
    {
      // this is the item they chose in the drop-down list..
      const String player = w.getComboBoxComponent ("player")->getText();
      team1Stats.add(goalFlag + " " + player + " (" + String(timestamp) + "')" );
    }
    else
    {
      team1Stats.add(goalFlag + " (" + String(timestamp) + "')" );
    }
    
    indexOfGoal1 = team1Stats.size();
    #endif
  }
  else if ( button == &score1.decrease )
  {
    bool keepLooking = true;
    int i = team1Stats.size();
    
    while (keepLooking && i > 0)
    {
      if ( team1Stats[i].contains(goalFlag) )
      {
        team1Stats.remove(i);
        keepLooking = false;
      }
      else
      {
        i--;
      }
      indexOfGoal1 = -1;
    }
  }
  else if ( button == &score2.increase )
  {
    #if JUCE_MODAL_LOOPS_PERMITTED
    AlertWindow w ("Which Player Scored?", "", AlertWindow::QuestionIcon);
    int timestamp = gameTime.gameTime.timer.inMinutes();
    
    w.addComboBox ("player",
                   QuidStreamAssistantApplication::getApp().thisTournament->teams[team2.getSelectedId() - 1]->getRoster(),
                   team2.getText() + " Roster");
    
    w.addButton ("OK",     1, KeyPress (KeyPress::returnKey, 0, 0));
    w.addButton ("Cancel", 0, KeyPress (KeyPress::escapeKey, 0, 0));
    
    if (w.runModalLoop() != 0) // is they picked 'ok'
    {
      // this is the item they chose in the drop-down list..
      const String player = w.getComboBoxComponent ("player")->getText();
      team2Stats.add( player + " (" + String(timestamp) + "') " + goalFlag );
    }
    else
    {
      team2Stats.add( "(" + String(timestamp) + "') " + goalFlag );
    }
    
    indexOfGoal2 = team2Stats.size();
    #endif
  }
  else if ( button == &score2.decrease )
  {
    bool keepLooking = true;
    int i = team2Stats.size();
    
    while (keepLooking && i > 0)
    {
      if ( team2Stats[i].contains(goalFlag) )
      {
        team2Stats.remove(i);
        keepLooking = false;
      }
      else
      {
        i--;
      }
    }
    indexOfGoal2 = -1;
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
  if ( label == &streamer )
  {
    getGlobalProperties().setValue( StoredSettings::streamerSettingName, streamer.getText() );
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

void GameplayComponent::writeToFile (bool gameSetup)
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
  xml->createNewChildElement("hr")->addTextElement(hrList.getText());
  xml->createNewChildElement("snitch")->addTextElement(snitchList.getText());
  xml->createNewChildElement("streamer")->addTextElement(streamer.getText());
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
  
  if ( indexOfGoal1 != -1 )
  {
    xml->createNewChildElement("g1")->addTextElement(team1Stats[indexOfGoal1]);
  }
  
  if ( indexOfGoal2 != -1 )
  {
    xml->createNewChildElement("g2")->addTextElement(team2Stats[indexOfGoal2]);
  }
  
  if ( gameSetup ) //create the logo files in the output directory; files will be called tournament.png, and  t[1|2].png
  {
    const File tourn (writeHereDir.getChildFile("tournament.png"));

    //only output the tournament image file once so it doesn't keep getting rewritten all the time
    if ( isFirstGame )
    {
      tourn.deleteFile();
      isFirstGame = false;
      
      if ( hasLogoTourn )
      {
        FileOutputStream imageData (tourn);

        if (PNGImageFormat().writeImageToStream (QuidStreamAssistantApplication::getApp().\
                                                 thisTournament->logo, imageData))
        {
          imageData.flush();
        }
      }
    }
  }
  
  xml->writeToFile (writeHere, String());
}
