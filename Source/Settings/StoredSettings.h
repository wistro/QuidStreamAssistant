/*
  ==============================================================================

  StoredSettings.h
  Created: 27 May 2017 5:31:51pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include <map>
#include "OSDependencyThings.h"

//==============================================================================
class StoredSettings : public ValueTree::Listener
{
public:
  StoredSettings();
  ~StoredSettings();
    
  PropertiesFile& getGlobalProperties();
  PropertiesFile& getProjectProperties (const String& projectUID);
  
  void flush();
  void reload();
  
  //==============================================================================
  RecentlyOpenedFilesList recentFiles;
  
  Array<File> getLastProjects();
  void setLastProjects (const Array<File>& files);
  
  //==============================================================================
  Array<Colour> swatchColours;
  
  struct ColourSelectorWithSwatches  : public ColourSelector
  {
    ColourSelectorWithSwatches() {}
    
    int getNumSwatches() const override;
    Colour getSwatchColour (int index) const override;
    void setSwatchColour (int index, const Colour& newColour) override;
  };
  
  //==============================================================================

//  TeamSettings team;
  StringArray monospacedFontNames;
  
  //==============================================================================
  
private:
  OwnedArray<PropertiesFile> propertyFiles;
  ValueTree projectDefaults;
  
  void changed()
  {
    ScopedPointer<XmlElement> data (projectDefaults.createXml());
    propertyFiles.getUnchecked (0)->setValue ("PROJECT_DEFAULT_SETTINGS", data);
  }
  
  void updateGlobalPreferences();
  void updateAppearanceSettings();
  void updateRecentFiles();
  void updateKeyMappings();
    
  //==============================================================================
  void valueTreePropertyChanged (ValueTree&, const Identifier&) override  { changed(); }
  void valueTreeChildAdded (ValueTree&, ValueTree&) override              { changed(); }
  void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override       { changed(); }
  void valueTreeChildOrderChanged (ValueTree&, int, int) override         { changed(); }
  void valueTreeParentChanged (ValueTree&) override                       { changed(); }
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoredSettings)
};

StoredSettings& getAppSettings();
PropertiesFile& getGlobalProperties();
