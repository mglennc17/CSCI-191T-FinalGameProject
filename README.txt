CSCI 191T – Final Project
Driving Game

Team Members:
- Matthew Glenn Crain
- Eliezer Sanchez Lorenzano
- Ricardo Navarro


Overview:
This project is a 3D driving game built in C++ using OpenGL. The player drives a car through multiple levels, avoids traffic, collects items, and races against a time limit. The game includes multiple camera modes, dynamic fog effects, sound, particle effects, collectibles, and a full menu system.


Controls:
W  – Accelerate  
A  – Turn left  
D  – Turn right  
S  – Brake  
P  – Change camera view  
J  – Rotate camera left (third-person camera)  
L  – Rotate camera right (third-person camera)  
H  – Hold to show in-game help  
ESC – Pause game  


Team Contributions: (BASED OFF OF GITHUB)

Matthew Glenn Crain:
- used lesson 12 as a base for the game
- "found a low poly car model and converted it to md2, spinning car in menu"
- updated car UV Map and texture "the easiest way i found to do this was using blender to unwrap the UV and draw the textures, then exporting the model as a .obj file, then converting the .obj file into .md2 in Misfit Model 3D"
- added basic menu and switch statement for gamestate "buttons react to being hovered over and change size"
- added main game code "camera follows player, obstacle cars spawn randomly on the road and player an move left and right to avoid them"
- added menu and game music
- added obj and mtl file reader
- added better acceleration and dynamic fov
- added basic text display class
- added basic score and UI system
- animated car -> car tilt, tires spinning
- added different levels with level select menu
- added fog for deathscreen
- bug fixes on his own stuff and other's as well



Eliezer Sanchez Lorenzano:
- Added collectibles "added coins and dollars. They spin in place. When a coin is collected a '1' floats to show the points earned and a '4' for the dollar. The dollar is rare and it has 1 to 8 chances to coming out since they are worth more
- changed scoring to total amount of points and not based on speed
- added different POV modes
- added sounds to when the collectibles are collected
- added a braking system, moved the scoring ui a bit
- added in-game help menu
- added rotation of the camera using keys L and J
- added brake lights to car model
- collision with collectibles
- bug fixes on his own stuff and other's as well


Ricardo Navarro:
- implemented collision detection with other cars, camera shakes, sound effect
- 3 hit collision system
- crashed gameOver screen and crashed game over logic
- sound effect for crashing a third time
- added save system that saves the amount of dollar bills collected in a save.dat folder.
- Changed the point of the game, "Yellow coins only increase score and extend time, dollar bills are solely used for buying other cars that the player can use"
- updated text display for collecting dollar bills and coins, updated sound
- added gameOver screen for when timer runs out
- added another obstacle to work towards npc variety
- added particle effect when turning and braking