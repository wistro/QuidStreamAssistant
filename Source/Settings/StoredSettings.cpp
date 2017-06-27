/*
  ==============================================================================

  StoredSettings.cpp
  Created: 27 May 2017 5:31:51pm
  Author:  Willow Rosenberg

  ==============================================================================
*/

#include "StoredSettings.h"
#include "OSDependencyThings.h"
#include "../TopLevel/Application.h"

const String StoredSettings::overlaysSettingName = "overlaysFolder";
const String StoredSettings::streamerSettingName = "streamerInfo";

//==============================================================================
StoredSettings& getAppSettings()
{
  return *QuidStreamAssistantApplication::getApp().settings;
}


PropertiesFile& getGlobalProperties()
{
  return getAppSettings().getGlobalProperties();
}

//==============================================================================
StoredSettings::StoredSettings()
:  projectDefaults ("PROJECT_DEFAULT_SETTINGS")
{
  reload();
  projectDefaults.addListener (this);
}

StoredSettings::~StoredSettings()
{
  projectDefaults.removeListener (this);
  flush();
}


PropertiesFile& StoredSettings::getGlobalProperties()
{
  return *propertyFiles.getUnchecked (0);
}

static PropertiesFile* createPropsFile (const String& filename)
{
  return new PropertiesFile (QuidStreamAssistantApplication::getApp()
                 .getPropertyFileOptionsFor (filename));
}

PropertiesFile& StoredSettings::getProjectProperties (const String& projectUID)
{
  const String filename ("QuidStreamAssistant" + projectUID);
  
  for (int i = propertyFiles.size(); --i >= 0;)
  {
    PropertiesFile* const props = propertyFiles.getUnchecked(i);
    if (props->getFile().getFileNameWithoutExtension() == filename)
      return *props;
  }
  
  PropertiesFile* p = createPropsFile (filename);
  propertyFiles.add (p);
  return *p;
}

void StoredSettings::updateGlobalPreferences()
{  
  // update 'invisible' global settings
  updateRecentFiles();
  updateKeyMappings();
}

void StoredSettings::updateRecentFiles()
{
  getGlobalProperties().setValue ("recentFiles", recentFiles.toString());
}

void StoredSettings::updateKeyMappings()
{
  getGlobalProperties().removeValue ("keyMappings");
  
  if (ApplicationCommandManager* commandManager = QuidStreamAssistantApplication::getApp().commandManager)
  {
    const ScopedPointer<XmlElement> keys (commandManager->getKeyMappings()->createXml (true));
    
    if (keys != nullptr)
      getGlobalProperties().setValue ("keyMappings", keys);
  }
}

void StoredSettings::flush()
{
  updateGlobalPreferences();
  
  for (int i = propertyFiles.size(); --i >= 0;)
    propertyFiles.getUnchecked(i)->saveIfNeeded();
}

void StoredSettings::reload()
{
  propertyFiles.clear();
  propertyFiles.add (createPropsFile ("QuidStreamAssistant"));
  
  ScopedPointer<XmlElement> projectDefaultsXml (propertyFiles.getFirst()->getXmlValue ("PROJECT_DEFAULT_SETTINGS"));
  
  if (projectDefaultsXml != nullptr)
    projectDefaults = ValueTree::fromXml (*projectDefaultsXml);
  
  // recent files...
  recentFiles.restoreFromString (getGlobalProperties().getValue ("recentFiles"));
  recentFiles.removeNonExistentFiles();
    
}

Array<File> StoredSettings::getLastProjects()
{
  StringArray s;
  s.addTokens (getGlobalProperties().getValue ("lastProjects"), "|", "");
  
  Array<File> f;
  for (int i = 0; i < s.size(); ++i)
    f.add (File (s[i]));
  
  return f;
}

void StoredSettings::setLastProjects (const Array<File>& files)
{
  StringArray s;
  for (int i = 0; i < files.size(); ++i)
    s.add (files.getReference(i).getFullPathName());
  
  getGlobalProperties().setValue ("lastProjects", s.joinIntoString ("|"));
}

//==============================================================================
