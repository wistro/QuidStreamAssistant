/*
  ==============================================================================

    MainAppWindow.cpp
    Created: 24 May 2017 5:20:28pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "MainAppWindow.h"
#include "Application.h"

//==============================================================================
MainAppWindow::MainAppWindow()
            : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                  LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                  DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setResizable (false, false);
    setIcon(ImageCache::getFromMemory(BinaryData::willogo_png, BinaryData::willogo_pngSize));
    
//    setResizeLimits (400, 400, 10000, 10000);
//    
//    setBounds ((int) (0.1f * getParentWidth()),
//               (int) (0.1f * getParentHeight()),
//               jmax (60, (int) (0.5f * getParentWidth())),
//               jmax (40, (int) (0.7f * getParentHeight())));
    
    
    centreWithSize(400, 200);
    
    intro = new IntroAlertWindow(QuidStreamAssistantApplication::getApp().thisTournament);
    setContentNonOwned(intro, false);
    setVisible(true);

    
//    content = new MainContentComponent;
//    setContentNonOwned(content, false);
//    setVisible(true);
    
    
//    #if JUCE_WINDOWS || JUCE_LINUX || JUCE_MAC
//    taskbarIcon = new TaskbarComponent();
//    #endif
    


}

MainAppWindow::~MainAppWindow()
{
//    content = nullptr;
    intro = nullptr;
}

void MainAppWindow::closeButtonPressed()
{
    
    setVisible(false);
}

void MainAppWindow::showIntro()
{
    QuidStreamAssistantApplication::getApp().thisTournament->refreshTournamentList();
    intro->updateTournamentList();
    setVisible(true);
}

MainAppWindow* MainAppWindow::getMainAppWindow()
{
    for (int i = TopLevelWindow::getNumTopLevelWindows(); --i >= 0;)
        if (auto* maw = dynamic_cast<MainAppWindow*> (TopLevelWindow::getTopLevelWindow (i)))
            return maw;
    
    return nullptr;
}
