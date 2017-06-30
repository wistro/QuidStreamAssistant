/*
  ==============================================================================

    HRSnitchWindow.h
    Created: 30 Jun 2017 5:58:50pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"

//==============================================================================
/*
*/
class HRSnitchWindow    : public Component,
                          public Button::Listener
{
public:
  HRSnitchWindow();
  ~HRSnitchWindow();

  void paint (Graphics&) override;
  void resized() override;
  void buttonClicked (Button* button) override;

private:
  
  TextEditor editHRs;
  TextEditor editSnitches;
  
  Label hr;
  Label snitch;
  
  TextButton save;
  TextButton cancel;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HRSnitchWindow)
};
