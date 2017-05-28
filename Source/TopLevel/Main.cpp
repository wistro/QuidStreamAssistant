/*
  ==============================================================================

    Main.cpp
    Created: 22 May 2017 11:30:54pm
    Author:  Willow Rosenberg

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "../Settings/FilePathPropertyComponent.h"
#include "../Settings/StoredSettings.h"
#include "../Settings/Player.h"
#include "../Settings/TeamSettings.h"

#include "../Gameplay/ScoreComponent.h"
#include "../Gameplay/GameTimer.h"
#include "../Gameplay/LabelAndListComponent.h"
#include "../Gameplay/SnitchSliders.h"
#include "../Gameplay/StopWatch.h"
#include "../Gameplay/GameplayComponent.h"

//==============================================================================

#include "../TopLevel/CustomLookAndFeel.h"
#include "../TopLevel/MainAppWindow.h"
#include "../TopLevel/MainComponent.h"
#include "../TopLevel/MenuBar.h"
#include "../IntroAlertWindow.h"
#include "Application.h"

//==============================================================================




//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (QuidStreamAssistantApplication)
