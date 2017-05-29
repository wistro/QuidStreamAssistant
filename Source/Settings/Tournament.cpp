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
    Array<File> newTournaments;
    getTournamentsFolder().findChildFiles (newTournaments, File::findFiles, false, getTournamentFileWildCard());
    
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
    
    //I should do something with the result of this, in case the write operation fails
    //but I don't feel like it so soz
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

void Tournament::readFromXML (const XmlElement& xml)
{
    tournamentName = xml.getChildElementAllSubText("name", {});
    tournamentLocation = xml.getChildElementAllSubText("location", {});
    
    XmlElement* rounds = xml.getChildByName("rounds");
    
    if ( rounds != nullptr )
    {
        forEachXmlChildElement(*rounds, child)
        {
            if(child->hasTagName("rname"))
                addRound(child->getAllSubText());
        }
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
    
    
    if ( xml.getChildElementAllSubText("logo", {}) != "NOLOGO" )
    {
        MemoryOutputStream imageData;
        Base64::convertFromBase64 (imageData, xml.getChildElementAllSubText ("logo", {}));
        logo = ImageFileFormat::loadFrom (imageData.getData(), imageData.getDataSize());
    }
    
}

void Tournament::readFromFile (const File& file)
{
    const ScopedPointer<XmlElement> xml (XmlDocument::parse (file));
    readFromXML (*xml);
}

void Tournament::writeToFile (const File& file) const
{
    XmlElement* xml = new XmlElement ("TOURNAMENT");
    
    xml->createNewChildElement("name")->addTextElement(tournamentName);
    xml->createNewChildElement("location")->addTextElement(tournamentLocation);
    
    XmlElement* rounds = new XmlElement ("rounds");
    
    for( int i = 0; i < roundsList.size(); i++)
    {
        rounds->createNewChildElement("rname")->addTextElement(roundsList.operator[](i));
    }
    
    xml->addChildElement(rounds);
    
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
