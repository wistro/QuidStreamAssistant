/*
  ==============================================================================

    MainComponent.cpp
    Created: 22 May 2017 11:30:54pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent() : defaultLook(CustomLookAndFeel::getLightColourScheme())
{
    setLookAndFeel(&defaultLook);
    
    addAndMakeVisible (gameplay);
    
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    auto r = getLocalBounds();
    
    gameplay.setBounds(r);
}
