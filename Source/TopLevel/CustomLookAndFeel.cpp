/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 25 May 2017 9:29:11pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "CustomLookAndFeel.h"

//==============================================================================

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel() : currentColourScheme (getLightColourScheme())
{
    initialiseColours();
}

CustomLookAndFeel::CustomLookAndFeel (ColourScheme scheme)  : currentColourScheme (scheme)
{
    initialiseColours();
}

CustomLookAndFeel::~CustomLookAndFeel()  {}


void CustomLookAndFeel::drawLabel (Graphics& g, Label& label)
{
   
    g.fillAll (label.findColour (Label::backgroundColourId));
    
    if (! label.isBeingEdited())
    {
        const float alpha = label.isEnabled() ? 1.0f : 0.5f;
        const Font font (getLabelFont (label));
        
        g.setColour (label.findColour (Label::textColourId).withMultipliedAlpha (alpha));
        g.setFont (font);
        
        Rectangle<int> textArea (label.getBorderSize().subtractedFrom (label.getLocalBounds()));
        
        g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                          jmax (1, (int) (textArea.getHeight() / font.getHeight())),
                          label.getMinimumHorizontalScale());
        
        g.setColour (label.findColour (Label::outlineColourId).withMultipliedAlpha (alpha));
    }
    else if (label.isEnabled())
    {
        g.setColour (label.findColour (Label::outlineColourId));
    }
    
    g.drawRoundedRectangle(label.getLocalBounds().toFloat(), 10.0f, 4.0f);
    
}

//copied verbatim from Juce Modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V4
//so that my custom look and feel class can implement LAFV4 colourschemes
void CustomLookAndFeel::initialiseColours()
{
    const uint32 transparent = 0x00000000;
    
    const uint32 coloursToUse[] =
    {
        TextButton::buttonColourId,                 currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        TextButton::buttonOnColourId,               currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        TextButton::textColourOnId,                 currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedText).getARGB(),
        TextButton::textColourOffId,                currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        
        ToggleButton::textColourId,                 currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        ToggleButton::tickColourId,                 currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        ToggleButton::tickDisabledColourId,         currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).withAlpha (0.5f).getARGB(),
        
        TextEditor::backgroundColourId,             currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        TextEditor::textColourId,                   currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        TextEditor::highlightColourId,              currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        TextEditor::highlightedTextColourId,        currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedText).getARGB(),
        TextEditor::outlineColourId,                currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        TextEditor::focusedOutlineColourId,         currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        TextEditor::shadowColourId,                 transparent,
        
        CaretComponent::caretColourId,              currentColourScheme.getUIColour (ColourScheme::UIColour::defaultFill).getARGB(),
        
        Label::backgroundColourId,                  transparent,
        Label::textColourId,                        currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        Label::outlineColourId,                     transparent,
        Label::textWhenEditingColourId,             currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        
        ScrollBar::backgroundColourId,              transparent,
        ScrollBar::thumbColourId,                   currentColourScheme.getUIColour (ColourScheme::UIColour::defaultFill).getARGB(),
        ScrollBar::trackColourId,                   transparent,
        
        TreeView::linesColourId,                    transparent,
        TreeView::backgroundColourId,               transparent,
        TreeView::dragAndDropIndicatorColourId,     currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        TreeView::selectedItemBackgroundColourId,   transparent,
        TreeView::oddItemsColourId,                 transparent,
        TreeView::evenItemsColourId,                transparent,
        
        PopupMenu::backgroundColourId,              currentColourScheme.getUIColour (ColourScheme::UIColour::menuBackground).getARGB(),
        PopupMenu::textColourId,                    currentColourScheme.getUIColour (ColourScheme::UIColour::menuText).getARGB(),
        PopupMenu::headerTextColourId,              currentColourScheme.getUIColour (ColourScheme::UIColour::menuText).getARGB(),
        PopupMenu::highlightedTextColourId,         currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedText).getARGB(),
        PopupMenu::highlightedBackgroundColourId,   currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        
        ComboBox::buttonColourId,                   currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        ComboBox::outlineColourId,                  currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        ComboBox::textColourId,                     currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        ComboBox::backgroundColourId,               currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        ComboBox::arrowColourId,                    currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        
        PropertyComponent::backgroundColourId,      currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        PropertyComponent::labelTextColourId,       currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        
        TextPropertyComponent::backgroundColourId,  currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        TextPropertyComponent::textColourId,        currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        TextPropertyComponent::outlineColourId,     currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        
        BooleanPropertyComponent::backgroundColourId, currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        BooleanPropertyComponent::outlineColourId,    currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        
        ListBox::backgroundColourId,                currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        ListBox::outlineColourId,                   currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        ListBox::textColourId,                      currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        
        Slider::backgroundColourId,                 currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        Slider::thumbColourId,                      currentColourScheme.getUIColour (ColourScheme::UIColour::defaultFill).getARGB(),
        Slider::trackColourId,                      currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        Slider::rotarySliderFillColourId,           currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        Slider::rotarySliderOutlineColourId,        currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        Slider::textBoxTextColourId,                currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        Slider::textBoxBackgroundColourId,          currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).withAlpha (0.0f).getARGB(),
        Slider::textBoxHighlightColourId,           currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        Slider::textBoxOutlineColourId,             currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        
        ResizableWindow::backgroundColourId,        currentColourScheme.getUIColour (ColourScheme::UIColour::windowBackground).getARGB(),
        
        DocumentWindow::textColourId,               currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        
        AlertWindow::backgroundColourId,            currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        AlertWindow::textColourId,                  currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        AlertWindow::outlineColourId,               currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        
        ProgressBar::backgroundColourId,            currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        ProgressBar::foregroundColourId,            currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        
        TooltipWindow::backgroundColourId,          currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        TooltipWindow::textColourId,                currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedText).getARGB(),
        TooltipWindow::outlineColourId,             transparent,
        
        TabbedComponent::backgroundColourId,        transparent,
        TabbedComponent::outlineColourId,           currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        TabbedButtonBar::tabOutlineColourId,        currentColourScheme.getUIColour (ColourScheme::UIColour::outline).withAlpha (0.5f).getARGB(),
        TabbedButtonBar::frontOutlineColourId,      currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        
        Toolbar::backgroundColourId,                currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).withAlpha (0.4f).getARGB(),
        Toolbar::separatorColourId,                 currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        Toolbar::buttonMouseOverBackgroundColourId, currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).contrasting (0.2f).getARGB(),
        Toolbar::buttonMouseDownBackgroundColourId, currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).contrasting (0.5f).getARGB(),
        Toolbar::labelTextColourId,                 currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        Toolbar::editingModeOutlineColourId,        currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        
        DrawableButton::textColourId,               currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        DrawableButton::textColourOnId,             currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedText).getARGB(),
        DrawableButton::backgroundColourId,         transparent,
        DrawableButton::backgroundOnColourId,       currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        
        HyperlinkButton::textColourId,              currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).interpolatedWith (Colours::blue, 0.4f).getARGB(),
        
        GroupComponent::outlineColourId,            currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        GroupComponent::textColourId,               currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        
        BubbleComponent::backgroundColourId,        currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        BubbleComponent::outlineColourId,           currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        
        DirectoryContentsDisplayComponent::highlightColourId,   currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        DirectoryContentsDisplayComponent::textColourId,        currentColourScheme.getUIColour (ColourScheme::UIColour::menuText).getARGB(),
        
        0x1000440, /*LassoComponent::lassoFillColourId*/        currentColourScheme.getUIColour (ColourScheme::UIColour::defaultFill).getARGB(),
        0x1000441, /*LassoComponent::lassoOutlineColourId*/     currentColourScheme.getUIColour (ColourScheme::UIColour::outline).getARGB(),
        
        0x1005000, /*MidiKeyboardComponent::whiteNoteColourId*/               0xffffffff,
        0x1005001, /*MidiKeyboardComponent::blackNoteColourId*/               0xff000000,
        0x1005002, /*MidiKeyboardComponent::keySeparatorLineColourId*/        0x66000000,
        0x1005003, /*MidiKeyboardComponent::mouseOverKeyOverlayColourId*/     0x80ffff00,
        0x1005004, /*MidiKeyboardComponent::keyDownOverlayColourId*/          0xffb6b600,
        0x1005005, /*MidiKeyboardComponent::textLabelColourId*/               0xff000000,
        0x1005006, /*MidiKeyboardComponent::upDownButtonBackgroundColourId*/  0xffd3d3d3,
        0x1005007, /*MidiKeyboardComponent::upDownButtonArrowColourId*/       0xff000000,
        0x1005008, /*MidiKeyboardComponent::shadowColourId*/                  0x4c000000,
        
        0x1004500, /*CodeEditorComponent::backgroundColourId*/                currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        0x1004502, /*CodeEditorComponent::highlightColourId*/                 currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).getARGB(),
        0x1004503, /*CodeEditorComponent::defaultTextColourId*/               currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        0x1004504, /*CodeEditorComponent::lineNumberBackgroundId*/            currentColourScheme.getUIColour (ColourScheme::UIColour::highlightedFill).withAlpha (0.5f).getARGB(),
        0x1004505, /*CodeEditorComponent::lineNumberTextId*/                  currentColourScheme.getUIColour (ColourScheme::UIColour::defaultFill).getARGB(),
        
        0x1007000, /*ColourSelector::backgroundColourId*/                     currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        0x1007001, /*ColourSelector::labelTextColourId*/                      currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        
        0x100ad00, /*KeyMappingEditorComponent::backgroundColourId*/          currentColourScheme.getUIColour (ColourScheme::UIColour::widgetBackground).getARGB(),
        0x100ad01, /*KeyMappingEditorComponent::textColourId*/                currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
        
        FileSearchPathListComponent::backgroundColourId,        currentColourScheme.getUIColour (ColourScheme::UIColour::menuBackground).getARGB(),
        
        FileChooserDialogBox::titleTextColourId,                currentColourScheme.getUIColour (ColourScheme::UIColour::defaultText).getARGB(),
    };
    
    for (int i = 0; i < numElementsInArray (coloursToUse); i += 2)
        setColour ((int) coloursToUse [i], Colour ((uint32) coloursToUse [i + 1]));
}
