Name and Student ID: Aria Wong 101300465
Operating Systems Used: Linux (Ubuntu on my desktop at home) and Windows 11 (on my laptop)

Texture Sources:
I created the smiley, invincible smiley, neutral and frowny textures myself.
I got the explosion texture from https://opengameart.org/content/explosion-3 under CC0 license.
I got the coin texture from https://opengameart.org/content/gold-cointoken under CC0 license.

Part 1: Player physical movement
- Changed the behaviour that happens when a WASD and QE key is pressed around line 148 of game.cpp
- Added SetAcceleration function around line 57 of game_object.h
- Added an invincible texture for the player
- Added acceleration, velocity and position math to the Update function of game_object.cpp to allow for physically based motion

Part 2: Projectiles
- Added spacebar key press behaviour around line 175 of game.cpp
- Added projectile_game_object.cpp and projectile_game_object.h files that contain the behaviour of the projectile's collision and destroy timer
- Added Shoot function to player_game_object.cpp which spawns a projectile and adds acceleration to it
- The player_game_object also has a timer which is checked in its Update function and also set in the SetCanShoot function which creates a cooldown for when the player can shoot next

Part 3: Projectile lifespan
- Used Timer object in Projectile's Update function to check if the projectile should be destroyed
- The lifetime of the projectile is specified in its constructor

Part 4: Enemy spawning
- Reused enemy spawning from assignment 2, but increased the radius by 3 times around line 206

Part 5: Projectile and enemy collision using ray-to-circle collision
- Created header file called Constants.h which contains enums that can be used in multiple classes
- Added CollisionType enum to Constants.h which can be used to say if a gameobject uses a circle or ray collider
- Added CollisionType property to GameObject which is set in the constructor
- The default CollisionType is circle, but it is changed in the projectile_game_object.cpp constructor so that the projectile uses a ray collider
- Added check in game.cpp around line 259 to see if the collision between two objects is circle-circle or ray-circle
- Added ray-circle collision behaviour in game.cpp starting around line 280

Part 6: Continuous scrolling
- Added a matrix called transl_matrix around line 349 of game.cpp that gets multiplied to the view matrix to keep the camera centered on the player
- Created a background_fragment_shader.glsl which just increases the uv coordinates of the background which will make it repeat more
- Changed the texture wrapping behaviour around line 482 in game.cpp to repeat instead of clamp
- Initialized background_sprite_shader around line 435 in game.cpp which uses the new background fragment shader and applied it to the background object

Part 7: Scale
- Changed scale_ property in game_object.h around line 69 to use glm::vec2 instead of float
- Created new gameobject around line 93 that sets the scale of a gameobject to a non-uniform scale to demonstrate (it appears as a big blue smiley face in the game)

Part 8: Ghost
- Created ghost_fragment_shader.glsl which uses a uniform to check if the object is in ghost mode and changes the colour based on that
- Create ghost_sprite_shader_ around line 438 of game.cpp which uses the new ghost_fragment_shader and applied it to all of the collectible objects
- Added collected flag to collectible_game_object which gets set to true in its OnCollision function
- Overrided the CollectibleGameObject Render function to set the uniform if the object is in ghost mode
