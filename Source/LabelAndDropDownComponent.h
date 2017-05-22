/*
  ==============================================================================

    LabelAndDropDownComponent.h
    Created: 22 May 2017 11:46:33pm
    Author:  Willow Rosenberg
    
    Has a text Label and a ComboBox dropdown list organised nicely side-by-side

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class LabelAndDropDownComponent    : public Component
{
public:
    LabelAndDropDownComponent();
    ~LabelAndDropDownComponent();

    void paint (Graphics&) override;
    void resized() override;

private:

    Label listDescriptor;
    ComboBox dropDown;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabelAndDropDownComponent)
};
