/*
  ==============================================================================

    layoutComponent.cpp
    Created: 22 May 2017 11:30:54pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LayoutComponent.h"

//==============================================================================
LayoutComponent::LayoutComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(teamOne);
    addAndMakeVisible(teamTwo);
    addAndMakeVisible(tournament);
    addAndMakeVisible(round);
    addAndMakeVisible(snitchesGetStitches);

}

LayoutComponent::~LayoutComponent()
{
}

void LayoutComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);

}

void LayoutComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
