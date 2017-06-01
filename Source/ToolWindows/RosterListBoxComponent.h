/*
  ==============================================================================

    RosterListBoxComponent.h
    Created: 1 Jun 2017 10:28:16am
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "../Settings/OSDependencyThings.h"
#include "../Settings/Player.h"

class RosterListBoxComponent    : public Component,
                                public TableListBoxModel
{
public:
    RosterListBoxComponent();
    RosterListBoxComponent( String teamName );
    ~RosterListBoxComponent();
    
    static const StringArray columnIds;
    
    
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
    Player* getPlayer (const int rowNumber);
    void addBlankPlayer();
    void removeSelected();
    
    void givePi();
    void infiniteLove();

    bool getToggled (const int columnNumber, const int rowNumber) const;
    void setToggled (const int columnNumber, const int rowNumber, const bool shouldBeOn);
    
    String getText (const int columnNumber, const int rowNumber) const;
    void setText (const int columnNumber, const int rowNumber, const String& newText);
    
    
    //==============================================================================
    void resized() override;    
    
private:
    TableListBox table;
    Font font;
    
    OwnedArray<Player> roster;
    
    int numRows;            // The number of rows of data we've got
    
    String getAttributeNameForColumnId (const int columnId) const;
    
    void initBasics();
    void readFromXML (const XmlElement& xml);
    void readFromFile (const File& file);
    
    //==============================================================================
    // This is a custom Label component, which we use for the table's editable text columns.
    class EditableTextCustomComponent  : public Label
    {
    public:
        EditableTextCustomComponent (RosterListBoxComponent& td)  : owner (td)
        {
            // double click to edit the label text; single click handled below
            setEditable (false, true, false);
        }
        
        void mouseDown (const MouseEvent& event) override
        {
            // single click on the label should simply select the row
            owner.table.selectRowsBasedOnModifierKeys (row, event.mods, false);
            
            Label::mouseDown (event);
        }
        
        void textWasEdited() override
        {
            owner.setText (columnId, row, getText());
        }
        
        // Our demo code will call this when we may need to update our contents
        void setRowAndColumn (const int newRow, const int newColumn)
        {
            row = newRow;
            columnId = newColumn;
            setText (owner.getText(columnId, row), dontSendNotification);
        }
        
        void paint (Graphics& g) override
        {
            auto& lf = getLookAndFeel();
            if (! dynamic_cast<LookAndFeel_V4*> (&lf))
                lf.setColour (textColourId, Colours::black);
                
                Label::paint (g);
                }
        
    private:
        RosterListBoxComponent& owner;
        int row, columnId;
        Colour textColour;
    };
    
    //==============================================================================
    // This is a custom component containing a combo box, which we're going to put inside
    // our table's "rating" column.
    class ToggleBoxCustomComponent    : public Component,
                                        private Button::Listener
    {
    public:
        ToggleBoxCustomComponent (RosterListBoxComponent& td) : owner (td)
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
            button.setToggleState(owner.getToggled(columnId, row), dontSendNotification);
        }
        
        
        void buttonClicked (Button*) override
        {
            owner.setToggled(columnId, row, button.getToggleState());
        }
        
        
    private:
        RosterListBoxComponent& owner;
        ToggleButton button;
        int row, columnId;
    };
    
    //==============================================================================
    // A comparator used to sort our data when the user clicks a column header
    class RosterListSorter
    {
    public:
        RosterListSorter (const String& attributeToSortBy, bool forwards)
                : attributeToSort (attributeToSortBy),
        direction (forwards ? 1 : -1)
        {
        }
        
        int compareElements (Player* first, Player* second) const
        {
            int result = 0;
            
            if (attributeToSort == "Number")
            {
                result = PlayerComparator::compareElements(first, second);
            }
            else if (attributeToSort == "First Name")
            {
                result = first->getAttribute(attributeToSort).compare(second->getAttribute(attributeToSort));
            }
            else if (attributeToSort == "Last Name")
            {
                result = first->getAttribute(attributeToSort).compare(second->getAttribute(attributeToSort));
            }
            else if (attributeToSort == "Jersey Name")
            {
                result = first->getAttribute(attributeToSort).compare(second->getAttribute(attributeToSort));
            }
            else if (attributeToSort == "Pronouns")
            {
                result = first->getAttribute(attributeToSort).compare(second->getAttribute(attributeToSort));
            }
            else if (attributeToSort == "Keeper")
            {
                if ( first->getPosition(attributeToSort) )
                {
                    if ( ! second->getPosition(attributeToSort) )
                        result = -1; //first is selected, second is not
                }
                else
                {
                    if ( second->getPosition(attributeToSort) )
                    {
                        result = 1; //second is selected, first is not
                    }
                }//all other cases, they are equal and result can remain 0
            }
            else if (attributeToSort == "Chaser")
            {
                if ( first->getPosition(attributeToSort) )
                {
                    if ( ! second->getPosition(attributeToSort) )
                        result = -1; //first is selected, second is not
                }
                else
                {
                    if ( second->getPosition(attributeToSort) )
                    {
                        result = 1; //second is selected, first is not
                    }
                }//all other cases, they are equal and result can remain 0
            }
            else if (attributeToSort == "Beater")
            {
                if ( first->getPosition(attributeToSort) )
                {
                    if ( ! second->getPosition(attributeToSort) )
                        result = -1; //first is selected, second is not
                }
                else
                {
                    if ( second->getPosition(attributeToSort) )
                    {
                        result = 1; //second is selected, first is not
                    }
                }//all other cases, they are equal and result can remain 0
            }
            else if ( attributeToSort == "Seeker")
            {
                if ( first->getPosition(attributeToSort) )
                {
                    if ( ! second->getPosition(attributeToSort) )
                        result = -1; //first is selected, second is not
                }
                else
                {
                    if ( second->getPosition(attributeToSort) )
                    {
                        result = 1; //second is selected, first is not
                    }
                }//all other cases, they are equal and result can remain 0
            }
            
            return direction * result;
        }
        
    private:
        String attributeToSort;
        int direction;
    };
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RosterListBoxComponent)
};
