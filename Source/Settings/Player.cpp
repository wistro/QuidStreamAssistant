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

const String Player::digits = "0123456789";

//if playing by USQ RB10 or below rules, comment this line and uncomment the next
const String Player::allowedLetters = "AGHJKNPRWXY"; //IQA RB does not allow M or T
//const String Player::allowedLetters = "AGHJKMNPRTWXY"; //USQ letters include M and T

const String Player::pi = CharPointer_UTF8 ("\xCF\x80");
const String Player::infinity = CharPointer_UTF8 ("\xe2\x88\x9e");
const String Player::hashtag = "#";

const String Player::allowedNumbers = Player::digits + Player::allowedLetters + Player::pi + Player::infinity + Player::hashtag;

//==============================================================================
Player::Player()
{
    setPronouns();
    setPositions();
    reset();
}

Player::Player( String first, String last, String num, String jersey, String pn,
       bool k, bool c, bool b, bool s )
{
    setName(first, last, jersey);
    setNumber(num);
    setPronouns(pn);
    
    setPositions(k, c, b, s);
    
    reset();
}

Player::~Player()
{
    
}

//==============================================================================

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

void Player::setAttribute(String attribute, String data)
{
    if ( attribute == "Number" )
    {
        setNumber(data);
    }
    else if ( attribute == "First Name" )
    {
        firstName = data;
    }
    else if ( attribute == "Last Name" )
    {
        lastName = data;
    }
    else if ( attribute == "Jersey Name" )
    {
        jerseyName = data;
    }
    else if ( attribute == "Pronouns" )
    {
        pronouns = data;
    }
}

void Player::setPosition(String position, bool data)
{
    if ( position == "Keeper" )
    {
        keeper = data;
    }
    else if ( position == "Chaser" )
    {
        chaser = data;
    }
    else if ( position == "Beater" )
    {
        beater = data;
    }
    else if ( position == "Seeker" )
    {
        seeker = data;
    }
}

//==============================================================================

String Player::getNum()
{
    return number;
}

String Player::getRosterEntry()
{
    return (number + " " + jerseyName);
}

String Player::getFirst()
{
    return firstName;
}

String Player::getLast()
{
    return lastName;
}

String Player::getJersey()
{
    return jerseyName == "" ? lastName : jerseyName;
}

String Player::getPronouns()
{
    return pronouns;
}

String Player::getAttribute(String attribute )
{
    String result = "";
    
    if ( attribute == "Number" )
    {
        result = getNum();
    }
    else if ( attribute == "First Name" )
    {
        result = getFirst();
    }
    else if ( attribute == "Last Name" )
    {
        result = getLast();
    }
    else if ( attribute == "Jersey Name" )
    {
        result = getJersey();
    }
    else if ( attribute == "Pronouns" )
    {
        result = getPronouns();
    }

    return result;
}

bool Player::getPosition(String position)
{
    bool result = false;
    
    if ( position == "Keeper" )
    {
        result = keeper;
    }
    else if ( position == "Chaser" )
    {
        result = chaser;
    }
    else if ( position == "Beater" )
    {
        result = beater;
    }
    else if ( position == "Seeker" )
    {
        result = seeker;
    }
    
    return result;
}


//==============================================================================

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

void Player::setPositions(bool k, bool c, bool b, bool s)
{
    keeper = k;
    chaser = c;
    beater = b;
    seeker = s;
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
