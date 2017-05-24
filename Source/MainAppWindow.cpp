/*
  ==============================================================================

    MainAppWindow.cpp
    Created: 24 May 2017 5:20:28pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "MainAppWindow.h"

//==============================================================================
#if JUCE_WINDOWS || JUCE_LINUX || JUCE_MAC

// Just add a simple icon to the Window system tray area or Mac menu bar..
struct TaskbarComponent  : public SystemTrayIconComponent,
                            private Timer
{
    TaskbarComponent()
    {
        setIconImage (ImageCache::getFromMemory (BinaryData::willogo_png, BinaryData::willogo_pngSize));
        setIconTooltip ("Juce Demo App!");
    }
    
    void mouseDown (const MouseEvent&) override
    {
        // On OSX, there can be problems launching a menu when we're not the foreground
        // process, so just in case, we'll first make our process active, and then use a
        // timer to wait a moment before opening our menu, which gives the OS some time to
        // get its act together and bring our windows to the front.
        
        Process::makeForegroundProcess();
        startTimer (50);
    }
    
    // This is invoked when the menu is clicked or dismissed
    static void menuInvocationCallback (int chosenItemID, TaskbarComponent*)
    {
        if (chosenItemID == 1)
            JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    void timerCallback() override
    {
        stopTimer();
        
        PopupMenu m;
        m.addItem (1, "Quit the Juce demo");
        
        // It's always better to open menus asynchronously when possible.
        m.showMenuAsync (PopupMenu::Options(),
                         ModalCallbackFunction::forComponent (menuInvocationCallback, this));
    }
};

#endif
//==============================================================================
static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

MainAppWindow::MainAppWindow()
            : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                  LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                  DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setResizable (true, false);
    setResizeLimits (400, 400, 10000, 10000);
    
    #if JUCE_IOS || JUCE_ANDROID
    setFullScreen (true);
    #else
    setBounds ((int) (0.1f * getParentWidth()),
               (int) (0.1f * getParentHeight()),
               jmax (850, (int) (0.5f * getParentWidth())),
               jmax (600, (int) (0.7f * getParentHeight())));
    #endif
    
    
    #if JUCE_WINDOWS || JUCE_LINUX || JUCE_MAC
    taskbarIcon = new TaskbarComponent();
    #endif
    
#if JUCE_ANDROID
    setOpenGLRenderingEngine();
#endif
    
    triggerAsyncUpdate();
}

MainAppWindow::~MainAppWindow()
{
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}
MainAppWindow* MainAppWindow::getMainAppWindow()
{
    for (int i = TopLevelWindow::getNumTopLevelWindows(); --i >= 0;)
        if (auto* maw = dynamic_cast<MainAppWindow*> (TopLevelWindow::getTopLevelWindow (i)))
            return maw;
    
    return nullptr;
}

void MainAppWindow::showMessageBubble (const String& text)
{
    currentBubbleMessage = new BubbleMessageComponent (500);
    getContentComponent()->addChildComponent (currentBubbleMessage);
    
    AttributedString attString;
    attString.append (text, Font (15.0f));
    
    currentBubbleMessage->showAt ({ getLocalBounds().getCentreX(), 10, 1, 1 },
                                  attString,
                                  500,  // numMillisecondsBeforeRemoving
                                  true,  // removeWhenMouseClicked
                                  false); // deleteSelfAfterUse
}


//==============================================================================
// The following methods implement the ApplicationCommandTarget interface, allowing
// this window to publish a set of actions it can perform, and which can be mapped
// onto menus, keypresses, etc.

ApplicationCommandTarget* getNextCommandTarget()
{
    // this will return the next parent component that is an ApplicationCommandTarget (in this
    // case, there probably isn't one, but it's best to use this method in your own apps).
    return findFirstTargetParentComponent();
}

void MainAppWindow::getAllCommands (Array<CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] = { MainAppWindow::showPreviousDemo,
        MainAppWindow::showNextDemo,
        MainAppWindow::welcome,
        MainAppWindow::componentsAnimation,
        MainAppWindow::componentsDialogBoxes,
        MainAppWindow::componentsKeyMappings,
        MainAppWindow::componentsMDI,
        MainAppWindow::componentsPropertyEditors,
        MainAppWindow::componentsTransforms,
        MainAppWindow::componentsWebBrowsers,
        MainAppWindow::componentsWidgets,
        MainAppWindow::useLookAndFeelV1,
        MainAppWindow::useLookAndFeelV2,
        MainAppWindow::useLookAndFeelV3,
        MainAppWindow::useLookAndFeelV4Dark,
        MainAppWindow::useLookAndFeelV4Midnight,
        MainAppWindow::useLookAndFeelV4Grey,
        MainAppWindow::useLookAndFeelV4Light,
        MainAppWindow::toggleRepaintDebugging,
        #if ! JUCE_LINUX
        MainAppWindow::goToKioskMode,
        #endif
        MainAppWindow::useNativeTitleBar
    };
    
    commands.addArray (ids, numElementsInArray (ids));
    
    const CommandID engineIDs[] = { MainAppWindow::renderingEngineOne,
        MainAppWindow::renderingEngineTwo,
        MainAppWindow::renderingEngineThree };
    
    auto renderingEngines = MainAppWindow::getMainAppWindow()->getRenderingEngines();
    commands.addArray (engineIDs, renderingEngines.size());
}

void MainAppWindow::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    const String generalCategory ("General");
    const String demosCategory ("Demos");
    
    switch (commandID)
    {
        case MainAppWindow::showPreviousDemo:
            result.setInfo ("Previous Demo", "Shows the previous demo in the list", demosCategory, 0);
            result.addDefaultKeypress ('-', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::showNextDemo:
            result.setInfo ("Next Demo", "Shows the next demo in the list", demosCategory, 0);
            result.addDefaultKeypress ('=', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::welcome:
            result.setInfo ("Welcome Demo", "Shows the 'Welcome' demo", demosCategory, 0);
            result.addDefaultKeypress ('1', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::componentsAnimation:
            result.setInfo ("Animation Demo", "Shows the 'Animation' demo", demosCategory, 0);
            result.addDefaultKeypress ('2', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::componentsDialogBoxes:
            result.setInfo ("Dialog Boxes Demo", "Shows the 'Dialog Boxes' demo", demosCategory, 0);
            result.addDefaultKeypress ('3', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::componentsKeyMappings:
            result.setInfo ("Key Mappings Demo", "Shows the 'Key Mappings' demo", demosCategory, 0);
            result.addDefaultKeypress ('4', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::componentsMDI:
            result.setInfo ("Multi-Document Demo", "Shows the 'Multi-Document' demo", demosCategory, 0);
            result.addDefaultKeypress ('5', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::componentsPropertyEditors:
            result.setInfo ("Property Editor Demo", "Shows the 'Property Editor' demo", demosCategory, 0);
            result.addDefaultKeypress ('6', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::componentsTransforms:
            result.setInfo ("Component Transforms Demo", "Shows the 'Transforms' demo", demosCategory, 0);
            result.addDefaultKeypress ('7', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::componentsWebBrowsers:
            result.setInfo ("Web Browser Demo", "Shows the 'Web Browser' demo", demosCategory, 0);
            result.addDefaultKeypress ('8', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::componentsWidgets:
            result.setInfo ("Widgets Demo", "Shows the 'Widgets' demo", demosCategory, 0);
            result.addDefaultKeypress ('9', ModifierKeys::commandModifier);
            break;
            
        case MainAppWindow::renderingEngineOne:
        case MainAppWindow::renderingEngineTwo:
        case MainAppWindow::renderingEngineThree:
        {
            auto& mainWindow = *MainAppWindow::getMainAppWindow();
            auto engines = mainWindow.getRenderingEngines();
            const int index = commandID - MainAppWindow::renderingEngineOne;
            
            result.setInfo ("Use " + engines[index], "Uses the " + engines[index] + " engine to render the UI", generalCategory, 0);
            result.setTicked (mainWindow.getActiveRenderingEngine() == index);
            
            result.addDefaultKeypress ('1' + index, ModifierKeys::noModifiers);
            break;
        }
            
        case MainAppWindow::useLookAndFeelV1:
            result.setInfo ("Use LookAndFeel_V1", String(), generalCategory, 0);
            result.addDefaultKeypress ('i', ModifierKeys::commandModifier);
            result.setTicked (isLookAndFeelSelected<LookAndFeel_V1>());
            break;
            
        case MainAppWindow::useLookAndFeelV2:
            result.setInfo ("Use LookAndFeel_V2", String(), generalCategory, 0);
            result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
            result.setTicked (isLookAndFeelSelected<LookAndFeel_V2>());
            break;
            
        case MainAppWindow::useLookAndFeelV3:
            result.setInfo ("Use LookAndFeel_V3", String(), generalCategory, 0);
            result.addDefaultKeypress ('p', ModifierKeys::commandModifier);
            result.setTicked (isLookAndFeelSelected<LookAndFeel_V3>());
            break;
            
        case MainAppWindow::useLookAndFeelV4Dark:
            result.setInfo ("Use LookAndFeel_V4 Dark", String(), generalCategory, 0);
            result.addDefaultKeypress ('k', ModifierKeys::commandModifier);
            result.setTicked (isColourSchemeActive (LookAndFeel_V4::getDarkColourScheme()));
            break;
            
        case MainAppWindow::useLookAndFeelV4Midnight:
            result.setInfo ("Use LookAndFeel_V4 Midnight", String(), generalCategory, 0);
            result.setTicked (isColourSchemeActive (LookAndFeel_V4::getMidnightColourScheme()));
            break;
            
        case MainAppWindow::useLookAndFeelV4Grey:
            result.setInfo ("Use LookAndFeel_V4 Grey", String(), generalCategory, 0);
            result.setTicked (isColourSchemeActive (LookAndFeel_V4::getGreyColourScheme()));
            break;
            
        case MainAppWindow::useLookAndFeelV4Light:
            result.setInfo ("Use LookAndFeel_V4 Light", String(), generalCategory, 0);
            result.setTicked (isColourSchemeActive (LookAndFeel_V4::getLightColourScheme()));
            break;
            
        case MainAppWindow::toggleRepaintDebugging:
            result.setInfo ("Toggle repaint display", String(), generalCategory, 0);
            result.addDefaultKeypress ('r', ModifierKeys());
            result.setTicked (juceDemoRepaintDebuggingActive);
            break;
            
        case MainAppWindow::useNativeTitleBar:
        {
            result.setInfo ("Use native window title bar", String(), generalCategory, 0);
            result.addDefaultKeypress ('n', ModifierKeys::commandModifier);
            bool nativeTitlebar = false;
            
            if (auto* mainWindow = MainAppWindow::getMainAppWindow())
                nativeTitlebar = mainWindow->isUsingNativeTitleBar();
            
            result.setTicked (nativeTitlebar);
            break;
        }
            
#if ! JUCE_LINUX
        case MainAppWindow::goToKioskMode:
            result.setInfo ("Show full-screen kiosk mode", String(), generalCategory, 0);
            result.addDefaultKeypress ('f', ModifierKeys::commandModifier);
            result.setTicked (Desktop::getInstance().getKioskModeComponent() != 0);
            break;
#endif
            
        default:
            break;
    }
}

bool MainAppWindow::perform (const InvocationInfo& info)
{
    if (auto* mainWindow = MainAppWindow::getMainAppWindow())
    {
        switch (info.commandID)
        {
            case MainAppWindow::showPreviousDemo:   moveDemoPages (-1); break;
            case MainAppWindow::showNextDemo:       moveDemoPages ( 1); break;
                
            case MainAppWindow::welcome:
            case MainAppWindow::componentsAnimation:
            case MainAppWindow::componentsDialogBoxes:
            case MainAppWindow::componentsKeyMappings:
            case MainAppWindow::componentsMDI:
            case MainAppWindow::componentsPropertyEditors:
            case MainAppWindow::componentsTransforms:
            case MainAppWindow::componentsWebBrowsers:
            case MainAppWindow::componentsWidgets:
                demoList.selectRow (info.commandID - MainAppWindow::welcome);
                break;
                
            case MainAppWindow::renderingEngineOne:
            case MainAppWindow::renderingEngineTwo:
            case MainAppWindow::renderingEngineThree:
                mainWindow->setRenderingEngine (info.commandID - MainAppWindow::renderingEngineOne);
                break;
                
            case MainAppWindow::useLookAndFeelV1:
                LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV1);
                updateDemoListColours();
                break;
            case MainAppWindow::useLookAndFeelV2:
                LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV2);
                updateDemoListColours();
                break;
            case MainAppWindow::useLookAndFeelV3:
                LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV3);
                updateDemoListColours();
                break;
            case MainAppWindow::useLookAndFeelV4Dark:
                lookAndFeelV4.setColourScheme (LookAndFeel_V4::getDarkColourScheme());
                LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV4);
                updateDemoListColours();
                break;
            case MainAppWindow::useLookAndFeelV4Midnight:
                lookAndFeelV4.setColourScheme (LookAndFeel_V4::getMidnightColourScheme());
                LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV4);
                updateDemoListColours();
                break;
            case MainAppWindow::useLookAndFeelV4Grey:
                lookAndFeelV4.setColourScheme (LookAndFeel_V4::getGreyColourScheme());
                LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV4);
                updateDemoListColours();
                break;
            case MainAppWindow::useLookAndFeelV4Light:
                lookAndFeelV4.setColourScheme (LookAndFeel_V4::getLightColourScheme());
                LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV4);
                updateDemoListColours();
                break;
                
            case MainAppWindow::toggleRepaintDebugging:
                juceDemoRepaintDebuggingActive = ! juceDemoRepaintDebuggingActive;
                mainWindow->repaint();
                break;
                
            case MainAppWindow::useNativeTitleBar:
                mainWindow->setUsingNativeTitleBar (! mainWindow->isUsingNativeTitleBar());
                break;
                
            #if ! JUCE_LINUX
            case MainAppWindow::goToKioskMode:
            {
                auto& desktop = Desktop::getInstance();
                
                if (desktop.getKioskModeComponent() == nullptr)
                    desktop.setKioskModeComponent (getTopLevelComponent());
                else
                    desktop.setKioskModeComponent (nullptr);
                
                break;
            }
            #endif
                
            default:
                return false;
        }
    }
    
    return true;
}


