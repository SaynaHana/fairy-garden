Name and Student ID: Aria Wong 101300465
Operating Systems Used: Linux (Ubuntu on my desktop at home) and Windows 11 (on my laptop)

Texture Sources:
I created the smiley, invincible smiley, neutral and frowny textures myself.
I got the explosion texture from https://opengameart.org/content/explosion-3 under CC0 license.
I got the coin texture from https://opengameart.org/content/gold-cointoken under CC0 license.

Part 1: Collectible items
- Added collectible_game_object files that contain the behaviour of the collectible items
- Scattered collectible items around the world around line 80 of game.cpp
- Added an invincible texture for the player
- Made the player invincible for 10 seconds in the player OnCollision and SetInvisible functions

Part 2: Enemy game objects
- Added enemy_game_object files that contain the behaviour of the enemy game objects
- Added private state_ property to enemy_game_object.h
- Added eliptical movement in Patrol() function of enemy
- Added movement towards target in Intercept() function of enemy
- The properties of Intercept and Patrol are set in the constructor of the enemy by MoveData and PatrolData respectively (these are in move_data.h and patrol_data.h)
- Made the target of the motion updated in the Intercept function
- Made the enemy switch from Intercept to Patrol and vice versa in Detect function of the enemy

Part 3: Enemy initialization 
- Modified enemy spawning from assignment 1 around line 166 in game.cpp
