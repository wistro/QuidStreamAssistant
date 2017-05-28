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
Tournament::Tournament (bool updateAppWhenChanged)
: tournament("TOURNAMENT")
{
    
    if (updateAppWhenChanged)
        tournament.addListener (this);
    
    refreshTournamentList();
    tournamentList = setTournamentList();
    
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
        QuidStreamAssistantApplication::getCommandManager().commandStatusChanged();
    }
}

void Tournament::restoreDefaultTournamentFile()
{
    const File file (getTournamentsFolder().getChildFile ("Defaults").withFileExtension (getTournamentFileSuffix()));
    
    Tournament defaults (false);
    
    ScopedPointer<XmlElement> xml (XmlDocument::parse (BinaryData::default_tournament_xml));
    if (xml != nullptr)
        defaults.readFromXML (*xml);
    
    defaults.writeToFile (file);
}

void Tournament::setAsDefaults()
{
    Value v (tournament.getPropertyAsValue(Ids::name, nullptr));
    const File file (getTournamentsFolder().getChildFile(v.toString()).withFileExtension(getTournamentFileSuffix()));
    
    if ( !file.exists() )
        file.create();
    
    //I should do something with the result of this, in case the write operation fails
    //but I don't feel like it so soz
    writeToFile(file);
}

void Tournament::updateTournament()
{
    
}

bool Tournament::readFromXML (const XmlElement& xml)
{
    if (xml.hasTagName (tournament.getType().toString()))
    {
        const ValueTree newTournament (ValueTree::fromXml (xml));
        
        // we'll manually copy across the new properties to the existing tree so that
        // any open editors will be kept up to date..
        tournament.copyPropertiesFrom (newTournament, nullptr);
        
        for (int i = tournament.getNumChildren(); --i >= 0;)
        {
            ValueTree c (tournament.getChild (i));
            
            const ValueTree newValue (newTournament.getChildWithProperty (Ids::round, c.getProperty (Ids::round)));
            
            if (newValue.isValid())
                c.copyPropertiesFrom (newValue, nullptr);
        }
        
        return true;
    }
    
    return false;
}

bool Tournament::readFromFile (const File& file)
{
    const ScopedPointer<XmlElement> xml (XmlDocument::parse (file));
    return xml != nullptr && readFromXML (*xml);
}

bool Tournament::writeToFile (const File& file) const
{
    const ScopedPointer<XmlElement> xml (tournament.createXml());
    return xml != nullptr && xml->writeToFile (file, String());
}
