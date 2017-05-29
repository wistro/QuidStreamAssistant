/*
  ==============================================================================

    Application.h
    Created: 27 May 2017 1:57:44am
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once
#include "../Settings/OSDependencyThings.h"
#include "MainAppWindow.h"
#include "../Settings/StoredSettings.h"


//==============================================================================
class QuidStreamAssistantApplication  : public JUCEApplication
{
public:
    //==============================================================================
    QuidStreamAssistantApplication() {}
    
    static QuidStreamAssistantApplication& getApp();
    static ApplicationCommandManager& getCommandManager();
    PropertiesFile::Options getPropertyFileOptionsFor (const String& filename);
    
    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }
    
    //==============================================================================
    void initialise (const String& commandLine) override;
    void shutdown() override;
   
    //==============================================================================
    void systemRequestedQuit() override;
    void anotherInstanceStarted (const String& commandLine) override;
    
    //==============================================================================

    ScopedPointer<StoredSettings> settings;
    ScopedPointer<ApplicationCommandManager> commandManager;
    
    //==============================================================================
    
private:
    ScopedPointer<MainAppWindow> mainWindow;
};
