/*
  ==============================================================================

    TeamDatabase.cpp
    Created: 28 May 2017 10:17:40pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "Team.h"

#include "OSDependencyThings.h"
#include "StoredSettings.h"
#include "../TopLevel/Application.h"


const String Team::consolation = "Consolation";

const StringArray Team::consolationBracket =
{   "Consolation Round of 16",
    "Consolation Quarter Finals",
    "Consolation Semi Finals",
    "Consolation 3rd Place Playoff",
    "Consolation Finals"
};

//==============================================================================
Team::Team ()
{
    File defaults = getTeamsFolder().getChildFile(getDefaultFileName()).withFileExtension(getTeamFileSuffix());
    
    //if defaults file doesn't exist, create it from stored defaults
    if ( ! defaults.existsAsFile())
    {
        restoreDefaultTeamFile();
    }
    
    readFromFile(defaults);
}

File Team::getTeamsFolder()
{
    File f (getGlobalProperties().getFile().getSiblingFile ("Teams"));
    f.createDirectory();
    return f;
}

StringArray Team::setTeamList()
{
    
    
    StringArray s;
    for (int i = 0; i < tournamentFiles.size(); ++i)
        s.add (tournamentFiles.getReference(i).getFileNameWithoutExtension());
    
    return s;
}

void Team::refreshTeamList()
{
    //I don't want the default tournament file in the list
    //so we will remove it once we've created the list
    const File remove (getTeamsFolder().getChildFile (getDefaultFileName()).withFileExtension (getTeamFileSuffix()));
    Array<File> newTeams;
    getTeamsFolder().findChildFiles (newTeams, File::findFiles, false, getTeamFileWildCard());
    
    if( ! newTeams.isEmpty() && newTeams.contains(remove) )
        newTeams.removeFirstMatchingValue(remove);
    
    
    if (newTeams != tournamentFiles)
    {
        tournamentFiles.swapWith (newTeams);
    }
    
    tournamentList = setTeamList();
    
}

String Team::getDefaultFileName()
{
    return "Defaults";
}

void Team::restoreDefaultTeamFile()
{
    const File file (getTeamsFolder().getChildFile (getDefaultFileName()).withFileExtension (getTeamFileSuffix()));
    
    
    ScopedPointer<XmlElement> xml (XmlDocument::parse (BinaryData::default_tournament_xml));
    if (xml != nullptr)
        readFromXML (*xml);
    
    writeToFile (file);
}

void Team::setAsDefaults()
{
    const File file (getTeamsFolder().getChildFile(getDefaultFileName()).withFileExtension(getTeamFileSuffix()));
    
    if ( !file.exists() )
        file.create();
    
    writeToFile(file);
}

//void Team::addTeam()
//{
//    
//}

void Team::addRound(String newRound)
{
    roundsList.add(newRound);
}

//void Team::removeTeam()
//{
//    
//}

void Team::removeRound(String badRound)
{
    roundsList.removeString(badRound);
}

String Team::getTeamName()
{
    return tournamentName;
}

String Team::getTeamLocation()
{
    return tournamentLocation;
}

//StringArray Team::getTeamsList()
//{
//    
//}

StringArray Team::getRoundsList()
{
    return roundsList;
}

void Team::fillThisSucker(String name, String location, String rounds)
{
    tournamentName = name;
    tournamentLocation = location;
    roundsList.clear();
    roundsList.addLines(rounds);
    
    //once I add the "add teams" window I might move this to after that has been handled
    //so that we're not constantly writing out to files
    //for now, though, it goes here
    const File file (getTeamsFolder().getChildFile(tournamentName).withFileExtension(getTeamFileSuffix()));
    
    if ( !file.exists() )
        file.create();
    
    writeToFile(file);
}

void Team::fillThisSucker(String name, String location, String rounds, File pic)
{
    logo = ImageFileFormat::loadFrom(pic);
    fillThisSucker(name, location, rounds);
}

void Team::readFromXML (const XmlElement& xml)
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

void Team::readFromFile (const File& file)
{
    const ScopedPointer<XmlElement> xml (XmlDocument::parse (file));
    readFromXML (*xml);
}

void Team::writeToFile (const File& file) const
{
    ScopedPointer<XmlElement> xml = new XmlElement ("TOURNAMENT");
    
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
