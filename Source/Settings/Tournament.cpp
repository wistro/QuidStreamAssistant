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


const String Tournament::consolation = "Consolation";

const StringArray Tournament::consolationBracket =
{   "Consolation Round of 16",
    "Consolation Quarter Finals",
    "Consolation Semi Finals",
    "Consolation 3rd Place Playoff",
    "Consolation Finals"
};

const String Tournament::defaultTournamentName = "DEFAULT";
//==============================================================================
Tournament::Tournament ()
{
    File defaults = getTournamentsFolder().getChildFile(getDefaultFileName()).withFileExtension(getTournamentFileSuffix());
    
    //if defaults file doesn't exist, create it from stored defaults
    if ( ! defaults.existsAsFile())
    {
        restoreDefaultTournamentFile();
    }
    
    readFromFile(defaults);
}

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

//void Tournament::addTeam()
//{
//    
//}

void Tournament::addRound(String newRound)
{
    roundsList.add(newRound);
}

//void Tournament::removeTeam()
//{
//    
//}

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

//StringArray Tournament::getTeamsList()
//{
//    
//}

StringArray Tournament::getRoundsList()
{
    return roundsList;
}

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

void Tournament::readFromXML (const XmlElement& xml)
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
        
        //    delete rounds;
        
        //    XmlElement* teams = xml.getChildByName("teams");
        //
        //    if ( teams != nullptr )
        //    {
        //        forEachXmlChildElement(*teams, child)
        //        {
        //            if(child->hasTagName("tname"))
        //                addTeam
        //        }
        //    }
        
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
    
    //do the team handling here once I've figured that out
    if ( logo.isValid() )
    {
        MemoryOutputStream imageData;
        if (PNGImageFormat().writeImageToStream (logo, imageData))
            xml->createNewChildElement ("logo")->addTextElement (Base64::toBase64 (imageData.getData(), imageData.getDataSize()));
    }
    else
        xml->createNewChildElement("logo")->addTextElement("NOLOGO");
    
    xml->writeToFile (file, String());
}
