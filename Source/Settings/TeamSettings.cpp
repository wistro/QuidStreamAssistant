/*
  ==============================================================================

    TeamSettings.cpp
    Created: 27 May 2017 6:17:53pm
    Author:  Willow Rosenberg

    //use this for making and storing settings files I think, will have to read through and look later
 
  ==============================================================================
*/

#include "OSDependencyThings.h"
#include "TeamSettings.h"
#include "StoredSettings.h"
#include "../TopLevel/Application.h"
#include "Player.h"

//==============================================================================
TeamSettings::TeamSettings (bool updateAppWhenChanged)
: teamData ("TEAM")
{
    if (updateAppWhenChanged)
        teamData.addListener (this);
}

File TeamSettings::getTeamsFolder()
{
    File f (getGlobalProperties().getFile().getSiblingFile ("Teams"));
    f.createDirectory();
    return f;
}

void TeamSettings::writeDefaultTeamFile (const String& xmlString, const String& name)
{
    const File file (getTeamsFolder().getChildFile (name).withFileExtension (getTeamFileSuffix()));
    
    TeamSettings settings (false);
    
    ScopedPointer<XmlElement> xml (XmlDocument::parse (xmlString));
    if (xml != nullptr)
        settings.readFromXML (*xml);
    
    settings.writeToFile (file);
}

void TeamSettings::refreshTeamsList()
{
    writeDefaultTeamFile (BinaryData::default_team_xml,  "Default Team");
    
    Array<File> newTeams;
    getTeamsFolder().findChildFiles (newTeams, File::findFiles, false, String ("*") + getTeamFileSuffix());
    
    if (newTeams != teamFiles)
    {
        teamFiles.swapWith (newTeams);
        QuidStreamAssistantApplication::getCommandManager().commandStatusChanged();
    }
}

StringArray TeamSettings::getTeamList()
{
    StringArray s;
    for (int i = 0; i < teamFiles.size(); ++i)
        s.add (teamFiles.getReference(i).getFileNameWithoutExtension());
    
    return s;
}

void TeamSettings::selectTeam (int index)
{
    readFromFile (teamFiles [index]);
}

bool TeamSettings::readFromXML (const XmlElement& xml)
{
    if (xml.hasTagName (teamData.getType().toString()))
    {
        const ValueTree newTeam (ValueTree::fromXml (xml));
        
        // we'll manually copy across the new properties to the existing tree so that
        // any open editors will be kept up to date..
        teamData.copyPropertiesFrom (newTeam, nullptr);
        
        for (int i = teamData.getNumChildren(); --i >= 0;)
        {
            ValueTree c (teamData.getChild (i));
            
            const ValueTree newValue (newTeam.getChildWithProperty (Ids::name, c.getProperty (Ids::name)));
            
            if (newValue.isValid())
                c.copyPropertiesFrom (newValue, nullptr);
        }
        
        return true;
    }
    
    return false;
}

bool TeamSettings::readFromFile (const File& file)
{
    const ScopedPointer<XmlElement> xml (XmlDocument::parse (file));
    return xml != nullptr && readFromXML (*xml);
}

bool TeamSettings::writeToFile (const File& file) const
{
    const ScopedPointer<XmlElement> xml (teamData.createXml());
    return xml != nullptr && xml->writeToFile (file, String());
}

String TeamSettings::getDefaultTeamName()
{
    return "Default Team";
}

StringArray TeamSettings::getPlayerNumbers() const
{
    StringArray s;
    
    for (int i = 0; i < teamData.getNumChildren(); ++i)
    {
        const ValueTree c (teamData.getChild(i));
        
        if (c.hasType ("PLAYER"))
            s.add (c [Ids::number]);
    }
    
    return s;
}

void TeamSettings::updateTeamsList()
{
    teamsList = getTeamList();
}

void TeamSettings::applyToCodeEditor (CodeEditorComponent& editor) const
{
    CodeEditorComponent::ColourScheme cs (editor.getColourScheme());
    
    for (int i = cs.types.size(); --i >= 0;)
    {
        CodeEditorComponent::ColourScheme::TokenType& t = cs.types.getReference(i);
        getColour (t.name, t.colour);
    }
    
    editor.setColourScheme (cs);
    
    editor.setColour (ScrollBar::thumbColourId, editor.findColour (CodeEditorComponent::backgroundColourId)
                      .contrasting()
                      .withAlpha (0.13f));
}

String TeamSettings::getTeamName() const
{
    const String teamName (teamData [Ids::team].toString());
    
    if (teamName.isEmpty())
        return getDefaultTeamName();
    
    return teamName;
}


Value TeamSettings::getPlayerInfo (Player& player)
{
    ValueTree c (teamData.getChildWithProperty (Ids::number, player.getNum()));
    
    if (! c.isValid())
    {
        c = ValueTree ("PLAYER");
        c.setProperty (Ids::number, playerNumber, nullptr);
        teamData.addChild (c, -1, nullptr);
    }
    
    return c.getPropertyAsValue (Ids::number, nullptr);
}

bool TeamSettings::getColour (const String& name, Colour& result) const
{
    const ValueTree colour (settings.getChildWithProperty (Ids::name, name));
    
    if (colour.isValid())
    {
        result = Colour::fromString (colour [Ids::colour].toString());
        return true;
    }
    
    return false;
}
