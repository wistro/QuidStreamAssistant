/*
  ==============================================================================

  Team.h
  Created: 28 May 2017 10:17:40pm
  Author:  Willow Rosenberg
 
  ==============================================================================
*/

#include "OSDependencyThings.h"
#include "Player.h"

#pragma once

class Team
{
public:
  Team ();
  Team ( const File& file );
  ~Team();
  
  void readFromFile (const File& file);
  void readFromXML (const XmlElement&);
  void writeToFile (const File& file) const;
  
  static String getDefaultFileName();
  
  void addPlayer( Player* newPlayer );
  
  void removePlayer( Player* oldPlayer );
  void removePlayer( String number );
  
//  void editPlayer( Player* editMe );
//  void editPlayer( String number );
  
  String getTeamName();
  String getTeamAbv();
  StringArray getRoster();
  
  void setTeamName( String newName );
  void setTeamAbv( String newAbv);
  
  void refreshRoster();
  
  void fillThisSucker(String name, String abv);
  void fillThisSucker(String name, String abv, File pic);
  
  Image logo;
  OwnedArray<Player> team;

  static void refreshTeamList();
  
  static StringArray teamList;
  
  static const String defaultTeamName;
  static File getTeamsFolder();
  static const char* getTeamFileSuffix()    { return ".team"; }
  static const char* getTeamFileWildCard()  { return "*.team"; }

  
private:
  
  static Array<File> teamFiles;
  static StringArray setTeamList();
  
  String teamName;
  String teamAbv;
  
  StringArray roster;
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Team)
};
