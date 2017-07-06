/*
  ==============================================================================

    PronounsAndLogo.cpp
    Created: 6 Jul 2017 5:13:35pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#include "../Settings/OSDependencyThings.h"
#include "PronounsAndLogo.h"

//==============================================================================
PronounsAndLogo::PronounsAndLogo()
{
  logo.toBack();
  addAndMakeVisible(logo);
  
//  column1.setJustificationType(Justification::centredLeft);
//  addAndMakeVisible(column1);
//  
//  column2.setJustificationType(Justification::centredRight);
//  addAndMakeVisible(column2);
}

PronounsAndLogo::~PronounsAndLogo()
{
}

void PronounsAndLogo::paint (Graphics& g)
{
  Rectangle<int> area (getLocalBounds());
  
  logo.setBounds(area);
  g.drawFittedText(pronounChart.joinIntoString("   "), area, Justification::centred, (area.getHeight() / g.getCurrentFont().getHeight()) );

}

void PronounsAndLogo::resized()
{
}

//void PronounsAndLogo::setText(StringArray text)
//{
//  const int halfSize = text.size() / 2;
//  
//  StringArray part1, part2;
//  
//  for (int i = 0; i < halfSize; i++)
//  {
//    part1.add(text[i]);
//    part2.add(text[(text.size() - 1) - i]);
//  }
//  
//  part1.addIfNotAlreadyThere(text[halfSize]); //without this, we'd miss one person if there are an odd number of players
//  
//  part1.sortNatural();
//  part2.sortNatural();
//  
//  column1.setText(part1.joinIntoString("\n"), dontSendNotification);
//  column2.setText(part2.joinIntoString("\n"), dontSendNotification);
//}
