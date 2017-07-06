/*
  ==============================================================================

    PronounsAndLogo.h
    Created: 6 Jul 2017 5:13:35pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"

//==============================================================================
/*
*/
class PronounsAndLogo    : public Component
{
public:
  PronounsAndLogo();
  ~PronounsAndLogo();

  void paint (Graphics&) override;
  void resized() override;
  
  //the point of this class is to split the
  //pronoun list into two columns
  //so we're just going to give it a stringarray
  //and figure it out in here
//  void setText( StringArray text );
  
  ImageComponent logo;
  StringArray pronounChart;
  
private:
  
//  Label column1;
//  Label column2;
//  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PronounsAndLogo)
};
