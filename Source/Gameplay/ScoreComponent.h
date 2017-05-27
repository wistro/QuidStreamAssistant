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

#include "../Settings/OSDependencyThings.h"

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
    bool getSnitchCatchState(char period = 'r');
    
    void addSnitchCatch(char period = 'r');
    void removeSnitchCatch(char period = 'r');

private:
    
    int score;
    String snitchMarkers;
    static const String regMarker;
    static const String otMarker;
    static const String dotMarker;
    
    static const char reg = 'r';
    static const char ot = 'o';
    static const char dot = 'd';
    
    bool leftButtons;
    
    //flags for snitch catches instead of messing with character comparisons
    bool caughtR;
    bool caughtO;
    bool caught2O;
    
    ArrowButton increase;
    ArrowButton decrease;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScoreComponent)
};
