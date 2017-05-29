/*
  ==============================================================================

    EditTournamentWindow.cpp
    Created: 29 May 2017 3:47:46pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "MainAppWindow.h"
#include "EditTournamentWindow.h"

//==============================================================================
EditTournamentWindow::EditTournamentWindow()
{
    save.setButtonText("Save");
    save.addListener(this);
    addAndMakeVisible(save);
    
    cancel.setButtonText("Cancel");
    cancel.addListener(this);
    addAndMakeVisible(cancel);
    
    consolationBracket.setButtonText("Consolation Bracket?");
    consolationBracket.addListener(this);
    addAndMakeVisible(consolationBracket);
    
    browse.setButtonText("...");
    browse.addListener(this);
    addAndMakeVisible(browse);
    
    tournName.setTextToShowWhenEmpty("Enter Tournament Name", Colours::black.withAlpha(0.5f));
    tournament.setText("Tournament Name", dontSendNotification);
    tournament.attachToComponent(&tournName, true);
    
    addAndMakeVisible(tournName);
    addAndMakeVisible(tournament);
    
    location.setTextToShowWhenEmpty("Enter a Location", Colours::black.withAlpha(0.5f));
    loc.setText("Location", dontSendNotification);
    loc.attachToComponent(&location, true);
    
    addAndMakeVisible(location);
    addAndMakeVisible(loc);
    
    logoImage.setTextToShowWhenEmpty("path to logo image", Colours::black.withAlpha(0.5f));
    logo.setText("Logo Image File\n(transparent bg, .svg preferred)", dontSendNotification);
    logo.attachToComponent(&logoImage, true);
    
    addAndMakeVisible(logoImage);
    addAndMakeVisible(logo);
    
    
    
    TextEditor editRounds;
    
    Label rounds;

}

EditTournamentWindow::~EditTournamentWindow()
{
    save.removeListener(this);
    cancel.removeListener(this);
    consolationBracket.removeListener(this);
    browse.removeListener(this);
}

void EditTournamentWindow::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background


}

void EditTournamentWindow::buttonClicked (Button* button)
{
    
}

void EditTournamentWindow::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
