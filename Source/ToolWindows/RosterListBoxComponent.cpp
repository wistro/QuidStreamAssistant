/*
  ==============================================================================

    RosterListBoxComponent.cpp
    Created: 1 Jun 2017 10:38:35am
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "RosterListBoxComponent.h"
#include "../Settings/Team.h"
#include "../Settings/Player.h"

//==============================================================================

const StringArray RosterListBoxComponent::columnIds =
{
    "NOZEROTHCOLUMN", //instead of constantly checking (columnId + 1)
    "Number",
    "First Name",
    "Last Name",
    "Jersey Name",
    "Pronouns",
    "Keeper",
    "Chaser",
    "Beater",
    "Seeker"
};

//==============================================================================

RosterListBoxComponent::RosterListBoxComponent()   : font (14.0f)
{
    roster.clear();
    
    ScopedPointer<XmlElement> xml (XmlDocument::parse (BinaryData::default_team_xml));
    if (xml != nullptr)
        readFromXML (*xml);
    
    initBasics();
}

RosterListBoxComponent::RosterListBoxComponent( String teamName ) : font(14.0f)
{
    const File file ( Team::getTeamsFolder().getChildFile(teamName).withFileExtension(Team::getTeamFileSuffix()) );
    
    if ( ! file.existsAsFile() )
    {
        RosterListBoxComponent();
    }
    else
    {
        readFromFile(file);
    }
    
    initBasics();
}

RosterListBoxComponent::~RosterListBoxComponent()
{
    
}

void RosterListBoxComponent::initBasics()
{
    numRows = roster.size();

    addAndMakeVisible(table);
    table.setModel (this);
    
    // give it a border
    table.setColour (ListBox::outlineColourId, getLookAndFeel().findColour(Slider::textBoxOutlineColourId));
    table.setOutlineThickness (1);
    
    table.getHeader().addColumn ("#", 1, font.getStringWidth ("#"), 30, -1,
                                 TableHeaderComponent::visible | TableHeaderComponent::sortable, 0);
    table.getHeader().addColumn("First Name", 2, 100, 100, -1,
                                TableHeaderComponent::visible | TableHeaderComponent::sortable |
                                TableHeaderComponent::resizable);
    table.getHeader().addColumn("Last Name", 3, 100, 100, -1,
                                TableHeaderComponent::visible | TableHeaderComponent::sortable |
                                TableHeaderComponent::resizable);
    table.getHeader().addColumn("Jersey Name", 4, 100, 100, -1,
                                TableHeaderComponent::visible | TableHeaderComponent::sortable |
                                TableHeaderComponent::resizable);
    table.getHeader().addColumn("Pronouns", 5, 100, 100, -1,
                                TableHeaderComponent::visible | TableHeaderComponent::sortable |
                                TableHeaderComponent::resizable);
    table.getHeader().addColumn("Keeper", 6, 30, 30, -1,
                                TableHeaderComponent::visible | TableHeaderComponent::sortable);
    table.getHeader().addColumn("Chaser", 7, 30, 30, -1,
                                TableHeaderComponent::visible | TableHeaderComponent::sortable);
    table.getHeader().addColumn("Beater", 8, 30, 30, -1,
                                TableHeaderComponent::visible | TableHeaderComponent::sortable);
    table.getHeader().addColumn("Seeker", 9, 30, 30, -1,
                                TableHeaderComponent::visible | TableHeaderComponent::sortable);
    
    table.getHeader().setSortColumnId(1, true);
    table.getHeader().setStretchToFitActive (true);
    table.setMultipleSelectionEnabled (true);
}

//==============================================================================

// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
int RosterListBoxComponent::getNumRows()
{
    return numRows;
}

// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
void RosterListBoxComponent::paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    const Colour alternateColour (getLookAndFeel().findColour (ListBox::backgroundColourId)
                                  .interpolatedWith (getLookAndFeel().findColour (ListBox::textColourId), 0.03f));
    if (rowIsSelected)
        g.fillAll (Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll (alternateColour);
}

// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
// components. except every cell has custom components so...
void RosterListBoxComponent::paintCell (Graphics& g, int rowNumber, int columnId,
                                   int width, int height, bool /*rowIsSelected*/)
{
    g.setColour (getLookAndFeel().findColour (ListBox::textColourId));
    g.setFont (font);
    
    if ( rowNumber < numRows )
    {
        g.drawText (roster[rowNumber]->getAttribute(getAttributeNameForColumnId(columnId)), 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    
    g.setColour (getLookAndFeel().findColour (ListBox::backgroundColourId));
    g.fillRect (width - 1, 0, 1, height);
}

//==============================================================================

// This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
// to change the sort order.
void RosterListBoxComponent::sortOrderChanged (int newSortColumnId, bool isForwards)
{
    if (newSortColumnId != 0)
    {
        RosterListSorter sorter (getAttributeNameForColumnId (newSortColumnId), isForwards);
        roster.sort(sorter);
        
        table.updateContent();
    }
}

// This is overloaded from TableListBoxModel, and must update any custom components that we're using
Component* RosterListBoxComponent::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                                       Component* existingComponentToUpdate)
{
    if (columnId >= 6 && columnId <= 9 ) // The Position columns are custom togglebutton components
    {
        ToggleBoxCustomComponent* selectBox = static_cast<ToggleBoxCustomComponent*> (existingComponentToUpdate);
        
        // If an existing component is being passed-in for updating, we'll re-use it, but
        // if not, we'll have to create one.
        if (selectBox == nullptr)
            selectBox = new ToggleBoxCustomComponent (*this);
        
        selectBox->setRowAndColumn (rowNumber, columnId);
        return selectBox;
    }
    else if ( columnId > 0 && columnId <= 5 ) // first 5 columns are editable text
    {
        // The other columns are editable text columns, for which we use the custom Label component
        EditableTextCustomComponent* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
        
        // same as above...
        if (textLabel == nullptr)
            textLabel = new EditableTextCustomComponent (*this);
        
        textLabel->setRowAndColumn (rowNumber, columnId);
        return textLabel;
    }
    else //something's rotten in the state of Denmark
    {
        jassert (existingComponentToUpdate == nullptr);
        return nullptr;
    }
}

// This is overloaded from TableListBoxModel, and should choose the best width for the specified
// column.
int RosterListBoxComponent::getColumnAutoSizeWidth (int columnId)
{
    if (columnId >= 6 && columnId <= 9)
        return 30; // (this is the position switch columns, containing a custom togglebutton component)
    
    int widest = 32;
    
    // find the widest bit of text in this column..
    for (int i = 0; i < numRows; i++)
    {
        widest = jmax (widest, font.getStringWidth (roster[i]->getAttribute(getAttributeNameForColumnId(columnId))));
    }
    
    return widest + 8;
}

//==============================================================================

void RosterListBoxComponent::addBlankPlayer()
{
    roster.add(new Player("I'm NEW!", "Double Click to Edit Me!", "418", "Teapot"));
    numRows++;
    table.updateContent();
}

void RosterListBoxComponent::removeSelected()
{
    for ( int i = 0; i < numRows; i++ )
    {
        if ( table.isRowSelected(i) )
        {
            roster.remove(i);
            numRows--;
        }
    }
    
    table.updateContent();
}

void RosterListBoxComponent::givePi()
{
    
    if ( table.getNumSelectedRows() == 1 )
    {
        roster[table.getSelectedRow()]->setNumber(Player::pi);
    }
    
    table.updateContent();
}

void RosterListBoxComponent::infiniteLove()
{
    //will only do anything if only one row is selected
    if ( table.getNumSelectedRows() == 1 )
    {
        roster[table.getSelectedRow()]->setNumber(Player::infinity);
    }
    table.updateContent();
}

//==============================================================================

// A couple of quick methods to set and get cell values when the user changes them
bool RosterListBoxComponent::getToggled (const int columnNumber, const int rowNumber) const
{
    String pos = getAttributeNameForColumnId(columnNumber);
    return roster[rowNumber]->getPosition(pos);
}

void RosterListBoxComponent::setToggled (const int columnNumber, const int rowNumber, const bool shouldBeOn)
{
    String pos = getAttributeNameForColumnId(columnNumber);
    roster[rowNumber]->setPosition(pos, shouldBeOn);
}

Player* RosterListBoxComponent::getPlayer ( const int rowNumber )
{
    return roster[rowNumber];
}

String RosterListBoxComponent::getText (const int columnNumber, const int rowNumber) const
{
    String entry = getAttributeNameForColumnId(columnNumber);
    
    return roster[rowNumber]->getAttribute(entry);
}

void RosterListBoxComponent::setText (const int columnNumber, const int rowNumber, const String& newText)
{
    const String& columnName = getAttributeNameForColumnId(columnNumber);
    roster[rowNumber]->setAttribute(columnName, newText);
}

//==============================================================================

void RosterListBoxComponent::resized()
{
    // position our table with a gap around its edge
    table.setBoundsInset (BorderSize<int> (3));
}

// (a utility method to search our XML for the attribute that matches a column ID)
String RosterListBoxComponent::getAttributeNameForColumnId (const int columnId) const
{
    return columnIds[columnId];
}

//==============================================================================

void RosterListBoxComponent::readFromXML (const XmlElement& xml)
{
    if ( xml.hasTagName("ROSTER") || xml.hasTagName("TEAM") )
    {
        forEachXmlChildElement(xml, e)
        {
            if ( e->hasTagName("PLAYER") )
            {
                bool keeper, chaser, beater, seeker;
                keeper = chaser = beater = seeker = false;
                String positions = "";
                Player* newPlayer = new Player();
                
                newPlayer->setNumber( e->getStringAttribute("number") );
                newPlayer->setName( e->getStringAttribute("first"), e->getStringAttribute("last"), e->getStringAttribute("jersey") );
                newPlayer->setPronouns( e->getStringAttribute("pronouns") );
                
                positions = e->getStringAttribute("positions");
                
                if ( positions == "utility" )
                    keeper = chaser = beater = seeker = true;
                else
                {
                    if ( positions.contains("keeper") )
                        keeper = true;
                    if ( positions.contains("chaser") )
                        chaser = true;
                    if ( positions.contains("beater") )
                        beater = true;
                    if ( positions.contains("seeker") )
                        seeker = true;
                }
                
                newPlayer->setPositions( keeper, chaser, beater, seeker );
                
                roster.add(newPlayer);
            }
        }
    }
    
    //    refreshRoster();
}

void RosterListBoxComponent::readFromFile (const File& file)
{
    const ScopedPointer<XmlElement> xml (XmlDocument::parse (file));
    readFromXML (*xml);
}
