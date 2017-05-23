/*
  ==============================================================================

    layoutComponent.h
    Created: 22 May 2017 11:30:54pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LabelAndListComponent.h"
#include "SnitchSliders.h"
#include "ScoreComponent.h"

//==============================================================================
/*
*/
class LayoutComponent    : public Component
{
public:
    LayoutComponent();
    ~LayoutComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    //team, tournament and round dropdown lists with labels
    LabelAndListComponent team1;
    LabelAndListComponent team2;
    LabelAndListComponent tournament;
    LabelAndListComponent round;
    
    //snitch catch sliders are all in one pretty box here
    SnitchSliders snitchesGetStitches;
    
    ScoreComponent score1;
    ScoreComponent score2;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LayoutComponent)
};
