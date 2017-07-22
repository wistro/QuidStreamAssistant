# QuidStreamAssistant

cross-platform app to make it easy to add score-reporting, time-reporting and other information to a Quidditch Livestream.

This app handles the stats, team info and tournament info for a quidditch livestream. It creates default graphic files in an Overlay folder on your computer (defaults to <HOME DIRECTORY>/QuidStreamAssistant/Overlays, but can be changed by user) and these graphic files will automatically recieve output from the main app and display it on your stream.

## DISPLAYING THE OUTPUT

Using OBS Studio, or any other livestreaming software with Browser Display Support, target the file "inGame.html" in the Overlays folder (above) and set the size of the browser dispaly to the size of your stream

### NOTE ON THE GRAPHICS

They are currently hardcoded to look nicely on a 1080p stream. If your stream resolution is smaller than this, you'll have to adjust the size values in the Overlays/scripts/ css files (there are currently 4 of them).

More when I have actually written anything.

kthxbai
