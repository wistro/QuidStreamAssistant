/*
  ==============================================================================

  SnitchSliders.cpp
  Created: 23 May 2017 12:39:24am
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "SnitchSliders.h"

//==============================================================================
SnitchSliders::SnitchSliders()
{
  // make the snitchReg, snitchOT and snitch2OT sliders linear with values
  // from 0 to 2 set initially at 1 with no textbox
  // then initalise the Sliders and make them visible.
  
  const float textSize = 16.0;
  
  snitchReg.setSliderStyle(Slider::LinearHorizontal);
  snitchReg.setRange(0, 2, 1); //value can only be 0, 1, or 2
  snitchReg.setValue(1, dontSendNotification);
  snitchReg.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  addAndMakeVisible(snitchReg);
  
  snitchOT.setSliderStyle(Slider::LinearHorizontal);
  snitchOT.setRange(0, 2, 1); //value can only be 0, 1, or 2
  snitchOT.setValue(1, dontSendNotification);
  snitchOT.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  addAndMakeVisible(snitchOT);
  
  snitch2OT.setSliderStyle(Slider::LinearHorizontal);
  snitch2OT.setRange(0, 2, 1); //value can only be 0, 1, or 2
  snitch2OT.setValue(1, dontSendNotification);
  snitch2OT.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  addAndMakeVisible(snitch2OT);
  
  regulation.setFont(textSize);
  regulation.setJustificationType(Justification::centredBottom);
  regulation.setText("Snitch Catch", dontSendNotification);
  addAndMakeVisible(regulation);
  
  overTime.setFont(textSize);
  overTime.setJustificationType(Justification::centredBottom);
  overTime.setText("OT Snitch", dontSendNotification);
  addAndMakeVisible(overTime);
  
  doubleOverTime.setFont(textSize);
  doubleOverTime.setJustificationType(Justification::centredBottom);
  doubleOverTime.setText("2OT Snitch", dontSendNotification);
  addAndMakeVisible(doubleOverTime);

}

SnitchSliders::~SnitchSliders()
{
}

void SnitchSliders::paint (Graphics& g)
{
}

void SnitchSliders::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
  
  const int border = 2;
  const int sliderHeight = 30;
  const int textHeight = 18;
  
  Rectangle<int> area (getLocalBounds());
  
  regulation.setBounds(area.removeFromTop(textHeight));
  snitchReg.setBounds(area.removeFromTop(sliderHeight).reduced(border));
  overTime.setBounds(area.removeFromTop(textHeight));
  snitchOT.setBounds(area.removeFromTop(sliderHeight).reduced(border));
  doubleOverTime.setBounds(area.removeFromTop(textHeight));
  snitch2OT.setBounds(area.removeFromTop(sliderHeight));
  
}

void SnitchSliders::reset()
{
  snitchReg.setValue(1, dontSendNotification);
  snitchOT.setValue(1, dontSendNotification);
  snitch2OT.setValue(1, dontSendNotification);
}
