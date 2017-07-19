/*
  ==============================================================================

  Application.cpp
  Created: 27 May 2017 1:57:44am
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "../Settings/CommandIDs.h"
#include "Application.h"
#include "../Settings/FloatingToolWindow.h"
#include "../ToolWindows/SelectTeamsWindow.h"
#include "../ToolWindows/EditTeamWindow.h"
#include "../ToolWindows/HRSnitchWindow.h"
#include "../Gameplay/GameplayComponent.h"

//==============================================================================

void createGUIEditorMenu (PopupMenu&);
void handleGUIEditorMenuCommand (int);
void registerGUIEditorCommands();

//==============================================================================

struct QuidStreamAssistantApplication::MainMenuModel  : public MenuBarModel
{
  MainMenuModel()
  {
    setApplicationCommandManagerToWatch (&getCommandManager());
  }
  
  ~MainMenuModel ()
  {
  }
  
  StringArray getMenuBarNames() override
  {
    return getApp().getMenuNames();
  }
  
  PopupMenu getMenuForIndex (int /*topLevelMenuIndex*/, const String& menuName) override
  {
    PopupMenu menu;
    getApp().createMenu (menu, menuName);
    return menu;
  }
  
  void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/) override
  {
    getApp().handleMainMenuCommand (menuItemID);
  }
};

//==============================================================================
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
  commandManager->registerAllCommandsForTarget (this);
  
  menuModel = new MainMenuModel();
  
  #if JUCE_MAC
  PopupMenu extraAppleMenuItems;
  createExtraAppleMenuItems (extraAppleMenuItems);
  #endif
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
  menuModel = nullptr;
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
    streamingWindow = new GameplayWindow();
}

void QuidStreamAssistantApplication::showHRSnitchWindow()
{
  if (hrSnitch != nullptr)
    hrSnitch->toFront (true);
  else
    new FloatingToolWindow ("Head Refs & Snitches", "hrsnitchWindowPos", new HRSnitchWindow(),
                            hrSnitch, false,
                            600, 500, 600, 500, 600, 500);
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

//==============================================================================
MenuBarModel* QuidStreamAssistantApplication::getMenuModel()
{
  return menuModel.get();
}

StringArray QuidStreamAssistantApplication::getMenuNames()
{
  const char* const names[] = { "File", "Tools", nullptr };
  return StringArray (names);
}

void QuidStreamAssistantApplication::createMenu (PopupMenu& menu, const String& menuName)
{
  if (menuName == "File")             createFileMenu   (menu);
  else if (menuName == "Tools")       createToolsMenu(menu);
  else                                jassertfalse; // names have changed?
}

void QuidStreamAssistantApplication::createFileMenu (PopupMenu& menu)
{
  menu.addCommandItem (commandManager, StandardApplicationCommandIDs::quit);
}

void QuidStreamAssistantApplication::createToolsMenu (PopupMenu& menu)
{
  menu.addCommandItem (commandManager, CommandIDs::showEditTournamentWindow);
  menu.addCommandItem (commandManager, CommandIDs::showSelectTeamsWindow);
  menu.addCommandItem (commandManager, CommandIDs::showIntroWindow);
  menu.addCommandItem (commandManager, CommandIDs::showHRSnitchWindow);
}

void QuidStreamAssistantApplication::createExtraAppleMenuItems (PopupMenu& menu)
{
  menu.addCommandItem (commandManager, CommandIDs::showAboutWindow);
  menu.addSeparator();
}

//==============================================================================
void QuidStreamAssistantApplication::getAllCommands (Array <CommandID>& commands)
{
  JUCEApplication::getAllCommands (commands);
  
  const CommandID ids[] = { CommandIDs::showEditTournamentWindow,
    CommandIDs::showIntroWindow,
    CommandIDs::showSelectTeamsWindow,
    CommandIDs::showHRSnitchWindow,
    //CommandIDs::showAboutWindow 
  };
  
  commands.addArray (ids, numElementsInArray (ids));
}

void QuidStreamAssistantApplication::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
  switch (commandID)
  {
    case CommandIDs::showEditTournamentWindow:
      result.setInfo ("Edit Tournament Info", "Shows the edit tournament window.", CommandCategories::general, 0);
      result.defaultKeypresses.add (KeyPress (',', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::showIntroWindow:
      result.setInfo ("Go Back to the Beginning", "Shows the intro window.", CommandCategories::general, 0);
      break;
      
    case CommandIDs::showSelectTeamsWindow:
      result.setInfo ("Team Selector", "Choose which Teams will be at the Tournament (go here to edit Teams too)", CommandCategories::general, 0);
      break;
      
 /*   case CommandIDs::showAboutWindow:
      result.setInfo ("About QuidStream Assistant", "Shows the 'About' page.", CommandCategories::general, 0);
      break;*/
      
    case CommandIDs::showHRSnitchWindow:
      result.setInfo ("Head Refs & Snitches", "Add/Remove names from the HR & Snitch lists", CommandCategories::general, 0);
      break;

    default:
      JUCEApplication::getCommandInfo (commandID, result);
      break;
  }
}

bool QuidStreamAssistantApplication::perform (const InvocationInfo& info)
{
  switch (info.commandID)
  {
    case CommandIDs::showEditTournamentWindow:  showEditTournamentWindow(); break;
    case CommandIDs::showIntroWindow:           mainWindow->showIntro(); streamingWindow = nullptr; break;
    case CommandIDs::showSelectTeamsWindow:     showTeamSelectWindow(); break;
    case CommandIDs::showHRSnitchWindow:        showHRSnitchWindow (); break;
    //case CommandIDs::showAboutWindow:           showAboutWindow(); break;
    default:                                    return JUCEApplication::perform (info);
  }
  
  return true;
}

//==============================================================================
