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
#include "../Settings/CustomLookAndFeel.h"

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
  bool moreThanOneInstanceAllowed() override     { return false; }
  
  //==============================================================================
  void initialise (const String& commandLine) override;
  void shutdown() override;
   
  //==============================================================================
  void systemRequestedQuit() override;
  void anotherInstanceStarted (const String& commandLine) override;
  
  //==============================================================================
  
  void showEditTournamentWindow();
  
  void showTeamSelectWindow();
  void showTeamSelectWindow(StringArray prevTeams);
  
  void showEditTeamWindow();
  void showEditTeamWindow(String teamName);
  
  void showStreamingWindow();
  
  void showHRSnitchWindow();
  
  //==============================================================================

  ScopedPointer<StoredSettings> settings;
  ScopedPointer<ApplicationCommandManager> commandManager;
  ScopedPointer<Tournament> thisTournament;
  
  ScopedPointer<Component> editTournament;
  ScopedPointer<Component> teamSelect;
  ScopedPointer<Component> editTeam;
  ScopedPointer<Component> streamingWindow;
  ScopedPointer<Component> hrSnitch;
  
  CustomLookAndFeel defaultLook;
  ScopedPointer<MainAppWindow> mainWindow;
  
  //==============================================================================
  
private:
  TooltipWindow tooltipWindow;
};
