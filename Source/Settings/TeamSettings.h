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
    
    void updateTeamsList();
    void applyToCodeEditor (CodeEditorComponent& editor) const;
    
    StringArray getPlayerNumbers() const;
    Value getPlayerInfo (const String& playerNumber);
    bool getColour (const String& name, Colour& resultIfFound) const;
    
    String getTeamName() const;
    
    ValueTree teamData;
    
    static File getTeamsFolder();
    StringArray getTeamList();
    void refreshTeamsList();
    void selectTeam (int index);
    
    static String getDefaultTeamName();
    
    static void showGlobalPreferences (ScopedPointer<Component>& ownerPointer);
    
    static const char* getTeamFileSuffix()      { return ".team"; }
    static const char* getTeamFileWildCard()    { return "*.team"; }
    static StringArray teamsList;
    
private:
    
    Array<File> teamFiles;
    
    static void writeDefaultTeamFile (const String& xml, const String& name);
    
    void valueTreePropertyChanged (ValueTree&, const Identifier&) override   { updateTeamsList(); }
    void valueTreeChildAdded (ValueTree&, ValueTree&) override               { updateTeamsList(); }
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override        { updateTeamsList(); }
    void valueTreeChildOrderChanged (ValueTree&, int, int) override          { updateTeamsList(); }
    void valueTreeParentChanged (ValueTree&) override                        { updateTeamsList(); }
    void valueTreeRedirected (ValueTree&) override                           { updateTeamsList(); }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TeamSettings)
};
