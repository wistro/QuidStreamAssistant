/*
  ==============================================================================

    ConfirmClick.h
    Created: 23 May 2017 11:22:22pm
    Author:  Jason Rosenberg

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ConfirmClick    : public Component,
                        public Button::Listener
{
public:
    ConfirmClick( String labelTextToShow );
    ~ConfirmClick();

    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    
    bool getState();

private:
    Label text;
    TextButton yes;
    TextButton no;
    
    bool doWeKeepGoing;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConfirmClick)
};
