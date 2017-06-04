/*
  ==============================================================================

  StopWatch.h
  Created: 23 May 2017 6:37:57pm
  Author:  Willow Rosenberg
 
  it's a stopwatch. it counts in seconds. 
  calling startTimer() with a value other than 1000 (== #milliseconds in a second)
  is ill advised (as in it won't work and you'll be sad)

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"

//==============================================================================
/*
*/
class StopWatch  : public Timer,
            public Component
{
public:
  StopWatch();
  ~StopWatch();

  void timerCallback() override;
  void paint (Graphics&) override;
  void resized() override;
  
  double currentTimeSec();
  void resetTimer();
  
  Label currentTime;

private:
  
  RelativeTime timer;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StopWatch)
};
