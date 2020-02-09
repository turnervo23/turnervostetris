# turnervostetris
Personal project - a Tetris clone made in C++ with SDL

Turner Vo's Tetris:
This program is a fully functional Tetris clone made in C++ with SDL (Simple DirectMedia Layer), using Microsoft Visual Studio 2017. It features an endless mode with dynamic speed levels and scoring. It adheres (mostly) to the "Tetris Guideline" specified here: 
https://harddrop.com/wiki/Tetris_Guideline
But please note that I am NOT trying to pass this off as an official Tetris game. This is just a personal project, please don't sue me.

Background: Tetris is a classic video game that pretty much everyone has played. I've always enjoyed it to an extent--when I was a kid I used to play it on my calculator or iPod during school, but I never really got too invested in it. That changed in 2019 with the release of Tetris 99, the "battle royale" Tetris game for the Nintendo Switch. I ended up sinking tons of hours into this game, practicing and learning all the neat tricks you could do in Tetris. Eventually I had the idea to make my own Tetris game for fun, and this project was born.

Tetris sounds like a quick and easy project to make, but it turns out there are a LOT of minute details that need to be considered in order to make it feel like a "real" Tetris game and not a lazy knock-off. These include:
- Ability to view upcoming pieces
- Ability to "hold" a piece for later
- Displaying a "ghost block" indicating where the piece will land
- 7-bag random generator
- SRS rotation system
- Scoring system that recognizes different clear types and T-Spins
- Many variables that need to be tuned like lock delay, DAS, line clear delay, soft drop speed
- And much more!

I originally started this project with the intent to eventually make it as convincing and feature-complete as an official game, but as time went on and I became too busy with schoolwork and other stuff in life, I had to dial back the scope in order to release a finished version in a reasonable amount of time. So, the game might seem kind of bare-bones when you boot it up, but I find it to still be very fun to play despite the lack of content.

There is still plenty more work to be done on this project if I ever decide to return to it, such as
- Turning off automatic key repeating for rotate, hard drop, etc.
- Refine debug options - click to create block, pause block drop, respawn block, etc.
- Pause menu with resume/restart/return to menu
- Sound effects + music
- Customizable controls, graphics and other settings
- High scores
- Alternate modes (40L, Ultra, Survival, etc.)

But for now, this is the final version.
