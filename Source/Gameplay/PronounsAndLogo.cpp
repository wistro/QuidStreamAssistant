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
  
  Rectangle<int> left (area.removeFromLeft(proportionOfWidth(0.33f)));
  Rectangle<int> right (area.removeFromRight(proportionOfWidth(0.33f)));
  
  const int rows = area.getHeight() / g.getCurrentFont().getHeight();
  
  if ( rows >= pronounChart.size() )
  {
    g.drawFittedText(pronounChart.joinIntoString("\n"), area, Justification::centred, rows);
  }
  else if ( rows > (pronounChart.size() / 2) )
  {
    String l = "";
    String r = "";
    int half = pronounChart.size() / 2;
    
    for ( int i = 0; i < half; i++ )
    {
      l += pronounChart[i] + "\n";
      r += pronounChart[half + i] + "\n";
    }
    
    if ( pronounChart.size() % 2 )
    {
      r += pronounChart[pronounChart.size()  - 1];
    }
    
    g.drawFittedText(l, left, Justification::centredRight, 0);
    g.drawFittedText(r, right, Justification::centredLeft, 0);
  }
  else
  {
    String l = "";
    String m = "";
    String r = "";
    int third = pronounChart.size() / 3;
    
    switch(pronounChart.size() % 3)
    {
      case 0:
        for ( int i = 0; i < third; i++ )
        {
          l += pronounChart[i] + "\n";
          m += pronounChart[third + i] + "\n";
          r += pronounChart[(third * 2) + i] + "\n";
        }
        break;
      case 1:
        for ( int i = 0; i < third; i++ )
        {
          l += pronounChart[i] + "\n";
          m += pronounChart[third + 1 + i] + "\n";
          r += pronounChart[(third * 2) + 1 + i] + "\n";
        }
        
        l += pronounChart[third];
        break;
      case 2:
        for ( int i = 0; i < third; i++ )
        {
          l += pronounChart[i] + "\n";
          m += pronounChart[third + 1 + i] + "\n";
          r += pronounChart[(third + 1) * 2 + i] + "\n";
        }
        
        l += pronounChart[third];
        m += pronounChart[(third * 2) + 1];
        break;
    }
    
    g.drawFittedText(l, left, Justification::centredRight, 0);
    g.drawFittedText(m, area, Justification::centred, 0);
    g.drawFittedText(r, right, Justification::centredLeft, 0);
  }
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
