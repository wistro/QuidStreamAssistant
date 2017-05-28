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
    #define DECLARE_ID(name)      const Identifier name (#name)
    
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
