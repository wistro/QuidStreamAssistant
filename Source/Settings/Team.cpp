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
#include "Player.h"

StringArray Team::teamList = {};

//==============================================================================

Team::Team ()
{
    File defaults = getTeamsFolder().getChildFile(getDefaultFileName()).withFileExtension(getTeamFileSuffix());
    
    //if defaults file doesn't exist, create it from stored defaults
    if ( ! defaults.existsAsFile())
    {
        ScopedPointer<XmlElement> xml (XmlDocument::parse (BinaryData::default_team_xml));
        if (xml != nullptr)
            readFromXML (*xml);
        
        writeToFile (defaults);
    }
    
    readFromFile(defaults);
}

File Team::getTeamsFolder()
{
    File f (getGlobalProperties().getFile().getSiblingFile ("Teams"));
    f.createDirectory();
    return f;
}

//==============================================================================


StringArray Team::setTeamList()
{
    StringArray s;
    for (int i = 0; i < teamFiles.size(); ++i)
        s.add (teamFiles.getReference(i).getFileNameWithoutExtension());
    
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
    
    
    if (newTeams != teamFiles)
    {
        teamFiles.swapWith (newTeams);
    }
    
    teamList = setTeamList();
}

//==============================================================================

String Team::getDefaultFileName()
{
    return "Defaults";
}

String Team::getTeamName()
{
    return teamName;
}

String Team::getTeamAbv()
{
    return teamAbv;
}

StringArray Team::getRoster()
{
    return roster;
}

void Team::refreshRoster()
{
    for ( int i = 0; i < team.size(); i++ )
    {
        roster.add(team[i]->getRosterEntry());
    }
}

//==============================================================================

void Team::addPlayer( Player* newPlayer )
{
    PlayerComparator sorter;
    team.addSorted(sorter, newPlayer);
}

void Team::removePlayer( Player* oldPlayer )
{
    team.removeObject(oldPlayer);
}

void Team::removePlayer( String number )
{
    for ( int i = 0; i < team.size(); i++ )
    {
        if ( team[i]->getNum() == number )
        {
            team.remove(i);
            break; //we're going to assume there can be only one player with a specific number
                   //this will be enforced elsewhere in the roster edit window
        }
    }
}

//void Team::editPlayer( Player* editMe )
//{
//    
//}
//
//void Team::editPlayer( String number )
//{
//    
//}

//==============================================================================

void Team::readFromXML (const XmlElement& xml)
{
    teamName = xml.getStringAttribute("name");
    teamAbv = xml.getStringAttribute("abv");
    
    forEachXmlChildElement(xml, e)
    {
        if ( e->hasTagName("PLAYER") )
        {
            bool keeper, chaser, beater, seeker;
            String positions;
            Player newPlayer;
            
            newPlayer.setNumber( e->getStringAttribute("number") );
            newPlayer.setName( e->getStringAttribute("first"), e->getStringAttribute("last"), e->getStringAttribute("jersey") );
            newPlayer.setPronouns( e->getStringAttribute("pronouns") );
            
            positions = e->getStringAttribute("positions");
            
            if ( positions == "utility" )
                keeper = chaser = beater = seeker = true;
            else
            {
                if ( positions.contains("keeper") )
                    keeper = true;
                if ( positions.contains("chaser") )
                    chaser = true;
                if ( positions.contains("beater") )
                    beater = true;
                if ( positions.contains("seeker") )
                    seeker = true;
            }
            
            newPlayer.setPositions( keeper, chaser, beater, seeker );
            
            addPlayer(&newPlayer);
            
        }
        else if ( e->hasTagName("LOGO") )
        {
            if ( e->getAllSubText() != "NOLOGO" )
            {
                MemoryOutputStream imageData;
                Base64::convertFromBase64 (imageData, xml.getChildElementAllSubText ("LOGO", {}));
                logo = ImageFileFormat::loadFrom (imageData.getData(), imageData.getDataSize());
            }
        }
    }
    
//    refreshRoster();
}

void Team::readFromFile (const File& file)
{
    const ScopedPointer<XmlElement> xml (XmlDocument::parse (file));
    readFromXML (*xml);
}

void Team::writeToFile (const File& file) const
{
    ScopedPointer<XmlElement> xml = new XmlElement ("TEAM");
    
    xml->setAttribute("name", teamName);
    xml->setAttribute("abv", teamAbv);
    
    for ( int i = 0; i < team.size(); i++ )
    {
        ScopedPointer<XmlElement> player = new XmlElement ("PLAYER");
        
        player->setAttribute ("first", team[i]->getFirst());
        player->setAttribute ("last", team[i]->getLast());
        player->setAttribute ("jersey", team[i]->getJersey());
        player->setAttribute("number", team[i]->getNum());
        player->setAttribute("pronouns", team[i]->getPronouns());
        
        if ( team[i]->isUtility() )
        {
            player->setAttribute("positions", "utility");
        }
        else
        {
            String positions = "";
            if ( team[i]->isKeeper() )
            {
                positions += "keeper|";
            }
            if ( team[i]->isChaser() )
            {
                positions += "chaser|";
            }
            if ( team[i]->isBeater() )
            {
                positions += "beater|";
            }
            if ( team[i]->isSeeker() )
            {
                positions += "seeker";
            }
            
            player->setAttribute("positions", positions);
        }
        
        
        // ..and add our new element to the parent node
        xml->addChildElement (player);
    }
    
    //do the team handling here once I've figured that out
    if ( logo.isValid() )
    {
        MemoryOutputStream imageData;
        if (PNGImageFormat().writeImageToStream (logo, imageData))
            xml->createNewChildElement ("LOGO")->addTextElement (Base64::toBase64 (imageData.getData(), imageData.getDataSize()));
    }
    else
        xml->createNewChildElement("LOGO")->addTextElement("NOLOGO");
    
    xml->writeToFile (file, String());
}
