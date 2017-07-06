/*
  ==============================================================================

  Main.cpp
  Created: 22 May 2017 11:30:54pm
  Author:  Willow Rosenberg

  It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../Settings/CustomLookAndFeel.h"
#include "../Settings/OSDependencyThings.h"
#include "../Settings/PresetIDs.h"
#include "../Settings/StoredSettings.h"

#include "../Settings/Player.h"
#include "../Settings/FloatingToolWindow.h"
#include "../Settings/Team.h"
#include "../Settings/Tournament.h"

//==============================================================================

#include "../Gameplay/GameTimer.h"
#include "../Gameplay/ScoreComponent.h"
#include "../Gameplay/SnitchSliders.h"
#include "../Gameplay/StopWatch.h"
#include "../Gameplay/PronounsAndLogo.h"
#include "../Gameplay/GameplayComponent.h"

//==============================================================================

#include "../ToolWindows/EditTournamentWindow.h"
#include "../ToolWindows/IntroAlertWindow.h"
#include "../ToolWindows/MainAppWindow.h"
#include "../ToolWindows/TeamListBoxComponent.h"
#include "../ToolWindows/SelectTeamsWindow.h"
#include "../ToolWindows/RosterListBoxComponent.h"
#include "../ToolWindows/EditTeamWindow.h"
#include "../ToolWindows/HRSnitchWindow.h"

//==============================================================================

#include "Application.h"

//==============================================================================




//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (QuidStreamAssistantApplication)
