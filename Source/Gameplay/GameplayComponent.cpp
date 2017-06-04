/*
  ==============================================================================

  GameplayComponent.cpp
  Created: 22 May 2017 11:30:54pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "GameplayComponent.h"

//==============================================================================
GameplayComponent::GameplayComponent()
  : score2(false)
{
  //default locations
  writeHereDir = File::getSpecialLocation(File::userHomeDirectory).getChildFile("QuidStreamAssistant/Overlays/output");
  writeHere = writeHereDir.getChildFile("output.xml");
  
  addAndMakeVisible(tournament);
  addAndMakeVisible(round);
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
  
  snitchesGetStitches.snitchReg.addListener(this);
  snitchesGetStitches.snitchOT.addListener(this);
  snitchesGetStitches.snitch2OT.addListener(this);
  gameTime.gameTime.currentTime.addListener(this);

}

GameplayComponent::~GameplayComponent()
{
  snitchesGetStitches.snitchReg.removeListener(this);
  snitchesGetStitches.snitchOT.removeListener(this);
  snitchesGetStitches.snitch2OT.removeListener(this);
  gameTime.gameTime.currentTime.removeListener(this);
}

//==============================================================================

void GameplayComponent::paint (Graphics& g)
{
  /* This demo code just fills the component's background and
   draws some placeholder text to get you started.

   You should replace everything in this method with your own
   drawing code..
  */

  g.setColour ( getLookAndFeel().findColour(  ResizableWindow::backgroundColourId ));
  g.fillAll();

}

void GameplayComponent::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
  
  snitchesGetStitches.setBounds(0, 0, 100, 155);
  score1.setBounds(110, 160, 70, 50);
  score2.setBounds(200, 160, 70, 50);
  gameTime.setBounds(150, 0, 170, 90);

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
    score1.addSnitchCatch(reg);
  }
  else if (sliderValue == 2)
  {
    score2.addSnitchCatch(reg);
  }
  else if (sliderValue == 1)
  {
    checkSnitchMistakes(reg);
  }
  }
  else if (slider == &snitchesGetStitches.snitchOT)
  {
  if (sliderValue == 0)
  {
    score1.addSnitchCatch(ot);
  }
  else if (sliderValue == 2)
  {
    score2.addSnitchCatch(ot);
  }
  else if (sliderValue == 1)
  {
    checkSnitchMistakes(ot);
  }
  }
  else if (slider == &snitchesGetStitches.snitch2OT)
  {
  if (sliderValue == 0)
  {
    score1.addSnitchCatch(dot);
  }
  else if (sliderValue == 2)
  {
    score2.addSnitchCatch(dot);
  }
  else if (sliderValue == 1)
  {
    checkSnitchMistakes(dot);
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
}

void GameplayComponent::labelTextChanged (Label* label)
{
  if ( label == &gameTime.gameTime.currentTime );
  {
      writeToFile (writeHere);
  }
}

//==============================================================================

//called only when a snitch slider is moved to the 1 position
//this function will check both scores for snitch catches
//corresponding to the correct slider and will then
//remove the snitch marker character and 30 points from that team's total
void GameplayComponent::checkSnitchMistakes(char period)
{
  if ( period == reg)
  {
  if ( score1.getSnitchCatchState(reg) )
  {
    score1.removeSnitchCatch(reg);
  }
  else if ( score2.getSnitchCatchState(reg) )
  {
    score2.removeSnitchCatch(reg);
  }
  }
  else if ( period == ot)
  {
  if ( score1.getSnitchCatchState(ot) )
  {
    score1.removeSnitchCatch(ot);
  }
  else if ( score2.getSnitchCatchState(ot) )
  {
    score2.removeSnitchCatch(ot);
  }
  }
  if ( period == dot)
  {
  if ( score1.getSnitchCatchState(dot) )
  {
    score1.removeSnitchCatch(dot);
  }
  else if ( score2.getSnitchCatchState(dot) )
  {
    score2.removeSnitchCatch(dot);
  }
  }
  
}

void writeToFile (const File& file)
{
  
}
