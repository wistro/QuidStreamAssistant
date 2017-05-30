/*
  ==============================================================================

    Player.cpp
    Created: 28 May 2017 2:37:56pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "OSDependencyThings.h"
#include "Player.h"

const String Player::they = "they|them|their";
const String Player::he = "he|him|his";
const String Player::she = "she|her|hers";

const String Player::infinity = CharPointer_UTF8 ("\xe2\x88\x9e");
const String Player::pi = CharPointer_UTF8 ("\xCF\x80");

const String Player::allowedNumbers = "0123456789AGHJKNPRWXY#" + Player::infinity + Player::pi ;

//==============================================================================
Player::Player()
{
    setPronouns();
    utility();
    reset();
}

Player::Player( String first, String last, String num, String jersey, String pn,
       bool k, bool c, bool b, bool s )
{
    setName(first, last, jersey);
    setNumber(num);
    setPronouns(pn);
    
    keeper = k;
    chaser = c;
    beater = b;
    seeker = s;
    
    reset();
}

Player::~Player()
{
    
}

void Player::setName(String first, String last, String jersey)
{
    firstName = first;
    lastName = last;
    
    if ( jersey.isEmpty() )
        jerseyName = lastName;
    else
        jerseyName = jersey;
}

void Player::setNumber(String num)
{
    number = num;
}

void Player::setPronouns(String pn)
{
    pronouns = pn;
}

void Player::reset()
{
    hoops = 0;
    blueCards = 0;
    yellowCards = 0;
    redCard = false;
}

String Player::getNum()
{
    return number;
}

void Player::scored()
{
    hoops += 10;
}

void Player::gotCarded( char colour )
{
    if ( colour == blue )
        blueCards++;
    else if ( colour == yellow )
    {
        yellowCards++;
        if ( yellowCards == 2 )
            redCard = true;
    }
    else if ( colour == red )
        redCard = true;
}

bool Player::ejected()
{
    return redCard;
}

void Player::utility()
{
    keeper = true;
    chaser = true;
    beater = true;
    seeker = true;
}

bool Player::isKeeper() { return keeper; }
bool Player::isChaser() { return chaser; }
bool Player::isBeater() { return beater; }
bool Player::isSeeker() { return seeker; }

bool Player::isUtility()
{
    bool utility = false;
    
    if ( isKeeper() && isChaser() && isBeater() && isSeeker() )
        utility = true;
    
    return utility;
}
