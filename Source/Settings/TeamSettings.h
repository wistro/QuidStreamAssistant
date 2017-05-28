/*
  ==============================================================================

    TeamSettings.h
    Created: 27 May 2017 6:17:53pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once
#include "OSDependencyThings.h"

class TeamSettings    : private ValueTree::Listener
{
public:
    TeamSettings (bool updateAppWhenChanged);
    
    bool readFromFile (const File& file);
    bool readFromXML (const XmlElement&);
    bool writeToFile (const File& file) const;
    
    void updateColourScheme();
    void applyToCodeEditor (CodeEditorComponent& editor) const;
    
    StringArray getColourNames() const;
    Value getColourValue (const String& colourName);
    bool getColour (const String& name, Colour& resultIfFound) const;
    
    Font getCodeFont() const;
    Value getCodeFontValue();
    
    ValueTree settings;
    
    static File getTeamsFolder();
    StringArray getTeamList();
    void refreshTeamsList();
    void selectTeam (int index);
    
    static Font getDefaultCodeFont();
    
    static void showGlobalPreferences (ScopedPointer<Component>& ownerPointer);
    
    static const char* getTeamFileSuffix()      { return ".team"; }
    static const char* getTeamFileWildCard()    { return "*.team"; }
    
private:
    
    Array<File> teamFiles;
    
    static void writeDefaultTeamFile (const String& xml, const String& name);
    
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override   { updateColourScheme(); }
    void valueTreeChildAdded (ValueTree&, ValueTree&) override               { updateColourScheme(); }
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override        { updateColourScheme(); }
    void valueTreeChildOrderChanged (ValueTree&, int, int) override          { updateColourScheme(); }
    void valueTreeParentChanged (ValueTree&) override                        { updateColourScheme(); }
    void valueTreeRedirected (ValueTree&) override                           { updateColourScheme(); }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TeamSettings)
};
