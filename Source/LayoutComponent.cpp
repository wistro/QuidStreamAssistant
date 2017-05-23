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
    : score2(false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(team1);
    addAndMakeVisible(team2);
    addAndMakeVisible(tournament);
    addAndMakeVisible(round);
    addAndMakeVisible(snitchesGetStitches);
    addAndMakeVisible(score1);
    addAndMakeVisible(score2);

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
    
    snitchesGetStitches.setBounds(0, 0, 100, 155);
    score1.setBounds(110, 160, 60, 50);
    score2.setBounds(170, 210, 60, 50);

}
