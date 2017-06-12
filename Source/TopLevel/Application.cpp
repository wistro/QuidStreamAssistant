/*
  ==============================================================================

  Application.cpp
  Created: 27 May 2017 1:57:44am
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "Application.h"
#include "../Settings/FloatingToolWindow.h"
#include "../ToolWindows/SelectTeamsWindow.h"
#include "../ToolWindows/EditTeamWindow.h"
#include "../Gameplay/GameplayComponent.h"

void QuidStreamAssistantApplication::initialise (const String& commandLine)
{
  
  Desktop::getInstance().setOrientationsEnabled (Desktop::allOrientations);
  
  // This method is where you should put your application's initialisation code..
  
  settings = new StoredSettings ();
  thisTournament = new Tournament ();
  defaultLook.setColourScheme(CustomLookAndFeel::getLightColourScheme());
  defaultLook.setUsingNativeAlertWindows(true);
  LookAndFeel::setDefaultLookAndFeel(&defaultLook);
  
  mainWindow = new MainAppWindow ();
  commandManager = new ApplicationCommandManager ();
}

void QuidStreamAssistantApplication::shutdown()
{
  // Add your application's shutdown code here..
  
  mainWindow = nullptr;
  editTournament = nullptr;
  teamSelect = nullptr;
  editTeam = nullptr;
  streamingWindow = nullptr;
  thisTournament = nullptr;

  settings = nullptr;
  commandManager = nullptr;
}

//==============================================================================

void QuidStreamAssistantApplication::showEditTournamentWindow()
{
  if (editTournament != nullptr)
    editTournament->toFront (true);
  else
    new FloatingToolWindow ("Edit Tournament Details", "editTournamentWindowPos", new EditTournamentWindow(),
                editTournament, false,
                600, 500, 600, 500, 600, 500);
}

void QuidStreamAssistantApplication::showTeamSelectWindow()
{
  if (teamSelect != nullptr)
    teamSelect->toFront (true);
  else
    new FloatingToolWindow ("Select The Teams That Will Be Attending", "teamSelectWindowPos", new SelectTeamsWindow(),
                teamSelect, false,
                600, 500, 600, 500, 600, 500);
}

void QuidStreamAssistantApplication::showTeamSelectWindow( StringArray prevTeams )
{
  if (teamSelect != nullptr)
    teamSelect->toFront (true);
  else
    new FloatingToolWindow ("Select The Teams That Will Be Attending", "teamSelectWindowPos", new SelectTeamsWindow(prevTeams),
                            teamSelect, false,
                            600, 500, 600, 500, 600, 500);
}

void QuidStreamAssistantApplication::showEditTeamWindow()
{
  if (editTeam != nullptr)
    editTeam->toFront (true);
  else
    new FloatingToolWindow ("Add New Team Details", "editTeamWindowPos", new EditTeamWindow(),
                editTeam, false,
                800, 600, 800, 600, 800, 600);
}

void QuidStreamAssistantApplication::showEditTeamWindow(String teamName)
{
  if (editTeam != nullptr)
    editTeam->toFront (true);
  else
    new FloatingToolWindow ("Edit Team Details", "editTeamWindowPos", new EditTeamWindow(teamName),
                editTeam, false,
                800, 600, 800, 600, 800, 600);
}

void QuidStreamAssistantApplication::showStreamingWindow()
{
  if (streamingWindow != nullptr)
    streamingWindow->toFront (true);
  else
    new FloatingToolWindow ("Streaming Nervecentre", "streamingWindowPos", new GameplayComponent(),
                            streamingWindow, false,
                            1000, 410, 1000, 410, 1000, 410);
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
  options.applicationName   = filename;
  options.filenameSuffix    = "settings";
  options.osxLibrarySubFolder = "Application Support";
  #if JUCE_LINUX
  options.folderName      = "~/.config/QuidStreamAssistant";
  #else
  options.folderName      = "QuidStreamAssistant";
  #endif
  
  return options;
}

ApplicationCommandManager& QuidStreamAssistantApplication::getCommandManager()
{
  ApplicationCommandManager* cm = QuidStreamAssistantApplication::getApp().commandManager;
  jassert (cm != nullptr);
  return *cm;
}
