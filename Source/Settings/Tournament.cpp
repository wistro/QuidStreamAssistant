/*
  ==============================================================================

  TournamentDatabase.cpp
  Created: 28 May 2017 10:17:40pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "Tournament.h"

#include "OSDependencyThings.h"
#include "StoredSettings.h"
#include "../TopLevel/Application.h"
#include "Team.h"


const String Tournament::consolation = "Consolation";

const StringArray Tournament::consolationBracket =
{   "Consolation Round of 16",
  "Consolation Quarter Finals",
  "Consolation Semi Finals",
  "Consolation 3rd Place Playoff",
  "Consolation Finals"
};

const String Tournament::defaultTournamentName = "DEFAULT";
StringArray Tournament::tournamentList = {};
Array<File> Tournament::tournamentFiles = {};

//==============================================================================
Tournament::Tournament ()
{
  File defaults = getTournamentsFolder().getChildFile(getDefaultFileName()).withFileExtension(getTournamentFileSuffix());
  
  //if defaults file doesn't exist, create it from stored defaults
  if ( ! defaults.existsAsFile())
  {
    restoreDefaultTournamentFile();
  }
  
  teamsList.clear();
  teams.clear();
  
  readFromFile(defaults);
}

//==============================================================================

File Tournament::getTournamentsFolder()
{
  File f (getGlobalProperties().getFile().getSiblingFile ("Tournaments"));
  f.createDirectory();
  return f;
}

StringArray Tournament::setTournamentList()
{
  
  StringArray s;
  for (int i = 0; i < tournamentFiles.size(); ++i)
    s.add (tournamentFiles.getReference(i).getFileNameWithoutExtension());
  
  return s;
}

void Tournament::refreshTournamentList()
{
  //I don't want the default tournament file in the list
  //so we will remove it once we've created the list
  const File remove (getTournamentsFolder().getChildFile (getDefaultFileName()).withFileExtension (getTournamentFileSuffix()));
  Array<File> newTournaments;
  getTournamentsFolder().findChildFiles (newTournaments, File::findFiles, false, getTournamentFileWildCard());
  
  if( ! newTournaments.isEmpty() && newTournaments.contains(remove) )
    newTournaments.removeFirstMatchingValue(remove);
  
  
  if (newTournaments != tournamentFiles)
  {
    tournamentFiles.swapWith (newTournaments);
  }
  
  tournamentList = setTournamentList();
  
}

//==============================================================================

String Tournament::getDefaultFileName()
{
  return "Defaults";
}

void Tournament::restoreDefaultTournamentFile()
{
  const File file (getTournamentsFolder().getChildFile (getDefaultFileName()).withFileExtension (getTournamentFileSuffix()));
  
  
  ScopedPointer<XmlElement> xml (XmlDocument::parse (BinaryData::default_tournament_xml));
  if (xml != nullptr)
    readFromXML (*xml);
  
  writeToFile (file);
}

void Tournament::setAsDefaults()
{
  const File file (getTournamentsFolder().getChildFile(getDefaultFileName()).withFileExtension(getTournamentFileSuffix()));
  
  if ( !file.exists() )
    file.create();
  
  writeToFile(file);
}

void Tournament::clear()
{
  File defaults = getTournamentsFolder().getChildFile(getDefaultFileName()).withFileExtension(getTournamentFileSuffix());
  
  //if defaults file doesn't exist, create it from stored defaults
  if ( ! defaults.existsAsFile())
  {
    restoreDefaultTournamentFile();
  }
  
  readFromFile(defaults);
}

//==============================================================================

void Tournament::addTeam(String teamName)
{
  const File file (Team::getTeamsFolder().getChildFile(teamName).withFileExtension(Team::getTeamFileSuffix()));
  
  //if the file doesn't exist, create a new team object with default settings
  if ( ! file.existsAsFile() )
  {
    Team* newTeam = new Team();
    newTeam->setTeamName(teamName);
    
    teams.add(newTeam);
    teamsAbvList.add(newTeam->getTeamAbv());
  }
  else //team settings file for this team exists, so we populate its data in the new team object
  {
    Team* newTeam = new Team(file);
    
    teams.add(newTeam);
    teamsAbvList.add(newTeam->getTeamAbv());
  }
  
  teamsList.addIfNotAlreadyThere(teamName); //teamsList is a StringArray containing the names of all teams at this tournament
}

void Tournament::addRound(String newRound)
{
  roundsList.add(newRound);
}

void Tournament::removeTeam(String teamName)
{
  if ( teamsList.contains(teamName) )
  {
    for ( int i = 0; i < teams.size(); i++ )
    {
      if ( teams[i]->getTeamName() == teamName )
      {
        teamsAbvList.removeString(teams[i]->getTeamAbv());
        teams.remove(i);
        break;
      }
    }
    
    teamsList.removeString(teamName);
  }
}

void Tournament::removeRound(String badRound)
{
  roundsList.removeString(badRound);
}

String Tournament::getTournamentName()
{
  return tournamentName;
}

String Tournament::getTournamentLocation()
{
  return tournamentLocation;
}

String Tournament::getTeamList()
{
  return teamsList.joinIntoString("|"); //returns a pipe delineated string of all teams at this tournament
}

StringArray Tournament::getRoundsList()
{
  return roundsList;
}

StringArray Tournament::getTeamAbvList()
{
  return teamsAbvList;
}

//==============================================================================

void Tournament::fillThisSucker(String name, String location, String rounds)
{
  tournamentName = name;
  tournamentLocation = location;
  roundsList.clear();
  roundsList.addLines(rounds);
  
  //once I add the "add teams" window I might move this to after that has been handled
  //so that we're not constantly writing out to files
  //for now, though, it goes here
  const File file (getTournamentsFolder().getChildFile(tournamentName).withFileExtension(getTournamentFileSuffix()));
  
  if ( !file.exists() )
    file.create();
  
  writeToFile(file);
}

void Tournament::fillThisSucker(String name, String location, String rounds, File pic)
{
  logo = ImageFileFormat::loadFrom(pic);
  fillThisSucker(name, location, rounds);
}

//==============================================================================

void Tournament::readFromXML (const XmlElement& xml)
{
  if ( xml.hasTagName("TOURNAMENT") )
  {
    forEachXmlChildElement(xml, e)
    {
      if ( e->hasTagName("name") )
      {
        tournamentName = e->getAllSubText();
      }
      else if ( e->hasTagName("location") )
      {
        tournamentLocation = e->getAllSubText();
      }
      else if ( e->hasTagName("rounds") )
      {
        roundsList.clear();
        roundsList.addTokens(e->getAllSubText(), "|", "");
      }
      else if ( e->hasTagName("teams") )
      {
        teams.clear();
        teamsList.clear();
        teamsList.addTokens(e->getAllSubText(), "|", "");
        
        for ( int i = 0; i < teamsList.size(); i++ )
        {
          addTeam(teamsList[i]);
        }
      }
      
      else if ( e->hasTagName("logo") )
      {
        if ( e->getAllSubText() != "NOLOGO" )
        {
          MemoryOutputStream imageData;
          Base64::convertFromBase64 (imageData, xml.getChildElementAllSubText ("logo", {}));
          logo = ImageFileFormat::loadFrom (imageData.getData(), imageData.getDataSize());
        }
      }
    }
  }
}

void Tournament::readFromFile (const File& file)
{
  const ScopedPointer<XmlElement> xml (XmlDocument::parse (file));
  readFromXML (*xml);
}

void Tournament::writeToFile (const File& file) const
{
  ScopedPointer<XmlElement> xml = new XmlElement ("TOURNAMENT");
  
  //default tournament name will ALWAYS be DEFAULT
  if ( file.getFileNameWithoutExtension() == getDefaultFileName() )
    xml->createNewChildElement("name")->addTextElement(defaultTournamentName);
  else
    xml->createNewChildElement("name")->addTextElement(tournamentName);
  
  xml->createNewChildElement("location")->addTextElement(tournamentLocation);
  
  xml->createNewChildElement("rounds")->addTextElement(roundsList.joinIntoString("|"));
  
  xml->createNewChildElement("teams")->addTextElement(teamsList.joinIntoString("|"));

  //teams will always already be files when this is called
  //recreating them each time is a lot of processing for no purpose
//  //also make sure each team has it's settings file
//  for ( int i = 0; i < teams.size(); i++ )
//  {
//    const File file(Team::getTeamsFolder().getChildFile(teams[i]->getTeamName())\
//            .withFileExtension(Team::getTeamFileSuffix()));
//    teams[i]->writeToFile(file);
//  }
  
  if ( logo.isValid() )
  {
    MemoryOutputStream imageData;
    if (PNGImageFormat().writeImageToStream (logo, imageData))
      xml->createNewChildElement ("logo")->addTextElement (Base64::toBase64 (imageData.getData(), imageData.getDataSize()));
//    else if (JPEGImageFormat().writeImageToStream(logo, imageData))
//      xml->createNewChildElement ("logo")->addTextElement (Base64::toBase64 (imageData.getData(), imageData.getDataSize()));
//    else if (GIFImageFormat().writeImageToStream(logo, imageData))
//      xml->createNewChildElement ("logo")->addTextElement (Base64::toBase64 (imageData.getData(), imageData.getDataSize()));
  }
  else
    xml->createNewChildElement("logo")->addTextElement("NOLOGO");
  
  xml->writeToFile (file, String());
}
