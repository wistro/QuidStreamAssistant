/*
  ==============================================================================

    LabelAndListComponent.h
    Created: 22 May 2017 11:46:33pm
    Author:  Willow Rosenberg
    
    Has a text Label and a ComboBox dropdown list organised nicely side-by-side

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"

//==============================================================================
/*
*/
class LabelAndListComponent    : public Component
{
public:
    LabelAndListComponent();
    ~LabelAndListComponent();

    void paint (Graphics&) override;
    void resized() override;

private:

    Label label;
    ComboBox dropDown;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabelAndListComponent)
};
