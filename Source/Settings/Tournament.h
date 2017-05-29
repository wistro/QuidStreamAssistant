/*
  ==============================================================================

    Tournament.h
    Created: 28 May 2017 10:17:40pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "OSDependencyThings.h"

#pragma once

class Tournament
{
public:
    Tournament ();
    
    void readFromFile (const File& file);
    void readFromXML (const XmlElement&);
    void writeToFile (const File& file) const;
    
    static void restoreDefaultTournamentFile();
    void setAsDefaults();
    
//    void addTeam();
    void addRound(String newRound);
//    void removeTeam();
    void removeRound(String badRound);
    
    String getTournamentName();
    String getTournamentLocation();
//    StringArray getTeamsList();
    StringArray getRoundsList();
    
    Image logo;

    static StringArray setTournamentList();
    static void refreshTournamentList();
    
    static const String consolation;
    static StringArray tournamentList;
    
    static File getTournamentsFolder();
    static const char* getTournamentFileSuffix()      { return ".tourn"; }
    static const char* getTournamentFileWildCard()    { return "*.tourn"; }

    
private:
    
    static Array<File> tournamentFiles;
    
    String tournamentName;
    String tournamentLocation;
    
//    StringArray teamsList;
    StringArray roundsList;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tournament)
};
