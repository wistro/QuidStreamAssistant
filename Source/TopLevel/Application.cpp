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
struct QuidStreamAssistantApplication::MainMenuModel  : public MenuBarModel
{
  MainMenuModel()
  {
    setApplicationCommandManagerToWatch (&getCommandManager());
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
  if (menuName == "Tools")            createToolsMenu   (menu);
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
  
  const CommandID ids[] = { CommandIDs::newProject,
    CommandIDs::open,
    CommandIDs::closeAllDocuments,
    CommandIDs::saveAll,
    CommandIDs::showGlobalPreferences,
    CommandIDs::showUTF8Tool,
    CommandIDs::showSVGPathTool,
    CommandIDs::showAboutWindow,
    CommandIDs::showAppUsageWindow,
    CommandIDs::loginLogout };
  
  commands.addArray (ids, numElementsInArray (ids));
}

void QuidStreamAssistantApplication::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
  switch (commandID)
  {
    case CommandIDs::newProject:
      result.setInfo ("New Project...", "Creates a new Jucer project", CommandCategories::general, 0);
      result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::open:
      result.setInfo ("Open...", "Opens a Jucer project", CommandCategories::general, 0);
      result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::showGlobalPreferences:
      result.setInfo ("Preferences...", "Shows the preferences window.", CommandCategories::general, 0);
      result.defaultKeypresses.add (KeyPress (',', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::closeAllDocuments:
      result.setInfo ("Close All Documents", "Closes all open documents", CommandCategories::general, 0);
      result.setActive (openDocumentManager.getNumOpenDocuments() > 0);
      break;
      
    case CommandIDs::saveAll:
      result.setInfo ("Save All", "Saves all open documents", CommandCategories::general, 0);
      result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
      break;
      
    case CommandIDs::showUTF8Tool:
      result.setInfo ("UTF-8 String-Literal Helper", "Shows the UTF-8 string literal utility", CommandCategories::general, 0);
      break;
      
    case CommandIDs::showSVGPathTool:
      result.setInfo ("SVG Path Converter", "Shows the SVG->Path data conversion utility", CommandCategories::general, 0);
      break;
      
    case CommandIDs::showAboutWindow:
      result.setInfo ("About Projucer", "Shows the Projucer's 'About' page.", CommandCategories::general, 0);
      break;
      
    case CommandIDs::showAppUsageWindow:
      result.setInfo ("Application Usage Data", "Shows the application usage data agreement window", CommandCategories::general, 0);
      break;
      
    case CommandIDs::loginLogout:
    {
      bool isLoggedIn = false;
      String username;
      
      if (licenseController != nullptr)
      {
        const LicenseState state = licenseController->getState();
        isLoggedIn = (state.type != LicenseState::Type::notLoggedIn && state.type != LicenseState::Type::GPL);
        username = state.username;
      }
      
      result.setInfo (isLoggedIn
                      ? String ("Sign out ") + username + "..."
                      : String ("Sign in..."),
                      "Log out of your JUCE account", CommandCategories::general, 0);
    }
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
    case CommandIDs::newProject:                createNewProject(); break;
    case CommandIDs::open:                      askUserToOpenFile(); break;
    case CommandIDs::saveAll:                   openDocumentManager.saveAll(); break;
    case CommandIDs::closeAllDocuments:         closeAllDocuments (true); break;
    case CommandIDs::showUTF8Tool:              showUTF8ToolWindow(); break;
    case CommandIDs::showSVGPathTool:           showSVGPathDataToolWindow(); break;
    case CommandIDs::showGlobalPreferences:     AppearanceSettings::showGlobalPreferences (globalPreferencesWindow); break;
    case CommandIDs::showAboutWindow:           showAboutWindow(); break;
    case CommandIDs::showAppUsageWindow:        showApplicationUsageDataAgreementPopup(); break;
    case CommandIDs::loginLogout:               doLogout(); break;
    default:                                    return JUCEApplication::perform (info);
  }
  
  return true;
}

//==============================================================================
