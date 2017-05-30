/*
  ==============================================================================

    Team.h
    Created: 28 May 2017 10:17:40pm
    Author:  Willow Rosenberg

    Need:
        List of Players (probably an array of Player objects)
        team name
        team abbreviation
        logo
        addPlayer()
        removePlayer()
        editPlayer()
        set/refresh master team list
        read/write file/xml
  ==============================================================================
*/

#include "OSDependencyThings.h"
#include "Player.h"

#pragma once

class Team
{
public:
    Team ();
    
    void readFromFile (const File& file);
    void readFromXML (const XmlElement&);
    void writeToFile (const File& file) const;
    
    static String getDefaultFileName();
    
    void addPlayer( Player newPlayer );
    
    void removePlayer( Player* oldPlayer );
    void removePlayer( String number );
    
    void editPlayer( Player* editMe );
    void editPlayer( String number );
    
    String getTeamName();
    String getTeamAbv();
    StringArray getRoster();
    
    void refreshRoster();
    
//    void fillThisSucker(String name, String location, String rounds);
//    void fillThisSucker(String name, String location, String rounds, File pic);
    
    Image logo;

    static StringArray setTeamList();
    static void refreshTeamList();
    
    static StringArray teamList;
    
    static File getTeamsFolder();
    static const char* getTeamFileSuffix()      { return ".team"; }
    static const char* getTeamFileWildCard()    { return "*.team"; }

    
private:
    
    static Array<File> teamFiles;
    
    String teamName;
    String teamAbv;
    
    StringArray roster;
    
    Array<Player> team;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Team)
};
