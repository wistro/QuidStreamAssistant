/*
  ==============================================================================

    ScoreComponent.h
    Created: 23 May 2017 10:41:28am
    Author:  Willow Rosenberg
    
    displays a textbox with the current score inside with two arrowButtons on
    one (or the other) side of the box allowing the score to be increased or
    decreased.
 
    Defaults to buttons displayed to the left of the textbox, but can be switched
    to the right when initialised in the constructor.
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ScoreComponent    : public Component,
                            public Button::Listener
{
public:
    
    Label showScore;
    
    ScoreComponent(bool areButtonsOnLeft = true);
    ~ScoreComponent();

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked (Button* button) override;
    
    String getScoreWithSnitchMarks();
    int getScore();
    void addSnitchCatch(char period = 'r');

private:
    
    int score;
    String snitchMarkers;
    
    bool leftButtons;
    
    ArrowButton increase;
    ArrowButton decrease;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScoreComponent)
};
