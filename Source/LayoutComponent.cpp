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
    addAndMakeVisible(gameTime);
    
    snitchesGetStitches.snitchReg.addListener(this);
    snitchesGetStitches.snitchOT.addListener(this);
    snitchesGetStitches.snitch2OT.addListener(this);

}

LayoutComponent::~LayoutComponent()
{
    snitchesGetStitches.snitchReg.removeListener(this);
    snitchesGetStitches.snitchOT.removeListener(this);
    snitchesGetStitches.snitch2OT.removeListener(this);
}

void LayoutComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.setColour (Colours::lightblue);
    g.fillAll();

}

void LayoutComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    snitchesGetStitches.setBounds(0, 0, 100, 155);
    score1.setBounds(110, 160, 70, 50);
    score2.setBounds(200, 160, 70, 50);
    gameTime.setBounds(150, 0, 200, 50);

}

void LayoutComponent::sliderValueChanged (Slider* slider)
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

void LayoutComponent::buttonClicked (Button* button)
{
    
}

void LayoutComponent::labelTextChanged (Label* label)
{
    
}

//called only when a snitch slider is moved to the 1 position
//this function will check both scores for snitch catches
//corresponding to the correct slider and will then
//remove the snitch marker character and 30 points from that team's total
void LayoutComponent::checkSnitchMistakes(char period)
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
