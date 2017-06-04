/*
  ==============================================================================

  PresetIDs.h
  Created: 28 May 2017 8:54:16pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

// Handy list of static Identifiers..
namespace Ids
{
  #define DECLARE_ID(name)    const Identifier name (#name)
  
  DECLARE_ID (team);
  DECLARE_ID (abv);
  DECLARE_ID (first);
  DECLARE_ID (last);
  DECLARE_ID (number);
  DECLARE_ID (jersey);
  DECLARE_ID (pronouns);
  DECLARE_ID (positions);
  DECLARE_ID (name);
  DECLARE_ID (location);
  DECLARE_ID (round);
  
  const Identifier ID ("id");
  const Identifier ID_uppercase ("ID");
  const Identifier class_ ("class");
  
  #undef DECLARE_ID
}

enum ColourIds
{
  backgroundColourId                = 0x2340000,
  secondaryBackgroundColourId       = 0x2340001,
  defaultTextColourId               = 0x2340002,
  widgetTextColourId                = 0x2340003,
  defaultButtonBackgroundColourId   = 0x2340004,
  secondaryButtonBackgroundColourId = 0x2340005,
  userButtonBackgroundColourId      = 0x2340006,
  defaultIconColourId               = 0x2340007,
  treeIconColourId                  = 0x2340008,
  defaultHighlightColourId          = 0x2340009,
  defaultHighlightedTextColourId    = 0x234000a,
  codeEditorLineNumberColourId      = 0x234000b,
  activeTabIconColourId             = 0x234000c,
  inactiveTabBackgroundColourId     = 0x234000d,
  inactiveTabIconColourId           = 0x234000e,
  contentHeaderBackgroundColourId   = 0x234000f,
  widgetBackgroundColourId          = 0x2340010,
  secondaryWidgetBackgroundColourId = 0x2340011,
};
