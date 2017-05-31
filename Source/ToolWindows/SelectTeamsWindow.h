/*
  ==============================================================================

    SelectTeamsWindow.h
    Created: 31 May 2017 1:28:55pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once
#include "../Settings/OSDependencyThings.h"

//==============================================================================
class SelectableTeam
{
public:
    SelectableTeam( String name )
    {
        selected.setToggleState(false, dontSendNotification);
        teamName = name;
    }
    
    ToggleButton selected;
    String teamName;
    
};

//==============================================================================

class SelectTeamsWindow    : public Component,
                                public TableListBoxModel,
                                public Button::Listener
{
public:
    SelectTeamsWindow();
    SelectTeamsWindow( String teamsPipeDelineated );
    ~SelectTeamsWindow();
    
    // This is overloaded from TableListBoxModel, and must return the total number of rows in our table
    int getNumRows() override;
    
    // This is overloaded from TableListBoxModel, and should fill in the background of the whole row
    void paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
    
    
    // This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
    // components.
    void paintCell (Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool /*rowIsSelected*/) override;
    
    
    // This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
    // to change the sort order.
    void sortOrderChanged (int newSortColumnId, bool isForwards) override;
    
    
    // This is overloaded from TableListBoxModel, and must update any custom components that we're using
    Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                        Component* existingComponentToUpdate) override;
    
    
    // This is overloaded from TableListBoxModel, and should choose the best width for the specified
    // column.
    int getColumnAutoSizeWidth (int columnId) override;
    
    
    // A couple of quick methods to set and get cell values when the user changes them
    bool getToggled (const int rowNumber) const;
    
    
    void setToggled (const int rowNumber, const bool shouldBeOn);
    
    //==============================================================================
    void resized() override;
    void buttonClicked (Button* button) override;
    
    
private:
    TableListBox table;     // the table component itself
    Font font;
    
    OwnedArray<SelectableTeam> selectTeams;
    
    TextButton selectAll;
    TextButton selectNone;
    
    TextButton saveTournament;
    TextButton cancel;
    
    TextButton editSelectedTeam;
    TextButton addTeam;
    
    Label tournamentHeader;
    
    int numRows;            // The number of rows of data we've got
    
    String getAttributeNameForColumnId (const int columnId) const;
    
    void initBasics();
    
    //==============================================================================
    // This is a custom component containing a combo box, which we're going to put inside
    // our table's "rating" column.
    class ToggleBoxCustomComponent    : public Component,
                                        private Button::Listener
    {
    public:
        ToggleBoxCustomComponent (SelectTeamsWindow& td) : owner (td)
        {
            addAndMakeVisible(button);
            
            // when the combo is changed, we'll get a callback.
            button.addListener (this);
            button.setWantsKeyboardFocus (false);
        }
        
        void resized() override
        {
            button.setBoundsInset (BorderSize<int> (2));
        }
        
        // Our demo code will call this when we may need to update our contents
        void setRowAndColumn (int newRow, int newColumn)
        {
            row = newRow;
            columnId = newColumn;
            button.setToggleState(owner.getToggled(row), dontSendNotification);
        }
        
        
        void buttonClicked (Button*) override
        {
            owner.setToggled(row, button.getToggleState());
        }
        
        
    private:
        SelectTeamsWindow& owner;
        ToggleButton button;
        int row, columnId;
    };
    
    //==============================================================================
    // A comparator used to sort our data when the user clicks a column header
    class TeamListSorter
    {
    public:
        TeamListSorter (const String& attributeToSortBy, bool forwards)
        : attributeToSort (attributeToSortBy),
        direction (forwards ? 1 : -1)
        {
        }
        
        int compareElements (SelectableTeam* first, SelectableTeam* second) const
        {
            int result = 0;
            
            if (attributeToSort == "Select")
            {
                if ( first->selected.getToggleState() )
                {
                    if ( ! second->selected.getToggleState() )
                        result = -1; //first is selected, second is not
                }
                else
                {
                    if ( second->selected.getToggleState() )
                    {
                        result = 1; //second is selected, first is not
                    }
                }//all other cases, they are equal and result can remain 0
            }
            else if (attributeToSort == "Team Name")
            {
                result = first->teamName.compare(second->teamName);
            }
            
            return direction * result;
        }
        
    private:
        String attributeToSort;
        int direction;
    };
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SelectTeamsWindow)
};
