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
CustomLookAndFeel::CustomLookAndFeel()
{
    setupColours();
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

void CustomLookAndFeel::setupColours()
{
    auto& colourScheme = getCurrentColourScheme();
    
    if (colourScheme == getDarkColourScheme())
    {
        setColour (backgroundColourId,                   Colour (0xff323e44));
        setColour (secondaryBackgroundColourId,          Colour (0xff263238));
        setColour (defaultTextColourId,                  Colours::white);
        setColour (widgetTextColourId,                   Colours::white);
        setColour (defaultButtonBackgroundColourId,      Colour (0xffa45c94));
        setColour (secondaryButtonBackgroundColourId,    Colours::black);
        setColour (userButtonBackgroundColourId,         Colour (0xffa45c94));
        setColour (defaultIconColourId,                  Colours::white);
        setColour (treeIconColourId,                     Colour (0xffa9a9a9));
        setColour (defaultHighlightColourId,             Colour (0xffe0ec65));
        setColour (defaultHighlightedTextColourId,       Colours::black);
        setColour (codeEditorLineNumberColourId,         Colour (0xffaaaaaa));
        setColour (activeTabIconColourId,                Colours::white);
        setColour (inactiveTabBackgroundColourId,        Colour (0xff181f22));
        setColour (inactiveTabIconColourId,              Colour (0xffa9a9a9));
        setColour (contentHeaderBackgroundColourId,      Colours::black);
        setColour (widgetBackgroundColourId,             Colour (0xff495358));
        setColour (secondaryWidgetBackgroundColourId,    Colour (0xff303b41));
        
        colourScheme.setUIColour (LookAndFeel_V4::ColourScheme::UIColour::defaultFill, Colour (0xffa45c94));
    }
    else if (colourScheme == getGreyColourScheme())
    {
        setColour (backgroundColourId,                   Colour (0xff505050));
        setColour (secondaryBackgroundColourId,          Colour (0xff424241));
        setColour (defaultTextColourId,                  Colours::white);
        setColour (widgetTextColourId,                   Colours::black);
        setColour (defaultButtonBackgroundColourId,      Colour (0xff26ba90));
        setColour (secondaryButtonBackgroundColourId,    Colours::black);
        setColour (userButtonBackgroundColourId,         Colour (0xff26ba90));
        setColour (defaultIconColourId,                  Colours::white);
        setColour (treeIconColourId,                     Colour (0xffa9a9a9));
        setColour (defaultHighlightColourId,             Colour (0xffe0ec65));
        setColour (defaultHighlightedTextColourId,       Colours::black);
        setColour (codeEditorLineNumberColourId,         Colour (0xffaaaaaa));
        setColour (activeTabIconColourId,                Colours::white);
        setColour (inactiveTabBackgroundColourId,        Colour (0xff373737));
        setColour (inactiveTabIconColourId,              Colour (0xffa9a9a9));
        setColour (contentHeaderBackgroundColourId,      Colours::black);
        setColour (widgetBackgroundColourId,             Colours::white);
        setColour (secondaryWidgetBackgroundColourId,    Colour (0xffdddddd));
    }
    else if (colourScheme == getLightColourScheme())
    {
        setColour (backgroundColourId,                   Colour (0xffefefef));
        setColour (secondaryBackgroundColourId,          Colour (0xfff9f9f9));
        setColour (defaultTextColourId,                  Colours::black);
        setColour (widgetTextColourId,                   Colours::black);
        setColour (defaultButtonBackgroundColourId,      Colour (0xff42a2c8));
        setColour (secondaryButtonBackgroundColourId,    Colour (0xffa1c677));
        setColour (userButtonBackgroundColourId,         Colour (0xffff5b5b));
        setColour (defaultIconColourId,                  Colours::white);
        setColour (treeIconColourId,                     Colour (0xffa9a9a9));
        setColour (defaultHighlightColourId,             Colour (0xffffd05b));
        setColour (defaultHighlightedTextColourId,       Colour (0xff585656));
        setColour (codeEditorLineNumberColourId,         Colour (0xff888888));
        setColour (activeTabIconColourId,                Colour (0xff42a2c8));
        setColour (inactiveTabBackgroundColourId,        Colour (0xffd5d5d5));
        setColour (inactiveTabIconColourId,              Colour (0xffa9a9a9));
        setColour (contentHeaderBackgroundColourId,      Colour (0xff42a2c8));
        setColour (widgetBackgroundColourId,             Colours::white);
        setColour (secondaryWidgetBackgroundColourId,    Colour (0xfff4f4f4));
    }
    
    setColour (Label::textColourId,                             findColour (defaultTextColourId));
    setColour (Label::textColourId,                             findColour (defaultTextColourId));
    setColour (TextEditor::highlightColourId,                   findColour (defaultHighlightColourId));
    setColour (TextEditor::highlightedTextColourId,             findColour (defaultHighlightedTextColourId));
    setColour (TextEditor::outlineColourId,                     Colours::transparentBlack);
    setColour (TextEditor::focusedOutlineColourId,              Colours::transparentBlack);
    setColour (TextEditor::backgroundColourId,                  findColour (widgetBackgroundColourId));
    setColour (TextEditor::textColourId,                        findColour (widgetTextColourId));
    setColour (TextButton::buttonColourId,                      findColour (defaultButtonBackgroundColourId));
    setColour (ScrollBar::ColourIds::thumbColourId,             Colour (0xffd0d8e0));
    setColour (TextPropertyComponent::outlineColourId,          Colours::transparentBlack);
    setColour (TextPropertyComponent::backgroundColourId,       findColour (widgetBackgroundColourId));
    setColour (TextPropertyComponent::textColourId,             findColour (widgetTextColourId));
    setColour (BooleanPropertyComponent::outlineColourId,       Colours::transparentBlack);
    setColour (BooleanPropertyComponent::backgroundColourId,    findColour (widgetBackgroundColourId));
    setColour (ToggleButton::tickDisabledColourId,              Colour (0xffa9a9a9));
    setColour (ToggleButton::tickColourId,                      findColour (defaultButtonBackgroundColourId).withMultipliedBrightness(1.3f));
    setColour (CodeEditorComponent::backgroundColourId,         findColour (secondaryBackgroundColourId));
    setColour (CodeEditorComponent::lineNumberTextId,           findColour (codeEditorLineNumberColourId));
    setColour (CodeEditorComponent::lineNumberBackgroundId,     findColour (backgroundColourId));
    setColour (CaretComponent::caretColourId,                   findColour (defaultButtonBackgroundColourId));
    setColour (TreeView::selectedItemBackgroundColourId,        findColour (defaultHighlightColourId));
}
