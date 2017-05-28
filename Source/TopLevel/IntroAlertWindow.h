/*
  ==============================================================================

    IntroAlertWindow.h
    Created: 28 May 2017 7:35:17pm
    Author:  Jason Rosenberg

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class IntroAlertWindow    : public Component
{
public:
    IntroAlertWindow();
    ~IntroAlertWindow();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntroAlertWindow)
};
