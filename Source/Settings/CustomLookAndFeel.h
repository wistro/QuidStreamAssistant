/*
  ==============================================================================

  CustomLookAndFeel.h
  Created: 25 May 2017 9:29:11pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"

//==============================================================================
/*
*/
class CustomLookAndFeel  : public LookAndFeel_V4
{
public:
  //==============================================================================
  /** Creates a CustomLookAndFeel object with a default colour scheme. */
  CustomLookAndFeel();
  
  
  /** Destructor. */
  ~CustomLookAndFeel();
  
  
  void drawLabel (Graphics&, Label&) override;
  void setupColours();
  
private:
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};
