/*
==============================================================================

MainAppWindow.h
Created: 24 May 2017 5:20:28pm
Author:  Willow Rosenberg

==============================================================================
*/

#pragma once
#include "../Settings/OSDependencyThings.h"
#include "MainComponent.h"

//==============================================================================
/*
This class implements the desktop window that contains an instance of
our MainContentComponent class.
*/

class MainAppWindow   : public DocumentWindow,
                            public ApplicationCommandTarget
{
public:
    //==============================================================================
    MainAppWindow();
    ~MainAppWindow();
    
    static MainAppWindow* getMainAppWindow(); // returns the MainWindow if it exists.
    
    // called by the OS when the window's close button is pressed.
    void closeButtonPressed() override;
    
    // (return the command manager object used to dispatch command events)
    static ApplicationCommandManager& getApplicationCommandManager();
    
    /* Note: Be careful when overriding DocumentWindow methods - the base class
     uses a lot of them, so by overriding you might break its functionality.
     It's best to do all your work in you content component instead, but if
     you really have to override any DocumentWindow methods, make sure your
     implementation calls the superclass's method.
     */
    
    //==============================================================================
    enum CommandIDs
    {
        showPreviousDemo            = 0x2100,
        showNextDemo                = 0x2101,
        
        welcome                     = 0x2000,
        componentsAnimation         = 0x2001,
        componentsDialogBoxes       = 0x2002,
        componentsKeyMappings       = 0x2003,
        componentsMDI               = 0x2004,
        componentsPropertyEditors   = 0x2005,
        componentsTransforms        = 0x2006,
        componentsWebBrowsers       = 0x2007,
        componentsWidgets           = 0x2008,
        
        renderingEngineOne          = 0x2300,
        renderingEngineTwo          = 0x2301,
        renderingEngineThree        = 0x2302, // these must be contiguous!
        
        useLookAndFeelV1            = 0x300a,
        useLookAndFeelV2            = 0x300b,
        useLookAndFeelV3            = 0x300c,
        useLookAndFeelV4Dark        = 0x300d,
        useLookAndFeelV4Midnight    = 0x300e,
        useLookAndFeelV4Grey        = 0x300f,
        useLookAndFeelV4Light       = 0x3010,
        
        toggleRepaintDebugging      = 0x201a,
        
        useNativeTitleBar           = 0x201d,
        goToKioskMode               = 0x200f
    };
    
private:    
    ScopedPointer<Component> taskbarIcon;
    ScopedPointer<BubbleMessageComponent> currentBubbleMessage;
    
    ScopedPointer<MainContentComponent> content;
    
    TooltipWindow tooltipWindow; // to add tooltips to an application, you
                                 // just need to create one of these and leave it
                                 // there to do its work..
    LookAndFeel_V4 lookAndFeelV4;
        
    void showMessageBubble (const String&);
    
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands (Array<CommandID>& commands) override;
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override;
    bool perform (const InvocationInfo& info) override;
    bool isColourSchemeActive (LookAndFeel_V4::ColourScheme scheme);
    

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)
};

