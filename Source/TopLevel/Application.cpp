/*
  ==============================================================================

    Application.cpp
    Created: 27 May 2017 1:57:44am
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "Application.h"

void QuidStreamAssistantApplication::initialise (const String& commandLine)
{
    
    Desktop::getInstance().setOrientationsEnabled (Desktop::allOrientations);
    
    // This method is where you should put your application's initialisation code..
    
    mainWindow = new MainAppWindow ();
}

void QuidStreamAssistantApplication::shutdown()
{
    // Add your application's shutdown code here..
    
    mainWindow = nullptr; // (deletes our window)
}

//==============================================================================
void QuidStreamAssistantApplication::systemRequestedQuit()
{
    // This is called when the app is being asked to quit: you can ignore this
    // request and let the app carry on running, or call quit() to allow the app to close.
    quit();
}

void QuidStreamAssistantApplication::anotherInstanceStarted (const String& commandLine)
{
    // When another instance of the app is launched while this one is running,
    // this method is invoked, and the commandLine parameter tells you what
    // the other instance's command-line arguments were.
}

QuidStreamAssistantApplication& QuidStreamAssistantApplication::getApp()
{
    QuidStreamAssistantApplication* const app = dynamic_cast<QuidStreamAssistantApplication*> (JUCEApplication::getInstance());
    jassert (app != nullptr);
    return *app;
}

PropertiesFile::Options QuidStreamAssistantApplication::getPropertyFileOptionsFor (const String& filename)
{
    PropertiesFile::Options options;
    options.applicationName     = filename;
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Application Support";
    #if JUCE_LINUX
    options.folderName          = "~/.config/Projucer";
    #else
    options.folderName          = "QuidStreamAssistant";
    #endif
    
    return options;
}

ApplicationCommandManager& QuidStreamAssistantApplication::getCommandManager()
{
    ApplicationCommandManager* cm = QuidStreamAssistantApplication::getApp().commandManager;
    jassert (cm != nullptr);
    return *cm;
}
