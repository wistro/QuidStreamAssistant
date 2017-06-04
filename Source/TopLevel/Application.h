/*
  ==============================================================================

  Application.h
  Created: 27 May 2017 1:57:44am
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once
#include "../Settings/OSDependencyThings.h"
#include "../ToolWindows/MainAppWindow.h"
#include "../ToolWindows/EditTournamentWindow.h"
#include "../Settings/StoredSettings.h"
#include "../Settings/Tournament.h"

//==============================================================================
class QuidStreamAssistantApplication  : public JUCEApplication
{
public:
  //==============================================================================
  QuidStreamAssistantApplication() {}
  
  static QuidStreamAssistantApplication& getApp();
  static ApplicationCommandManager& getCommandManager();
  PropertiesFile::Options getPropertyFileOptionsFor (const String& filename);
  
  const String getApplicationName() override     { return ProjectInfo::projectName; }
  const String getApplicationVersion() override  { return ProjectInfo::versionString; }
  bool moreThanOneInstanceAllowed() override     { return true; }
  
  //==============================================================================
  void initialise (const String& commandLine) override;
  void shutdown() override;
   
  //==============================================================================
  void systemRequestedQuit() override;
  void anotherInstanceStarted (const String& commandLine) override;
  
  //==============================================================================
  
  void showEditTournamentWindow();
  void showTeamSelectWindow();
  
  void showEditTeamWindow();
  void showEditTeamWindow(String teamName);
  
  //==============================================================================

  ScopedPointer<StoredSettings> settings;
  ScopedPointer<ApplicationCommandManager> commandManager;
  ScopedPointer<Tournament> thisTournament;
  
  ScopedPointer<Component> editTournament;
  ScopedPointer<Component> teamSelect;
  ScopedPointer<Component> editTeam;
  
  CustomLookAndFeel defaultLook;
  ScopedPointer<MainAppWindow> mainWindow;
  
  //==============================================================================
  
private:
  TooltipWindow tooltipWindow;
};
