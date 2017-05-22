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
    // make the snitchReg slider linear with values from 0 to 1
    // set initially at 0.5 with no textbox
    // then initalise the Slider and make it visible.
    snitchReg.setSliderStyle(Slider::LinearBar);
    snitchReg.setRange(0, 1);
    snitchReg.setValue(0.5f);
    snitchReg.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(snitchReg);

}

SnitchSliders::~SnitchSliders()
{
}

void SnitchSliders::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("SnitchSliders", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void SnitchSliders::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
