/*
  ==============================================================================

    GameplayComponent.h
    Created: 22 May 2017 11:30:54pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LabelAndListComponent.h"
#include "SnitchSliders.h"
#include "ScoreComponent.h"
#include "GameTimer.h"

//==============================================================================
/*
*/
class GameplayComponent    : public Component,
                            public Slider::Listener,
                            public Button::Listener,
                            public Label::Listener
{
public:
    GameplayComponent();
    ~GameplayComponent();

    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider* slider) override;
    void buttonClicked (Button* button) override;
    void labelTextChanged (Label* label) override;
    
    void checkSnitchMistakes(char period);

private:
    
    static const char reg = 'r';
    static const char ot = 'o';
    static const char dot = 'd';
    
    //team, tournament and round dropdown lists with labels
    LabelAndListComponent team1;
    LabelAndListComponent team2;
    LabelAndListComponent tournament;
    LabelAndListComponent round;
    
    //snitch catch sliders are all in one pretty box here
    SnitchSliders snitchesGetStitches;
    
    ScoreComponent score1;
    ScoreComponent score2;
    
    GameTimer gameTime;
    
    LookAndFeel_V4 defaultLook;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameplayComponent)
};
