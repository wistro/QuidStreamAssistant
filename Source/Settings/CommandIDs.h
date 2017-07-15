/*
  ==============================================================================

    CommandIDs.h
    Created: 11 Jul 2017 10:11:57pm
    Author:  Willow Rosenberg

  ==============================================================================
*/

#pragma once

/*
 A namespace to hold all the possible command IDs.
 */
namespace CommandIDs
{
  enum
  {
    showEditTournamentWindow  = 0x300020,
    showSelectTeamsWindow     = 0x300021,
    showIntroWindow           = 0x300022,
    showHRSnitchWindow        = 0x300023,
//    showAddTeamWindow         = 0x300024,
    showAboutWindow           = 0x300025,
    
        
    lastCommandIDEntry
  };
}
