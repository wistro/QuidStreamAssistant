/*
  ==============================================================================

  StopWatch.cpp
  Created: 23 May 2017 6:37:57pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "StopWatch.h"

//==============================================================================
StopWatch::StopWatch()
{
  currentTime.setFont(50.0f);
  currentTime.setJustificationType(Justification::centred);
  addAndMakeVisible(currentTime);
}

StopWatch::~StopWatch()
{
}

//increment timer by 1 second and display new value
void StopWatch::timerCallback()
{
  timer.operator+=(1);
  repaint();
}

//read out current value of timer in seconds
double StopWatch::currentTimeSec()
{
  return timer.inSeconds();
}

//reset the timer to 0
void StopWatch::resetTimer()
{
  RelativeTime reset;
  timer.operator=(reset);
  repaint();
}

void StopWatch::paint (Graphics& g)
{
  int minutes = floor(timer.inMinutes());
  int seconds = timer.inSeconds() - (minutes * 60);
  
  String min = "";
  String sec = "";
  
  if ( minutes < 10 )
  {
    min = "0" + String(minutes);
  }
  else
  {
    min = String(minutes);
  }
  
  if ( seconds < 10 )
  {
    sec = "0" + String(seconds);
  }
  else
  {
    sec = String(seconds);
  }
  
  currentTime.setText (min + ":" + sec, sendNotification);
}

void StopWatch::resized()
{
  
}
