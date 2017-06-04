/*
  ==============================================================================

  SnitchSliders.h
  Created: 23 May 2017 12:39:24am
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"

//==============================================================================
/*
*/
class SnitchSliders  : public Component
{
public:
  
  //snitch catch sliders
  Slider snitchReg;
  Slider snitchOT;
  Slider snitch2OT;
  
  SnitchSliders();
  ~SnitchSliders();

  void paint (Graphics&) override;
  void resized() override;

private:
  
  //labels for sliders
  Label regulation;
  Label overTime;
  Label doubleOverTime;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SnitchSliders)
};
