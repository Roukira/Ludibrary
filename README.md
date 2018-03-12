Want to give it a try ?
Download :
- Windows : [Download latest version](https://github.com/Roukira/Ludibrary/raw/master/Ludibrary%20v0.7.zip)

Tutorial : [Here.](https://github.com/Roukira/Ludibrary/blob/master/tutorial/guide.md)

________________________________________________________________________________________________________________________________________

# Ludibrary v0.7 : User Stats, Improved Game Timer, Game State, Improved Game Design, External Software support

Hello !

It's been a month since the last update, and I'm bringing a lot of stuff ! This update contains some stuff I also worked on before but wasn't ready.

## General Changes :
* Improved Game Timer : Behavior is now different for games using multiple child processes. It will now detect child of child processes, which was an issue before for games with launchers which would stop the timer.
* Improved UI :
	* Introduced a status bar on bottom, which will only be visible when loading a game for now.
	* New design for games : you can now hover over games and get the buttons there instead of the old one. Can be reverted on settings.
	* Replaced the ugly side info by a blue I button which when hovered shows game info inside the window without moving games.

## New features : 
* User Stats System :
	* Created a tab that shows the user stats in real time.
	* Contains general stats and specific stats per platform, emulator and software.

* Game State :
	* Each game now has a state that describes how it is used.
	* Possible values : Completed, Dropped, Plan to Play, Regularly Playing, Never Played.
	* It will modify itself depending on what you do, but can also be modified manually.

* External Softwares : 
	* You can now run external softwares with any game or platform you want !
		
### Several fixes : 
* Fixed CEMU not loading.
* Fixed specific UI issues.
* Fixed time played on game displayed without "s" when it's at 1.
* Fixed game description being editable.
* Added possibility to grab All platforms or No platform in add dialogs.
* Fixed system tray not disappearing when force closed.
* Several minor fixes.

[Download this version](https://github.com/Roukira/Ludibrary/raw/master/Ludibrary%20v0.7.zip)

________________________________________________________________________________________________________________________________________

# Ludibrary v0.6.0 : More games compatibility and Game Timer Update

Hello !

Big update with small update log, it took quite some time but it finally works!

Changes :
- Ludibrary can now run all PC games, including steam and Launcher-based games.
The only issue could be games opened through specific emulators which I will work on later on.

- Game timer : 
Ludibrary finally features a fully functional game timer.
It will count time played and update in real-time. It was pretty hard to make it work, and even harder for launcher based games which had child processes to deal with but now it works !

- Added more detail when a game search has no result.
- Fixed "Cancel" button on Add Button.

And that's it !

[Download this version](https://github.com/Roukira/Ludibrary/raw/master/Ludibrary%20v0.6.0.zip)

________________________________________________________________________________________________________________________________________

# Ludibrary v0.5 : Notification Update

Hello !

Small update there fixing some bugs and adding a little nice feature : Notifications.
It will notify you inside the software when you add a game, platform, or when you run a game.

Changes :
- Fixed a lot of bugs on emulators.
- Fixed a crash happening at some games added.
- Notifications.

And that's it !

[Download this version](https://github.com/Roukira/Ludibrary/raw/master/Ludibrary%20v0.5.zip)

________________________________________________________________________________________________________________________________________

# Ludibrary v0.4 : Small Emulator Update

Hello !

I've been working on an emulator update for quite some time and it's now ready !
Fixed some major bugs, and added supported emulators, which when put into the emulators folder will automatically be loaded for corresponding platforms.
This update is a small one for emulators, I plan to add much more into it but later, when I finish other things.

Changes :
- Fixed a bug with steam url shortcuts on desktop not working for game .exe location.
- Supported emulators feature : It will allow you when you add the emulator to directly play games without configuring each game emulator when the platform is supported.
List of supported emulators as of now (it will grow later !) :
	- Nestopia
	- SNES9X
	- Project 64
	- Dolphin
	- Cemu
	- Visual Boy Advance
	- Desmume
	- Citra
	- ePSXe
	- PCSX2
- You can now edit your emulator settings, changing emulator path, and choose as default for each platform.
- Fixed a bug that made Ludibrary crash when trying to use detected games feature.
- Removed some weird and useless texts.
- Added a setting for emulators directory.

And that's it !

[Download this version](https://github.com/Roukira/Ludibrary/raw/master/Ludibrary%20v0.4.zip)

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


