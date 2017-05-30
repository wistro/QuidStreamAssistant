/*
  ==============================================================================

    Team.h
    Created: 28 May 2017 10:17:40pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "OSDependencyThings.h"

#pragma once

class Team
{
public:
    Team ();
    
    void readFromFile (const File& file);
    void readFromXML (const XmlElement&);
    void writeToFile (const File& file) const;
    
    void restoreDefaultTeamFile();
    void setAsDefaults();
    static String getDefaultFileName();
    
    void addPlayer();
    void removePlayer();
    
    String getTeamName();
    String getTeamAbv();
    StringArray getRoster();
    
//    void fillThisSucker(String name, String location, String rounds);
//    void fillThisSucker(String name, String location, String rounds, File pic);
    
    Image logo;

    StringArray setTeamList();
    void refreshTeamList();
    
    static const String consolation;
    static const StringArray consolationBracket;
    StringArray tournamentList;
    
    static File getTeamsFolder();
    static const char* getTeamFileSuffix()      { return ".tourn"; }
    static const char* getTeamFileWildCard()    { return "*.tourn"; }

    
private:
    
    Array<File> tournamentFiles;
    
    String tournamentName;
    String tournamentLocation;
    
//    StringArray teamsList;
    StringArray roundsList;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Team)
};
