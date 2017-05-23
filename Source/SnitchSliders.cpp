/*
  ==============================================================================

    SnitchSliders.cpp
    Created: 23 May 2017 12:39:24am
    Author:  Jason Rosenberg

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SnitchSliders.h"

//==============================================================================
SnitchSliders::SnitchSliders()
{
    // make the snitchReg, snitchOT and snitch2OT sliders linear with values
    // from 0 to 2 set initially at 1 with no textbox
    // then initalise the Sliders and make them visible.
    snitchReg.setSliderStyle(Slider::LinearHorizontal);
    snitchReg.setRange(0, 2, 1); //value can only be 0, 1, or 2
    snitchReg.setValue(1);
    snitchReg.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(snitchReg);
    
    snitchOT.setSliderStyle(Slider::LinearHorizontal);
    snitchOT.setRange(0, 2, 1); //value can only be 0, 1, or 2
    snitchOT.setValue(1);
    snitchOT.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(snitchOT);
    
    snitch2OT.setSliderStyle(Slider::LinearHorizontal);
    snitch2OT.setRange(0, 2, 1); //value can only be 0, 1, or 2
    snitch2OT.setValue(1);
    snitch2OT.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(snitch2OT);
    
    addAndMakeVisible(regulation);
    addAndMakeVisible(overTime);
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
    Rectangle<int> area = getLocalBounds();
    
    

}
