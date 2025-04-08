Game Title: Fairy Garden
Name and Student ID: Aria Wong 101300465
Operating Systems Used: Linux (Ubuntu on my desktop at home) and Windows 11 (on my laptop)

Texture Sources:
TBA

How to Play:
You're a fairy defending your garden from invading evil fairies. First, you must defeat several waves of evil fairies. When you defeat all fairies in one wave, the next one will start. Once you pass wave 10, the fairy queen becomes frustrated with her incompetent minions and fights you herself. You will be teleported to the center of the map for this fight. Once you defeat the fairy queen, you have successfully defended your garden against the evil fairies and you win.

Controls:
Move - WASD
Shoot - Left click and move cursor to the direction that you want to aim
1-2 - Select 1 or 2 to change weapons (you unlock the second weapon at wave 5)

Note: The player hitbox is much smaller than the sprite which will allow you to move more precisely through the projectiles.

Known Bugs
- Health is supposed to be capped at 5, but sometimes the dark fairy dust can mess this up and temporarily cap your health at 4 or 6.
- If you wait too long after a dying earth enemy loses its arm, it could leave the map (although very slowly). In which case, you would might have to restart the game.

Requirements
- The game is a top-view aircraft game taking place in the sky above your village and the "aircrafts" are fairies

Game mechanics: There are waves of fairies to clear and also a boss (i.e fairy queen) at the end to defeat.

Enemies
1. Magic Missile Fairy: Moves towards player and additional up and down movement. Shoots projectiles that home towards the player. The code is in magic_missile_enemy, magic_missile_projectile and magic_missile_weapon files.
2. Water Wave Fairy: Moves towards the player and flees if the player is too close, it also "hops". Shoots two projectiles that move in a circle in the direction of the player. The code is in water_wave_enemy, water_wave_projectile and water_wave_weapon files.
3. Dying Earth Fairy: Move towards the player while swinging arm which uses hierarchical transformations. If this fairy loses its arm, but is still alive (i.e from using the long ranged weapon), then it will become scared and flees from the player while shooting a slow moving homing missile every few seconds (so the player can know the location of it) This enemy also has 3 HP while the rest have 1. The code is in dying_earth_enemy and dying_earth_enemy_link files. The dying_earth_enemy_link file has the children code and the dying_earth_enemy is the parent.
4. Dark Fairy Queen (Boss): Stays in one place. Chooses between a magic missile and a water wave attack randomly. At 2/3 and 1/3 health, the number of attacks that the dark fairy queen will perform at once increases. The code is in the dark_fairy_queen files.

Weapons
1. Shotgun: Allows the player to shoot multiple projectiles in an arc. It has a slower fire rate than the other weapon and its range is not very far. The code is in shotgun_player_weapon files.
2. Long Ranged: Allows the player to shoot one projectile at a time. It has a faster fire rate and can go much further than the shotgun. The code is in default_player_weapon files.

Collectibles
1. Fairy Dust: Spawns on a cooldown at a random location around the player. It makes the player shoot faster for a short period of time and gives them one additional health point. The code is in fairy_dust_collectible files.
2. Rainbow Fairy Dust: Spawns on a cooldown at a random location around the player, but less often than the fairy dust. It gives the player an additional health point, make the player invincible, shoot faster and move faster for a short period of time. The code is in rainbow_fairy_dust files.
3. Dark Fairy Dust: Spawns as a 5% drop chance from enemies. It gives the player an additional health point, makes the player invincible, makes the player move very fast and increases the player's hitbox allowing you to devour any enemies near you for a short period of time. The code is in dark_fairy_dust files.

Movement and Transformations
- The movement is handled in game through transformations in the game objects' render functions.
- An example of physically-based movement in this game is in dying_earth_enemy.cpp Move function where it chases and flees from the player.
- An example of parametric-based movement in this game is in the water_wave_projectile.cpp move function where it uses a parametric circle function to move in a circle.

Collision Detection
- There is collision detection between player, enemies and collectibles

Game World
- The game world is bigger than the view which the player can move around in
- The game world uses texture tiling to make not stretch the texture too large, this can be seen around line 160 of game.cpp as well as in background_fragment_shader.glsl

Particle Systems
- The player's long ranged weapon projectiles has a trail particle system.
- When the player gets hit, there is a short explosion particle system.
- I allowed for changing of particle system shapes in particles.cpp file around line 68.

Hierarchical Transformations
- The dying earth enemy uses three links of hierarchical transformations using it's arm.
- The links can be seen to move independently of each other because of how it swings

UI
- The HUD shows score, wave, enemies left, health and weapons

Advanced Method
- The dying earth enemy has a more tactical AI in that it goes towards the player when it has its arm and runs away when it doesn't
