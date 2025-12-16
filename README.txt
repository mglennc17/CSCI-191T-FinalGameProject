CSCI 191T – Final Project
Driving Game

Team Members:
- Ricardo Navarro
- Matthew Glenn Crain
- Eliezer Sanchez Lorenzano


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


Team Contributions:

Ricardo Navarro:
- Implemented collision detection and refined crash handling
- Added the third-crash logic and time out game over behavior
- Implemented particle effects (dust when braking/turning)
- Added floating score/time popups
- Worked on main game flow logic and polishing gameplay behavior
- Helped debug model and obstacle behavior

Matthew Glenn Crain:
- Built the core game structure and main game loop
- Implemented level system, level transitions, and level select menu
- Added dynamic fog effects and fog behavior on death
- Implemented camera systems, including POV modes and dynamic FOV
- Created and integrated UI elements (menus, score display, timers)
- Added terrain, skyboxes, bridges, tunnels, and environment visuals
- Implemented object loaders (OBJ/MTL) and texture handling

Eliezer Sanchez Lorenzano:
- Implemented player movement, steering, braking, and rotation behavior
- Added collectibles (coins and dollars) and pickup logic
- Implemented in-game help menu
- Added brake lights and visual feedback
- Helped tune gameplay feel, controls, and collision behavior
- Assisted with testing and bug fixes throughout development