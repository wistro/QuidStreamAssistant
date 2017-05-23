/*
  ==============================================================================

    ConfirmClick.cpp
    Created: 23 May 2017 11:22:22pm
    Author:  Jason Rosenberg

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ConfirmClick.h"

//==============================================================================
ConfirmClick::ConfirmClick(String labelTextToShow)
{
    doWeKeepGoing = false; //default to a resounding 'no thank you'
    
    text.setText(labelTextToShow, dontSendNotification);
    text.setJustificationType(Justification::centredBottom);
    text.setFont(16.0f);
    
    addAndMakeVisible(text);
    addAndMakeVisible(yes);
    addAndMakeVisible(no);
    
    yes.addListener(this);
    no.addListener(this);
}

ConfirmClick::~ConfirmClick()
{
    yes.removeListener(this);
    no.removeListener(this);
}

void ConfirmClick::paint (Graphics& g)
{
    Rectangle<int> area( getLocalBounds() );
    const int margin = 4;
    
    text.setBounds(area.removeFromTop(area.getHeight() / 2).reduced(margin));
    
    yes.setBounds(area.removeFromLeft((area.getWidth() / 2) - (margin / 2)).reduced(margin));
    
    no.setBounds(area);
}

void ConfirmClick::resized()
{
}

void ConfirmClick::buttonClicked(Button* button)
{
    if ( button == &yes )
        doWeKeepGoing = true;
    else if ( button == &no )
        doWeKeepGoing = false;
}

bool ConfirmClick::getState()
{
    return doWeKeepGoing;
}
