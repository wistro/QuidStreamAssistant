/*
==============================================================================

    SelectTeamsWindow.cpp
    Created: 31 May 2017 1:28:55pm
    Author:  Willow Rosenberg

==============================================================================
*/

#include "SelectTeamsWindow.h"
#include "../Settings/Team.h"
#include "../TopLevel/Application.h"


SelectTeamsWindow::SelectTeamsWindow()   : font (14.0f)
{
    Team::refreshTeamList();
    
    selectTeams.clear();
    
    numRows = Team::teamList.size();
    
    for ( int i = 0; i < numRows; i++ )
    {
        selectTeams.add(new SelectableTeam(Team::teamList[i]));
    }
    
    initBasics();
}

SelectTeamsWindow::SelectTeamsWindow( String teamsPipeDelineated ) : font(14.0f)
{
    Team::refreshTeamList();
    
    selectTeams.clear();

    StringArray prevSelectedTeams;
    prevSelectedTeams.addTokens(teamsPipeDelineated, "|");
    
    numRows = Team::teamList.size();
    
    for ( int i = 0; i < numRows; i++ )
    {
        selectTeams.add(new SelectableTeam(Team::teamList[i]));
        
        if ( prevSelectedTeams.contains(Team::teamList[i]) )
        {
            selectTeams[i]->selected.setToggleState(true, dontSendNotification);
        }
    }
    
    initBasics();
}

SelectTeamsWindow::~SelectTeamsWindow()
{
    selectAll.removeListener(this);
    selectNone.removeListener(this);
    saveTournament.removeListener(this);
    cancel.removeListener(this);
    editSelectedTeam.removeListener(this);
    addTeam.removeListener(this);
}

void SelectTeamsWindow::initBasics()
{
    tournamentHeader.setText(QuidStreamAssistantApplication::getApp().thisTournament->getTournamentName(), dontSendNotification);
    tournamentHeader.setFont(20.0f);
    addAndMakeVisible(tournamentHeader);
    
    selectAll.setButtonText("Select All");
    selectAll.addListener(this);
    addAndMakeVisible(selectAll);
    
    selectNone.setButtonText("Select None");
    selectNone.addListener(this);
    addAndMakeVisible(selectNone);
    
    saveTournament.setButtonText("Save");
    saveTournament.setTooltip("Selected teams will be available to show when creating game overlays");
    saveTournament.addListener(this);
    addAndMakeVisible(saveTournament);
    
    cancel.setButtonText("Cancel");
    cancel.setTooltip("Return to Tournament Edit Screen. Does not save changes.");
    cancel.addListener(this);
    addAndMakeVisible(cancel);
    
    editSelectedTeam.setButtonText("Edit Selected Team");
    editSelectedTeam.setTooltip("Opens a new window to change the details of the selected team.");
    editSelectedTeam.addListener(this);
    addAndMakeVisible(editSelectedTeam);
    
    addTeam.setButtonText("Add New Team");
    addTeam.setTooltip("Opens a new window to create a new team from defaults");
    addTeam.addListener(this);
    addAndMakeVisible(addTeam);
    
    // Create our table component and add it to this component..
    addAndMakeVisible (table);
    table.setModel (this);
    
    // give it a border
    table.setColour (ListBox::outlineColourId, getLookAndFeel().findColour(Slider::textBoxOutlineColourId));
    table.setOutlineThickness (1);
    
    table.getHeader().addColumn ("Select", 1, 30, 30, -1,
                                 TableHeaderComponent::visible | TableHeaderComponent::sortable, 0);
    table.getHeader().addColumn("Team Name", 2, 200, 100, -1,
                                TableHeaderComponent::visible | TableHeaderComponent::sortable | TableHeaderComponent::resizable
                                | TableHeaderComponent::sortedForwards, -1);
    
    table.getHeader().setStretchToFitActive (true);
}

//==============================================================================

// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
int SelectTeamsWindow::getNumRows()
{
    return numRows;
}

// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
void SelectTeamsWindow::paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    const Colour alternateColour (getLookAndFeel().findColour (ListBox::backgroundColourId)
                                  .interpolatedWith (getLookAndFeel().findColour (ListBox::textColourId), 0.03f));
    if (rowIsSelected)
        g.fillAll (Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll (alternateColour);
}

// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
// components.
void SelectTeamsWindow::paintCell (Graphics& g, int rowNumber, int columnId,
                int width, int height, bool /*rowIsSelected*/)
{
    g.setColour (getLookAndFeel().findColour (ListBox::textColourId));
    g.setFont (font);
    
    if ( rowNumber < numRows )
    {
        g.drawText (selectTeams[rowNumber]->teamName, 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    
    g.setColour (getLookAndFeel().findColour (ListBox::backgroundColourId));
    g.fillRect (width - 1, 0, 1, height);
}

//==============================================================================

// This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
// to change the sort order.
void SelectTeamsWindow::sortOrderChanged (int newSortColumnId, bool isForwards)
{
    if (newSortColumnId != 0)
    {
        TeamListSorter sorter (getAttributeNameForColumnId (newSortColumnId), isForwards);
        selectTeams.sort(sorter);
        
        table.updateContent();
    }
}

// This is overloaded from TableListBoxModel, and must update any custom components that we're using
Component* SelectTeamsWindow::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                    Component* existingComponentToUpdate)
{
    if (columnId != 1) // The Team Name column has no custom component (there are only two IDs possible, 1 = Button; 2 = TN
    {
        jassert (existingComponentToUpdate == nullptr);
        return nullptr;
    }
    else  // For the button/select column, we return the custom togglebutton component
    {
        ToggleBoxCustomComponent* selectBox = static_cast<ToggleBoxCustomComponent*> (existingComponentToUpdate);
        
        // If an existing component is being passed-in for updating, we'll re-use it, but
        // if not, we'll have to create one.
        if (selectBox == nullptr)
            selectBox = new ToggleBoxCustomComponent (*this);
        
        selectBox->setRowAndColumn (rowNumber, columnId);
        return selectBox;
    }
    
}

// This is overloaded from TableListBoxModel, and should choose the best width for the specified
// column.
int SelectTeamsWindow::getColumnAutoSizeWidth (int columnId)
{
    if (columnId == 1)
        return 30; // (this is the select button column, containing a custom togglebutton component)
    
    int widest = 32;
    
    // find the widest bit of text in this column..
    for (int i = 0; i < numRows; i++)
    {
        widest = jmax (widest, font.getStringWidth (selectTeams[i]->teamName));
    }
    
    return widest + 8;
}

// A couple of quick methods to set and get cell values when the user changes them
bool SelectTeamsWindow::getToggled (const int rowNumber) const
{
    return selectTeams[rowNumber]->selected.getToggleState();
}

void SelectTeamsWindow::setToggled (const int rowNumber, const bool shouldBeOn)
{
    selectTeams[rowNumber]->selected.setToggleState(shouldBeOn, dontSendNotification);
}



//==============================================================================
void SelectTeamsWindow::resized()
{
    // position our table with a gap around its edge
    table.setBoundsInset (BorderSize<int> (8));
}

// (a utility method to search our XML for the attribute that matches a column ID)
String SelectTeamsWindow::getAttributeNameForColumnId (const int columnId) const
{
    if ( columnId == 0 )
        return "Select";
    else if ( columnId == 1 )
        return "Team Name";
    else
        return String();
}

void SelectTeamsWindow::buttonClicked(Button* button)
{
    if ( button == &selectAll )
    {
        for ( int i = 0; i < numRows; i++ )
        {
            setToggled(i, true);
        }
    }
    else if (  button == &selectNone )
    {
        for ( int i = 0; i < numRows; i++ )
        {
            setToggled(i, false);
        }
    }
    else if ( button == &saveTournament )
    {
        //add teams to tournament
    }
    else if ( button == &cancel )
    {
        //go back to previous window
    }
    else if ( button == &editSelectedTeam )
    {
        //open edit team window with that team's data loaded
        //haven't written this window yet.
        //So it goes.
    }
    else if ( button == &addTeam)
    {
        //open edit team window with default data
    }
}
