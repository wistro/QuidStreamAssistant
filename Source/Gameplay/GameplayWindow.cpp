/*
  ==============================================================================

    GameplayWindow.cpp
    Created: 14 Jul 2017 3:03:08pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "GameplayWindow.h"
#include "../TopLevel/Application.h"

//==============================================================================
GameplayWindow::GameplayWindow()
                : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                  LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                  DocumentWindow::allButtons)
{
  setUsingNativeTitleBar (true);
  setResizable (false, false);
  setIcon(ImageCache::getFromMemory(BinaryData::willogo_png, BinaryData::willogo_pngSize));
  
  #if ! JUCE_MAC
    setMenuBar (ProjucerApplication::getApp().getMenuModel());
  #endif
  
  int defaultW = 1000;
  int defaultH = 450;
  
  #if ! JUCE_MAC
    defaultH += 40;
  #endif
  
  windowPosProperty = "streamingWindow";
  String windowState;
  if (windowPosProperty.isNotEmpty())
    windowState = getGlobalProperties().getValue (windowPosProperty);
  
  if (windowState.isNotEmpty())
    restoreWindowStateFromString (windowState);
  else
    centreAroundComponent (Component::getCurrentlyFocusedComponent(), defaultW, defaultH);
  
  streamWindow = new GameplayComponent();
  setContentNonOwned(streamWindow, false);
  setVisible(true);
}

GameplayWindow::~GameplayWindow()
{
  #if ! JUCE_MAC
    setMenuBar (nullptr);
  #endif
  
  getGlobalProperties().setValue (windowPosProperty, getWindowStateAsString());
  
  streamWindow = nullptr;
}

void GameplayWindow::closeButtonPressed()
{
  QuidStreamAssistantApplication::getApp().systemRequestedQuit();
}

bool GameplayWindow::escapeKeyPressed()
{
  closeButtonPressed();
  return true;
}
