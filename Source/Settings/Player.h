/*
  ==============================================================================

    Player.h
    Created: 28 May 2017 2:37:56pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

#include "OSDependencyThings.h"

//==============================================================================
/*
*/
class Player
{
public:
    Player();
    Player( String first, String last, String num, String jersey = "", String pn = they,
           bool k = true, bool c = true, bool b = true, bool s = true );
    ~Player();
    
    void reset();
    
    void setName(String first, String last, String jersey="");
    void setNumber(String num);
    void setPronouns(String pn = they);

    String getNum();
    
    void scored();
    void gotCarded( char colour );
    bool ejected();
    
    void utility();
    
    bool isKeeper();
    bool isChaser();
    bool isBeater();
    bool isSeeker();
    bool isUtility();
    
    static const char blue = 'b';
    static const char yellow = 'y';
    static const char red = 'r';
    
    static const String they;
    static const String he;
    static const String she;

    static const String allowedNumbers;
    static const String pi;
    static const String infinity;

    

private:
    
    String firstName;
    String lastName;
    String number;
    String jerseyName;
    String pronouns;
    
    bool keeper;
    bool chaser;
    bool beater;
    bool seeker;
    
    int blueCards;
    int yellowCards;
    bool redCard;
    
    int hoops;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Player)
};
