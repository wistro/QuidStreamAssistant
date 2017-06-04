/*
  ==============================================================================

  FloatingToolWindow.h
  Created: 29 May 2017 8:47:27pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "OSDependencyThings.h"

//==============================================================================
struct FloatingToolWindow  : public DialogWindow
{
  FloatingToolWindow (const String& title,
            const String& windowPosPropertyName,
            Component* content,
            ScopedPointer<Component>& ownerPointer,
            bool shouldBeResizable,
            int defaultW, int defaultH,
            int minW, int minH,
            int maxW, int maxH)
  : DialogWindow (title, content->findColour (backgroundColourId), true, true),
  windowPosProperty (windowPosPropertyName),
  owner (ownerPointer)
  {
    setUsingNativeTitleBar (true);
    setResizable (shouldBeResizable, shouldBeResizable);
    setResizeLimits (minW, minH, maxW, maxH);
    setContentOwned (content, false);
    
    String windowState;
    if (windowPosProperty.isNotEmpty())
      windowState = getGlobalProperties().getValue (windowPosProperty);
    
    if (windowState.isNotEmpty())
      restoreWindowStateFromString (windowState);
    else
      centreAroundComponent (Component::getCurrentlyFocusedComponent(), defaultW, defaultH);
    
    setVisible (true);
    owner = this;
  }
  
  ~FloatingToolWindow()
  {
    if (windowPosProperty.isNotEmpty())
      getGlobalProperties().setValue (windowPosProperty, getWindowStateAsString());
  }
  
  void closeButtonPressed() override
  {
    owner = nullptr;
  }
  
  bool escapeKeyPressed() override
  {
    closeButtonPressed();
    return true;
  }
  
  void paint (Graphics& g) override
  {
    g.fillAll (findColour (backgroundColourId));
  }
  
private:
  String windowPosProperty;
  ScopedPointer<Component>& owner;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FloatingToolWindow)
};
