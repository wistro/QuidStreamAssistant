/*
  ==============================================================================

    ScoreComponent.h
    Created: 23 May 2017 10:41:28am
    Author:  Jason Rosenberg

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ScoreComponent    : public Component
{
public:
    ScoreComponent();
    ~ScoreComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScoreComponent)
};
