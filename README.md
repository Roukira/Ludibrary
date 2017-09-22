Want to give it a try ?
Download :
- Windows : [Download latest version](https://github.com/Roukira/Ludibrary/raw/master/Ludibrary%20v0.3.zip)

________________________________________________________________________________________________________________________________________

# Ludibrary v0.3 : Play Update

Hello !

Finally finished working on this update. You can now add most emulators and link them to a platform. 
You can also add any type of game(.exe or anything else) : this means that you can run your emulated games directly !

Changes :
- Added a way to manually add an emulator. (You can not delete them yet without an external program, sorry !)
Note that most emulators work (most famous ones) but I didn't make it work for any emulator yet, I'll add a list of known supported emulators when I get to do it,and also provide myself the emulators (optional ofcourse) on next version.
- You can now run an emulated rom by editing the game's path and adding your emulator.
- Added an emulator viewer (it's not looking very good but I'll work on that thing later).
- Fixed a crash happening when pressing the delete button without selection.
- New setting is up active by default : when you delete all the games of a platform, the platform is deleted aswell (this also means that an emulator for that platform won't be valid anymore, even if you add back the platform, you'll have to add again the emulator without deleting it, sorry another thing to improve :(

That's it for now.

[Download this version](https://github.com/Roukira/Ludibrary/raw/master/Ludibrary%20v0.3.zip)

________________________________________________________________________________________________________________________________________

# Ludibrary v0.2

A new version is out !

Changes : 
- I got rid of folders storage Data/etc... and I am now using a SQLite local database. It will allow later easy import/export.
- I've rewritten half of the program which makes it much faster and smoother, and got rid of most known bugs. The other half is set for later.
- I've implemented a settings feature to the software, and also a settings.ini file for easy import/export.
- Search bar is a lot faster, and there's a little animation gif while waiting.
- Edit button working for any game, which will allow to edit a game settings (for now only the exe path).
- You can now run any PC game added to the list, just by adding the exe path on the edit menu.
I've added a play button instead of double-clicking the game which I think is better.
- I got rid of global variables which were useful but quite messy. 
- More info added into settings and help menus.
- You can't now run 2 instances of the software, a warning message will pop up.
- The only setting available right now is smooth scrolling, which basically allows to scroll pixel per pixel.
- The app .exe now has an icon !

[Download this version](https://github.com/Roukira/Ludibrary/raw/master/Ludibrary%20v0.2.zip)

________________________________________________________________________________________________________________________________________

# Ludibrary v0.1
Ludibrary is a (mostly hobby) **Windows** software project. It will allow users to list their games (on any platform)
and even launch them if they set them up to. It is using [thegamesdb database](http://www.thegamesdb.net) API to search for games and corresponding informations and image. (Thanks for this awesome database !)

Please keep in mind this is still an early version of what I'm aiming for.

Want to give it a try ? [Download.](https://github.com/Roukira/Ludibrary/raw/master/Ludibrary%20v0.1.zip)

Screenshots of the current version :
![Main Menu listing all games](https://raw.githubusercontent.com/Roukira/Ludibrary/master/screenshot1.png?raw=true)

![Search for games](https://raw.githubusercontent.com/Roukira/Ludibrary/master/screenshot2.png?raw=true)


