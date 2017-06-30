/*
  ==============================================================================

    HRSnitchWindow.cpp
    Created: 30 Jun 2017 5:58:50pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "HRSnitchWindow.h"
#include "../Settings/Tournament.h"
#include "../TopLevel/Application.h"

//==============================================================================
HRSnitchWindow::HRSnitchWindow()
{
  save.setButtonText("Save");
  save.addListener(this);
  addAndMakeVisible(save);
  
  cancel.setButtonText("Cancel");
  cancel.addListener(this);
  addAndMakeVisible(cancel);
  
  editHRs.setMultiLine(true);
  editHRs.setReturnKeyStartsNewLine(true);
  editHRs.setText(QuidStreamAssistantApplication::getApp().thisTournament->getHRList().joinIntoString("\n"));
  hr.setText("Head Refs (one per line)", dontSendNotification);
  hr.attachToComponent(&editHRs, false);
  
  addAndMakeVisible(editHRs);
  addAndMakeVisible(hr);
  
  editSnitches.setMultiLine(true);
  editSnitches.setReturnKeyStartsNewLine(true);
  editSnitches.setText(QuidStreamAssistantApplication::getApp().thisTournament->getSnitchList().joinIntoString("\n"));
  snitch.setText("Snitches (one per line)", dontSendNotification);
  snitch.attachToComponent(&editSnitches, false);
  
  addAndMakeVisible(editSnitches);
  addAndMakeVisible(snitch);

}

HRSnitchWindow::~HRSnitchWindow()
{
  save.removeListener(this);
  cancel.removeListener(this);
}

//==============================================================================

void HRSnitchWindow::buttonClicked(Button* button)
{
  if ( button == &cancel )
  {
    QuidStreamAssistantApplication::getApp().hrSnitch = nullptr;
  }
  else if ( button == &save )
  {
    QuidStreamAssistantApplication::getApp().thisTournament->fillHR(editHRs.getText());
    QuidStreamAssistantApplication::getApp().thisTournament->fillSnitches(editSnitches.getText());
    QuidStreamAssistantApplication::getApp().hrSnitch = nullptr;
  }
}

//==============================================================================

void HRSnitchWindow::paint (Graphics& g)
{
}

void HRSnitchWindow::resized()
{
  Rectangle<int> area(getLocalBounds());
  const int margin = 2;
  const int buttonHeight = 40;
  
  Rectangle<int> savecancel (area.removeFromBottom(buttonHeight));
  save.setBounds(savecancel.removeFromRight(proportionOfWidth(0.25f)).reduced(margin));
  cancel.setBounds(savecancel.removeFromRight(proportionOfWidth(0.25f)).reduced(margin));
  
  editHRs.setBounds(area.removeFromTop(proportionOfHeight(0.5f)).reduced(margin));
  editSnitches.setBounds(area.reduced(margin));
}

//==============================================================================
