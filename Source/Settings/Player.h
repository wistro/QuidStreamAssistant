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
class Player : public Component
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
  void setPositions(bool keeper = true, bool chaser = true, bool beater = true, bool seeker = true);

  String getNum();
  String getRosterEntry();
  String getFirst();
  String getLast();
  String getJersey();
  String getPronouns();
  
  void setAttribute(String attribute, String data);
  String getAttribute(String attribute);
  void setPosition(String position, bool data);
  bool getPosition(String position);
  
  void scored();
  void gotCarded( char colour );
  bool ejected();
    
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
  static const String digits;
  static const String allowedLetters;
  static const String pi;
  static const String infinity;
  static const String hashtag;

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

//==============================================================================

//small class to allow for sorting Player objects by number
//sorts integers first chronologically
//then letters alphabetically
//then pi, infinity and number symbols in that order
/***************************************************************
  IF EITHER PLAYER HAS AN UNALLOWED NUMBER THIS WILL BREAK HARD
 ***************************************************************/
class PlayerComparator
{
public:
  static int compareElements (Player* first, Player* second)
  {
    const int oneFirst = -1;
    const int equal = 0;
    const int twoFirst = 1;
    String one, two;
    int returnValue = 0;
    
    one = first->getNum();
    two = second->getNum();
    
    if ( one.containsOnly(Player::digits) )
    {
      if ( two.containsOnly(Player::digits) )
      {
        if ( one.getIntValue() < two.getIntValue() )
          returnValue = oneFirst;
        else if ( one.getIntValue() == two.getIntValue() )
          returnValue = equal;
        else if ( one.getIntValue() > two.getIntValue() )
          returnValue = twoFirst;
      }
      else
        returnValue = oneFirst;
    }
    else if ( two.containsOnly(Player::digits) ) //here, P2's number is an integer while P1's is not
    {
      returnValue = twoFirst;
    }
    else if ( one.containsOnly(Player::allowedLetters) ) //now we know neither player uses an integer as their number
    {
      if ( two.containsOnly(Player::allowedLetters) )
      {
        returnValue = one.compareIgnoreCase(two);
      }
      else
      {
        returnValue = oneFirst;
      }
    }
    else if ( one == Player::pi )
    {
      if ( two == Player::pi )
      {
        returnValue = equal;
      }
      else
      {
        returnValue = oneFirst;
      }
    }
    else if ( two == Player::pi ) //if we've gotten this far, P1 is either infinity or hashtag, so P2 comes first
    {
      returnValue = twoFirst;
    }
    else if ( one == Player::infinity )
    {
      if ( two == Player::infinity )
      {
        returnValue = equal;
      }
      else
      {
        returnValue = oneFirst;
      }
    }
    
    return returnValue;
  }
};
