/*
 ==============================================================================
 
 GameplayWindow.h
 Created: 14 Jul 2017 3:03:08pm
 Author:  Willow Rosenberg
 
 ==============================================================================
 */

#pragma once
#include "../Settings/OSDependencyThings.h"
#include "GameplayComponent.h"

//==============================================================================
/*
 This class implements the desktop window that contains an instance of
 our GameplayComponent class.
 */

class GameplayWindow   : public DocumentWindow
{
public:
  //==============================================================================
  GameplayWindow();
  ~GameplayWindow();
  
  // called by the OS when the window's close button is pressed.
  void closeButtonPressed() override;
  bool escapeKeyPressed();
  
private:
  
  String windowPosProperty;
  
  ScopedPointer<GameplayComponent> streamWindow;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameplayWindow)
};

