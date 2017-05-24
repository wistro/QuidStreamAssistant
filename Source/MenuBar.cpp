/*
==============================================================================

MenuBar.cpp
Created: 24 May 2017 4:20:05pm
Author:  Willow Rosenberg

Portions of this code taken from the JuceDemo libraries included with
JUCE. The portions of this code from said demo are used under the GPLv3
license under which this, and all other files in this project, are also licensed.

==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "MenuBar.h"


MenuBar::MenuBar()
{
    addAndMakeVisible (menuBar = new MenuBarComponent (this));
    
    setApplicationCommandManagerToWatch (&MainAppWindow::getApplicationCommandManager());
    
    #if JUCE_MAC
    if (MenuBarModel::getMacMainMenu() != nullptr)
    {
        MenuBarModel::setMacMainMenu (nullptr);
        menuBar->setModel (this);
    }
    else
    {
        menuBar->setModel (nullptr);
        MenuBarModel::setMacMainMenu (this);
    }
    #endif
}

MenuBar::~MenuBar()
{
    #if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
    #endif
    
    PopupMenu::dismissAllActiveMenus();
    
}

void MenuBar::resized()
{
    Rectangle<int> area (getLocalBounds());
    menuBar->setBounds (area.removeFromTop (LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));
//    
//    area.removeFromTop (20);
//    area = area.removeFromTop (33);
}

//==============================================================================
StringArray MenuBar::getMenuBarNames()
{
    const char* const names[] = { "Demo", "Look-and-feel", "Tabs", "Misc", nullptr };
    
    return StringArray (names);
}

PopupMenu MenuBar::getMenuForIndex (int menuIndex, const String& /*menuName*/)
{
    ApplicationCommandManager* commandManager = &MainAppWindow::getApplicationCommandManager();
    
    PopupMenu menu;
    
    if (menuIndex == 0)
    {
        menu.addCommandItem (commandManager, MainAppWindow::showPreviousDemo);
        menu.addCommandItem (commandManager, MainAppWindow::showNextDemo);
        menu.addSeparator();
        menu.addCommandItem (commandManager, StandardApplicationCommandIDs::quit);
    }
    else if (menuIndex == 1)
    {
        menu.addCommandItem (commandManager, MainAppWindow::useLookAndFeelV1);
        menu.addCommandItem (commandManager, MainAppWindow::useLookAndFeelV2);
        menu.addCommandItem (commandManager, MainAppWindow::useLookAndFeelV3);
        
        PopupMenu v4SubMenu;
        v4SubMenu.addCommandItem (commandManager, MainAppWindow::useLookAndFeelV4Dark);
        v4SubMenu.addCommandItem (commandManager, MainAppWindow::useLookAndFeelV4Midnight);
        v4SubMenu.addCommandItem (commandManager, MainAppWindow::useLookAndFeelV4Grey);
        v4SubMenu.addCommandItem (commandManager, MainAppWindow::useLookAndFeelV4Light);
        
        menu.addSubMenu ("Use LookAndFeel_V4", v4SubMenu);
        
        menu.addSeparator();
        menu.addCommandItem (commandManager, MainAppWindow::useNativeTitleBar);
        
        #if ! JUCE_LINUX
        menu.addCommandItem (commandManager, MainAppWindow::goToKioskMode);
        #endif
        
    }
    else if (menuIndex == 2)
    {
        if (TabbedComponent* tabs = findParentComponentOfClass<TabbedComponent>())
        {
            menu.addItem (3000, "Tabs at Top",    true, tabs->getOrientation() == TabbedButtonBar::TabsAtTop);
            menu.addItem (3001, "Tabs at Bottom", true, tabs->getOrientation() == TabbedButtonBar::TabsAtBottom);
            menu.addItem (3002, "Tabs on Left",   true, tabs->getOrientation() == TabbedButtonBar::TabsAtLeft);
            menu.addItem (3003, "Tabs on Right",  true, tabs->getOrientation() == TabbedButtonBar::TabsAtRight);
        }
    }
    
    return menu;
}

void MenuBar::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
    // most of our menu items are invoked automatically as commands, but we can handle the
    // other special cases here..
    

    if (menuItemID >= 3000 && menuItemID <= 3003)
    {
        if (TabbedComponent* tabs = findParentComponentOfClass<TabbedComponent>())
        {
            TabbedButtonBar::Orientation o = TabbedButtonBar::TabsAtTop;
            
            if (menuItemID == 3001) o = TabbedButtonBar::TabsAtBottom;
            if (menuItemID == 3002) o = TabbedButtonBar::TabsAtLeft;
            if (menuItemID == 3003) o = TabbedButtonBar::TabsAtRight;
            
            tabs->setOrientation (o);
        }
    }
    else if (menuItemID >= 12298 && menuItemID <= 12305)
    {
        sendChangeMessage();
    }
}

