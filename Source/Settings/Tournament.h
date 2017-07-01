/*
  ==============================================================================

  Tournament.h
  Created: 28 May 2017 10:17:40pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "OSDependencyThings.h"
#include "Team.h"

#pragma once

class Tournament
{
public:
  Tournament ();
  ~Tournament();
  
  void readFromFile (const File& file);
  void readFromXML (const XmlElement&);
  void writeToFile (const File& file) const;
  
  void restoreDefaultTournamentFile();
  void setAsDefaults();
  static String getDefaultFileName();
  
  void clear();
  void clearTeamsList();
  
  void addTeam(String teamName, bool writeLogo = false);
  void addRound(String newRound);
  void removeTeam(String teamName);
  void removeRound(String badRound);
  
  String getTournamentName();
  String getTournamentLocation();
  StringArray getRoundsList();
  StringArray getTeamAbvList();
  StringArray getHRList();
  StringArray getSnitchList();
  
  //both of these should be given a newline-delineated list
  //by which I mean, each HR or snitch is separated from the others by \n characters
  void fillHR ( String newHRs );
  void fillSnitches ( String newSnitches );
  
  
  StringArray getTeamList();
  
  void fillThisSucker(String name, String location, String rounds);
  void fillThisSucker(String name, String location, String rounds, File pic);
  
  Image logo;
  OwnedArray<Team> teams;

  static StringArray setTournamentList();
  static void refreshTournamentList();
  
  static const String consolation;
  static const StringArray consolationBracket;
  static StringArray tournamentList;
  
  static const String defaultTournamentName;
  static File getTournamentsFolder();
  static const char* getTournamentFileSuffix()    { return ".tourn"; }
  static const char* getTournamentFileWildCard()  { return "*.tourn"; }

  
private:
  
  static Array<File> tournamentFiles;
  
  String tournamentName;
  String tournamentLocation;
  
  StringArray teamsList;
  StringArray teamsAbvList;
  StringArray roundsList;
  StringArray hrList;
  StringArray snitchList;
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tournament)
};
