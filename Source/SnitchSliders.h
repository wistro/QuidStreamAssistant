/*
  ==============================================================================

    SnitchSliders.h
    Created: 23 May 2017 12:39:24am
    Author:  Jason Rosenberg

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class SnitchSliders    : public Component
{
public:
    SnitchSliders();
    ~SnitchSliders();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    //snitch catch sliders
    Slider snitchReg;
    Slider snitchOT;
    Slider snitch2OT;
    
    //labels for sliders
    Label regulation;
    Label overTime;
    Label doubleOverTime;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SnitchSliders)
};
