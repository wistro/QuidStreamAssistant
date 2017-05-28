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

//==============================================================================
TeamSettings::TeamSettings (bool updateAppWhenChanged)
: settings ("COLOUR_SCHEME")
{
    if (! QuidStreamAssistantApplication::getApp().isRunningCommandLine)
    {
        ProjucerLookAndFeel lf;
        
        CodeDocument doc;
        CPlusPlusCodeTokeniser tokeniser;
        CodeEditorComponent editor (doc, &tokeniser);
        
        const CodeEditorComponent::ColourScheme cs (editor.getColourScheme());
        
        for (int i = cs.types.size(); --i >= 0;)
        {
            CodeEditorComponent::ColourScheme::TokenType& t = cs.types.getReference(i);
            getColourValue (t.name) = t.colour.toString();
        }
        
        getCodeFontValue() = getDefaultCodeFont().toString();
        
        if (updateAppWhenChanged)
            settings.addListener (this);
    }
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
    writeDefaultSchemeFile (BinaryData::colourscheme_dark_xml,  "Default (Dark)");
    writeDefaultSchemeFile (BinaryData::colourscheme_light_xml, "Default (Light)");
    
    Array<File> newSchemes;
    getSchemesFolder().findChildFiles (newSchemes, File::findFiles, false, String ("*") + getSchemeFileSuffix());
    
    if (newSchemes != presetSchemeFiles)
    {
        presetSchemeFiles.swapWith (newSchemes);
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
    if (xml.hasTagName (settings.getType().toString()))
    {
        const ValueTree newTeam (ValueTree::fromXml (xml));
        
        // we'll manually copy across the new properties to the existing tree so that
        // any open editors will be kept up to date..
        settings.copyPropertiesFrom (newTeam, nullptr);
        
        for (int i = settings.getNumChildren(); --i >= 0;)
        {
            ValueTree c (settings.getChild (i));
            
            const ValueTree newValue (newSettings.getChildWithProperty (Ids::name, c.getProperty (Ids::name)));
            
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
    const ScopedPointer<XmlElement> xml (settings.createXml());
    return xml != nullptr && xml->writeToFile (file, String());
}

Font TeamSettings::getDefaultCodeFont()
{
    return Font (Font::getDefaultMonospacedFontName(), Font::getDefaultStyle(), 13.0f);
}

StringArray TeamSettings::getColourNames() const
{
    StringArray s;
    
    for (int i = 0; i < settings.getNumChildren(); ++i)
    {
        const ValueTree c (settings.getChild(i));
        
        if (c.hasType ("COLOUR"))
            s.add (c [Ids::name]);
    }
    
    return s;
}

void TeamSettings::updateColourScheme()
{
    QuidStreamAssistantApplication::getApp().mainWindowList.sendLookAndFeelChange();
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
    editor.setFont (getCodeFont());
    
    editor.setColour (ScrollBar::thumbColourId, editor.findColour (CodeEditorComponent::backgroundColourId)
                      .contrasting()
                      .withAlpha (0.13f));
}

Font TeamSettings::getCodeFont() const
{
    const String fontString (settings [Ids::font].toString());
    
    if (fontString.isEmpty())
        return getDefaultCodeFont();
    
    return Font::fromString (fontString);
}

Value TeamSettings::getCodeFontValue()
{
    return settings.getPropertyAsValue (Ids::font, nullptr);
}

Value TeamSettings::getColourValue (const String& colourName)
{
    ValueTree c (settings.getChildWithProperty (Ids::name, colourName));
    
    if (! c.isValid())
    {
        c = ValueTree ("COLOUR");
        c.setProperty (Ids::name, colourName, nullptr);
        settings.addChild (c, -1, nullptr);
    }
    
    return c.getPropertyAsValue (Ids::colour, nullptr);
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
