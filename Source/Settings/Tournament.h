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
    
    void restoreDefaultTournamentFile();
    void setAsDefaults();
    static String getDefaultFileName();
    
//    void addTeam();
    void addRound(String newRound);
//    void removeTeam();
    void removeRound(String badRound);
    
    String getTournamentName();
    String getTournamentLocation();
//    StringArray getTeamsList();
    StringArray getRoundsList();
    
    Image logo;

    StringArray setTournamentList();
    void refreshTournamentList();
    
    static const String consolation;
    StringArray tournamentList;
    
    static File getTournamentsFolder();
    static const char* getTournamentFileSuffix()      { return ".tourn"; }
    static const char* getTournamentFileWildCard()    { return "*.tourn"; }

    
private:
    
    Array<File> tournamentFiles;
    
    String tournamentName;
    String tournamentLocation;
    
//    StringArray teamsList;
    StringArray roundsList;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tournament)
};
