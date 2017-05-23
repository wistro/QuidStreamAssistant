/*
  ==============================================================================

    ScoreComponent.cpp
    Created: 23 May 2017 10:41:28am
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScoreComponent.h"

const String ScoreComponent::regMarker = "*";
const String ScoreComponent::otMarker = "^";
const String ScoreComponent::dotMarker = "$";

//==============================================================================
ScoreComponent::ScoreComponent(bool areButtonsOnLeft)
    : increase("increase", 0.75f, Colours::black),
      decrease("decrease", 0.25f, Colours::black)
{
    score = 0;
    snitchMarkers = "";
    caughtR = false;
    caughtO = false;
    caught2O = false;
    
    leftButtons = areButtonsOnLeft;
    
    addAndMakeVisible(increase);
    increase.addListener(this);
    
    addAndMakeVisible(decrease);
    decrease.addListener(this);
    
    showScore.setText(String(score), dontSendNotification);
    showScore.setColour(Label::backgroundColourId, Colours::white);
    showScore.setColour(Label::textColourId, Colours::black);
    showScore.setJustificationType(Justification::centred);
    showScore.setFont(20.0f);
    addAndMakeVisible(showScore);
    
    

}

ScoreComponent::~ScoreComponent()
{
    increase.removeListener(this);
    decrease.removeListener(this);
}

void ScoreComponent::paint (Graphics& g)
{
   
}

void ScoreComponent::resized()
{
    Rectangle<int> area (getLocalBounds());
    const int buttonWidth = 15;
    const int margin = 2;
    
    if (leftButtons)
    {
        Rectangle<int> buttons (area.removeFromLeft(buttonWidth).reduced(margin));
        increase.setBounds( buttons.removeFromTop( buttons.getHeight() / 2 ));
        decrease.setBounds( buttons.removeFromTop( buttons.getHeight() ));
        showScore.setBounds( area );
    }
    else
    {
        Rectangle<int> buttons (area.removeFromRight(buttonWidth).reduced(margin));
        increase.setBounds( buttons.removeFromTop( buttons.getHeight() / 2 ));
        decrease.setBounds( buttons.removeFromTop( buttons.getHeight() ));
        showScore.setBounds( area );
    }

}

void ScoreComponent::buttonClicked (Button* button)
{
    if (button == &increase)
    {
        score += 10;
        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
    else if (button == &decrease)
    {
        //negative scores are not allowed,
        //if score <= 0, nothing happens
        //when decrease button pressed
        if ( score > 0 )
        {
            score -= 10;
            showScore.setText(String(score) + snitchMarkers, sendNotification);
        }
    }

}

String ScoreComponent::getScoreWithSnitchMarks()
{
    return showScore.getText();
}

//returns true if the corresponding snitch catch flag is true
//otherwise returns false
//if sent an invalid period marker (not 'r', 'o' or 'd') returns false
bool ScoreComponent::getSnitchCatchState(char period)
{
    bool caught = false;
    period = tolower(period); //make sure it's lowercase
    
    if ( period == reg ) //regulation time
    {
        caught = caughtR;
    }
    else if ( period == ot ) //overtime
    {
        caught = caughtO;
    }
    else if ( period == dot )
    {
        caught = caught2O;
    }

    return caught;
}

int ScoreComponent::getScore()
{
    return score;
}

void ScoreComponent::addSnitchCatch(char period)
{
    period = tolower(period); //make sure it's lowercase
    
    if ( period == reg ) //regulation time
    {
        score += 30;
        snitchMarkers.append(regMarker, 1);
        caughtR = true;
        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
    else if ( period == ot ) //overtime
    {
        score += 30;
        snitchMarkers.append(otMarker, 1);
        caughtO = true;
        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
    else if ( period == dot )
    {
        score += 30;
        snitchMarkers.append(dotMarker, 1);
        caught2O = true;
        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
}

void ScoreComponent::removeSnitchCatch(char period)
{
    period = tolower(period); //make sure it's lowercase
    
    if ( period == reg ) //regulation time
    {
        score -= 30;
        caughtR = false;
        snitchMarkers = "";
        
        if ( caughtO )
            if ( caught2O )
            {
                snitchMarkers = otMarker + dotMarker;
            }
            else
            {
                snitchMarkers = otMarker;
            }
        else if ( caught2O )
        {
            snitchMarkers = dotMarker;
        }
        
        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
    else if ( period == ot ) //overtime
    {
        score -= 30;
        caughtO = false;
        snitchMarkers = "";
        
        if ( caughtR )
            if ( caught2O )
            {
                snitchMarkers = regMarker + dotMarker;
            }
            else
            {
                snitchMarkers = regMarker;
            }
        else if ( caught2O )
        {
            snitchMarkers = dotMarker;
        }

        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
    else if ( period == dot )
    {
        score -= 30;
        caught2O = false;
        snitchMarkers = "";
        
        if ( caughtR )
            if ( caughtO )
            {
                snitchMarkers = regMarker + otMarker;
            }
            else
            {
                snitchMarkers = regMarker;
            }
        else if ( caughtO )
        {
            snitchMarkers = otMarker;
        }
        
        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
}
