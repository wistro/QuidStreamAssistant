/*
  ==============================================================================

    MenuBar.h
    Created: 24 May 2017 4:20:05pm
    Author:  Willow Rosenberg

    Portions of this code taken from the JuceDemo libraries included with
    JUCE. The portions of this code from said demo are used under the GPLv3
    license under which this, and all other files in this project, are also licensed.
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainAppWindow.h"

class MenuBar : public Component,
                    public MenuBarModel,
                    public ChangeBroadcaster,
{
public:
    
    MenuBar();
    ~MenuBar();
    
    void resized() override;
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int menuIndex, const String& /*menuName*/) override;
    void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/) override;
    
    
private:
    ScopedPointer<MenuBarComponent> menuBar;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuBar)
};
