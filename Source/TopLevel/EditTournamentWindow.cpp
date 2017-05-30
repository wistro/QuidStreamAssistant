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
#include "Application.h"

//==============================================================================
EditTournamentWindow::EditTournamentWindow()
{
    save.setButtonText("Save");
    save.addListener(this);
    addAndMakeVisible(save);
    
    cancel.setButtonText("Cancel");
    cancel.addListener(this);
    addAndMakeVisible(cancel);
    
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
    
    logoImage.setTextToShowWhenEmpty("path to logo image (transparent bg, .svg preferred)", Colours::black.withAlpha(0.5f));
    logo.setText("Logo Image File", dontSendNotification);
    logo.attachToComponent(&logoImage, true);
    
    addAndMakeVisible(logoImage);
    addAndMakeVisible(logo);
    
    editRounds.setMultiLine(true);
    editRounds.setReturnKeyStartsNewLine(true);
    editRounds.setText(QuidStreamAssistantApplication::getApp().thisTournament->getRoundsList().joinIntoString("\n"));
    rounds.setText("Rounds (one per line)", dontSendNotification);
    rounds.attachToComponent(&editRounds, false);
    
    addAndMakeVisible(editRounds);
    addAndMakeVisible(rounds);
    
    consolationBracket.setButtonText("Consolation Bracket?");
    consolationBracket.setToggleState(true, dontSendNotification);
    consolationBracket.addListener(this);
    addAndMakeVisible(consolationBracket);

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
}

void EditTournamentWindow::buttonClicked (Button* button)
{
    //cancel and go back to tournament select screen
    if ( button == &cancel )
    {
        QuidStreamAssistantApplication::getApp().mainWindow->showIntro();
        QuidStreamAssistantApplication::getApp().editTournament = nullptr;
    }
    
    //save entered tournament settings as a .tourn setting file
    //next step, this has to close this window and open the AddTeamsWindow
    else if ( button == &save )
    {
        if ( tournName.isEmpty() )
        {
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Please Enter a Tournament Name",
                                              "", "OK");
        }
        else
        {
            Tournament* currentTournament = QuidStreamAssistantApplication::getApp().thisTournament;
            
            //some image input file was given, hopefully it was a real one
            if ( ! logoImage.isEmpty() )
            {
                File image ( logoImage.getText() );
                
                //file path given exists and is an image
                if ( image.existsAsFile() && image.hasFileExtension("jpeg;jpg;png;gif;svg"))
                {
                    currentTournament->fillThisSucker(tournName.getText(), location.getText(), editRounds.getText(), image);
                }
                else
                {
                    AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "The Logo File Path Is Not Valid",
                                                      "Please enter a valid file path or leave blank for no logo.", "OK");
                }
            }
            else
            {
                currentTournament->fillThisSucker(tournName.getText(), location.getText(), editRounds.getText());
            }
        }
    }
    
    //open a file browser to get an image
    else if ( button == &browse )
    {
        ImagePreviewComponent imagePreview;
        imagePreview.setSize (200, 200);
        
        FileChooser fc ("Choose the Tournament's Logo Image",
                        File::getSpecialLocation (File::userPicturesDirectory),
                        "*.jpg;*.jpeg;*.png;*.gif;*.svg",
                        true);
        
        if ( fc.browseForFileToOpen() )
        {
            logoImage.setText(fc.getResult().getFullPathName());
        }
    }
    
    //(when checking the box) adds the default list of Consolation rounds
    //(when unchecking the box) removes all rounds that begin with the word "Consolation" (ignorescase)
    else if ( button == &consolationBracket )
    {
        if ( consolationBracket.getToggleState() )
        {
            StringArray temp;
            temp.addLines(editRounds.getText());
            for ( int i = 0; i < Tournament::consolationBracket.size(); i++ )
            {
                temp.addIfNotAlreadyThere(Tournament::consolationBracket.operator[](i), true);
            }
            editRounds.setText(temp.joinIntoString("\n"));
        }
        else
        {
            StringArray temp;
            temp.addLines(editRounds.getText());
            temp.trim();
            for ( int i = 0; i < temp.size(); i++ )
            {
                if ( temp[i].startsWithIgnoreCase(Tournament::consolation) )
                {
                    temp.remove(i);
                    i--; //by removing an entry, we need to reiterate on the last index because it's value has changed
                }
            }
            editRounds.setText(temp.joinIntoString("\n"));
        }
    }
}

void EditTournamentWindow::resized()
{
    Rectangle<int> area(getLocalBounds());
    const int margin = 2;
    const int textBoxHeight = 30;
    const int buttonHeight = 40;
    const int buttonWidth = 60;
    
    Rectangle<int> nameInput (area.removeFromTop(textBoxHeight).reduced(margin));
    tournament.setBounds(nameInput.removeFromLeft(proportionOfWidth(0.2f)));
    tournName.setBounds(nameInput);
    
    Rectangle<int> locInput (area.removeFromTop(textBoxHeight).reduced(margin));
    loc.setBounds(locInput.removeFromLeft(proportionOfWidth(0.2f)));
    location.setBounds(locInput);
    
    Rectangle<int> logoInput (area.removeFromTop(textBoxHeight).reduced(margin));
    logo.setBounds(logoInput.removeFromLeft(proportionOfWidth(0.2f)));
    browse.setBounds(logoInput.removeFromRight(buttonWidth).reduced(margin));
    logoImage.setBounds(logoInput);
    
//    rounds.setBounds(area.removeFromTop(textBoxHeight).reduced(margin));
    consolationBracket.setBounds(area.removeFromTop(textBoxHeight).reduced(margin).removeFromRight(proportionOfWidth(0.76f)).reduced(margin * 2));
    
    Rectangle<int> saveCancel (area.removeFromBottom(buttonHeight).reduced(margin));
    saveCancel.removeFromLeft(saveCancel.getWidth() / 2);
    save.setBounds(saveCancel.removeFromRight(saveCancel.getWidth() / 2).reduced(margin));
    cancel.setBounds(saveCancel.reduced(margin));
    
    editRounds.setBounds(area.reduced(margin));

}
