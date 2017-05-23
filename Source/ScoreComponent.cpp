/*
  ==============================================================================

    ScoreComponent.cpp
    Created: 23 May 2017 10:41:28am
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScoreComponent.h"

const String regMarker = "*";
const String otMarker = "^";
const String dotMarker = "$";

//==============================================================================
ScoreComponent::ScoreComponent(bool areButtonsOnLeft)
    : increase("increase", 0.75f, Colours::black),
      decrease("decrease", 0.25f, Colours::black)
{
    score = 0;
    snitchMarkers = "";
    
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
        showScore.setText(String(score), sendNotification);
    }
    else if (button == &decrease)
    {
        //negative scores are not allowed,
        //if score <= 0, nothing happens
        //when decrease button pressed
        if ( score > 0 )
        {
            score -= 10;
            showScore.setText(String(score), sendNotification);
        }
    }

}

String ScoreComponent::getScoreWithSnitchMarks()
{
    return showScore.getText();
}

int ScoreComponent::getScore()
{
    return score;
}

void ScoreComponent::addSnitchCatch(char period)
{
    period = tolower(period); //make sure it's lowercase
    
    if ( period == 'r' ) //regulation time
    {
        score += 30;
        snitchMarkers.append(regMarker, 1);
        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
    else if ( period == 'o' ) //overtime
    {
        score += 30;
        snitchMarkers.append(otMarker, 1);
        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
    else if ( period == 'd' )
    {
        score += 30;
        snitchMarkers.append(dotMarker, 1);
        showScore.setText(String(score) + snitchMarkers, sendNotification);
    }
}
