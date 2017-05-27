/*
  ==============================================================================

    LabelAndListComponent.cpp
    Created: 22 May 2017 11:46:33pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "LabelAndListComponent.h"

//==============================================================================
LabelAndListComponent::LabelAndListComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(label);
    addAndMakeVisible(dropDown);
    
    label.attachToComponent(&dropDown, true);

}

LabelAndListComponent::~LabelAndListComponent()
{
}

void LabelAndListComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::lightgrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("LabelAndListComponent", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void LabelAndListComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
