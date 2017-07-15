/*
==============================================================================

MainAppWindow.h
Created: 24 May 2017 5:20:28pm
Author:  Willow Rosenberg

==============================================================================
*/

#pragma once
#include "../Settings/OSDependencyThings.h"
#include "IntroAlertWindow.h"

//==============================================================================
/*
This class implements the desktop window that contains an instance of
our IntroAlertWindow class.
*/

class MainAppWindow   : public DocumentWindow
{
public:
  //==============================================================================
  MainAppWindow();
  ~MainAppWindow();
  
  static MainAppWindow* getMainAppWindow(); // returns the MainWindow if it exists.
  
  
  // called by the OS when the window's close button is pressed.
  void closeButtonPressed() override;
  void showIntro();
  
private:  
  
  ScopedPointer<IntroAlertWindow> intro;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)
};

