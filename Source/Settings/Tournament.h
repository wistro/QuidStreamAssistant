/*
  ==============================================================================

    Tournament.h
    Created: 28 May 2017 10:17:40pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "OSDependencyThings.h"

#pragma once

class Tournament    : private ValueTree::Listener
{
public:
    Tournament (bool updateAppWhenChanged);
    
    bool readFromFile (const File& file);
    bool readFromXML (const XmlElement&);
    bool writeToFile (const File& file) const;
    
    static void restoreDefaultTournamentFile();
    void setAsDefaults();
    
    void updateTournament();
    
    void addTeam();
    void addRound();
    void removeTeam();
    void removeRound();
    
    ValueTree tournament;

    static StringArray setTournamentList();
    static void refreshTournamentList();
    
    static const String consolation;
    static StringArray tournamentList;
    
    static File getTournamentsFolder();
    static const char* getTournamentFileSuffix()      { return ".tourn"; }
    static const char* getTournamentFileWildCard()    { return "*.tourn"; }

    
private:
    
    static Array<File> tournamentFiles;
    
    StringArray teamsList;
    StringArray roundsList;
    
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override   { updateTournament(); }
    void valueTreeChildAdded (ValueTree&, ValueTree&) override               { updateTournament(); }
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override        { updateTournament(); }
    void valueTreeChildOrderChanged (ValueTree&, int, int) override          { updateTournament(); }
    void valueTreeParentChanged (ValueTree&) override                        { updateTournament(); }
    void valueTreeRedirected (ValueTree&) override                           { updateTournament(); }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tournament)
};
