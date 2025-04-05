#include <cmath>
#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/string_cast.hpp>
#include <SOIL/SOIL.h>
#include <iostream>

#include <path_config.h>

#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
#include "game.h"

#include "collectible_game_object.h"
#include "explosion_game_object.h"
#include "enemies/enemy_game_object.h"
#include "projectile_game_object.h"
#include "weapons/interval_weapon.h"
#include "weapons/magic_missile_weapon.h"
#include "weapons/water_wave_weapon.h"
#include "weapons/default_player_weapon.h"
#include "weapons/shotgun_player_weapon.h"
#include "enemies/spawner.h"
#include "ui/text_game_object.h"
#include "particles/particle_system.h"
#include "enemies/dark_fairy_queen.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "COMP2501 Final Project - Aria Wong";
unsigned int window_width_g = 800;
unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;

Game* Game::gamePtr = nullptr;

void Game::SetupGameWorld(void)
{

    // Setup the game world

    // **** Load all the textures that we will need
    //
    // Declare all the textures here
    std::vector<std::string> textures;

    score = 0;
    damage_taken_ = 0;
    collectibles_collected_ = 0;
    enemies_killed_ = 0;
    waves_cleared_ = 0;
    start_time_ = glfwGetTime();

    // CHANGE: Moved textures enum to header file so I can use it outside of this function

    // CHANGE: changed the textures of the destroyers to smiley, neutral and frowny
    textures.push_back("/textures/tex_player.png");
    textures.push_back("/textures/tex_magic_missile_enemy.png"); 
    textures.push_back("/textures/tex_water_wave_enemy.png");
    textures.push_back("/textures/stars.png");
    textures.push_back("/textures/orb.png");
    textures.push_back("/textures/explosion.png");
    textures.push_back("/textures/coin.png");
    textures.push_back("/textures/invincible_smiley.png");
    textures.push_back("/textures/bullet.png");
    textures.push_back("/textures/tex_enemy_projectile.png");
    textures.push_back("/textures/tex_player_projectile.png");
    textures.push_back("/textures/tex_water_projectile.png");
    textures.push_back("/textures/tex_fairy_dust.png");
    textures.push_back("/textures/tex_rainbow_fairy_dust.png");
    textures.push_back("/textures/tex_dark_fairy_dust.png");
    textures.push_back("/textures/font.png");
    textures.push_back("/textures/tex_dying_earth_enemy.png");

    // Load textures
    LoadTextures(textures);


    // **** Setup all the game objects in the world

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector
    MoveData player_move_data = MoveData(2, nullptr);
    GameObjectData player_obj_data = GameObjectData(sprite_, &sprite_shader_, tex_[tex_red_ship], 2.0f, 0.01f);

    // Create player weapons
    std::vector<Weapon*> weapons;

    // Shotgun
    WeaponData* secondary_weapon_data = new WeaponData(nullptr, 10, 1.0f);
    GameObjectData* secondary_projectile_data = new GameObjectData(sprite_, &sprite_shader_, tex_[tex_player_projectile], 0.25f);
    Weapon* secondary_player_weapon = new ShotgunWeapon(*secondary_weapon_data, *secondary_projectile_data);
    weapons.push_back(secondary_player_weapon);

    GameObjectData* hit_particle_data = new GameObjectData(hit_particles_, &hit_particles_shader_, tex_[tex_orb]);

    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), player_obj_data,
                                                 tex_[tex_invincible_ship], player_move_data,
                                                 weapons, 3, true, hit_particle_data));
    float pi_over_two = glm::pi<float>() / 2.0f;
    game_objects_[0]->SetRotation(pi_over_two);
    game_objects_[0]->SetScale(glm::vec2(1.5f, 1.5f));

    // CHANGE: Removed rotation from enemies
    // Setup other objects
    // game_objects_.push_back(new GameObject(glm::vec3(-1.0f, 1.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_green_ship], 1, true));
    // game_objects_.push_back(new GameObject(glm::vec3(1.0f, -0.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_blue_ship], 1, true));

    MoveData move_data = MoveData(0.5, game_objects_[0]);
    PatrolData patrol_data = PatrolData(1, 2, glm::vec3(-3.0f, 0.5f, 0.0f));


    //game_objects_.push_back(new EnemyGameObject(glm::vec3(-3.0f, -0.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_blue_ship], 1, move_data, patrol_data));

    // CHANGE: Setup collectible items
    /*
		game_objects_.push_back(new CollectibleGameObject(glm::vec3(2.0f, 2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
		game_objects_.push_back(new CollectibleGameObject(glm::vec3(3.0f, 1.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
		game_objects_.push_back(new CollectibleGameObject(glm::vec3(-2.0f, -2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
		game_objects_.push_back(new CollectibleGameObject(glm::vec3(1.0f, -3.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
		game_objects_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, 2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
		game_objects_.push_back(new CollectibleGameObject(glm::vec3(3.0f, -2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
		game_objects_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, -2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
		game_objects_.push_back(new CollectibleGameObject(glm::vec3(3.0f, 1.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
    */

    // Gameobject to show the vec2 scale
    GameObject *scaled = new GameObject(glm::vec3(2.0f, -1.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_invincible_ship], 10, false);
    scaled->SetScale(glm::vec2(3.0f, 1.0f));
    game_objects_.push_back(scaled);



    /*
    WeaponData* water_weapon_data = new WeaponData(game_objects_[0], 5.0f, 5);
    GameObjectData* water_projectile_data = new GameObjectData(sprite_, &sprite_shader_, tex_[tex_water_projectile], 10);
    WaterWaveWeapon* water_wave_weapon = new WaterWaveWeapon(*water_weapon_data, *water_projectile_data);
    GameObjectData water_enemy_data = GameObjectData(sprite_, &sprite_shader_, tex_[tex_blue_ship]);
    game_objects_.push_back(new EnemyGameObject(glm::vec3(-2.0f, 1.0f, 0.0f), water_enemy_data, 2, move_data, water_wave_weapon));
    game_objects_.push_back(new EnemyGameObject(glm::vec3(-5.0f, 1.0f, 0.0f), water_enemy_data, 2, move_data, new WaterWaveWeapon(*water_weapon_data, *water_projectile_data)));
     */

    GameObjectData* enemy_data = new GameObjectData(sprite_, &sprite_shader_, tex_[tex_blue_ship]);

    // Setup background
    // In this specific implementation, the background is always the
    // last object
    // CHANGE: Change shader of background to make it repeat more
    background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &background_sprite_shader_, tex_[tex_stars], 1, true);
    // CHANGE: Increase size of background
    background->SetScale(glm::vec2(12 * 9, 12 * 9));
    game_objects_.push_back(background);

    // CHANGE: Enemy spawn timer setup
    spawner_ = new Spawner(1, 3, game_objects_[0], enemy_data);
    spawner_->Start();

    SetupUI();

    game_over_timer_ = nullptr;
    game_ending_ = false;
}


void Game::DestroyGameWorld(void)
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }

    delete spawner_;
}


void Game::HandleControls(double delta_time)
{
    // Get player game object
    GameObject *player = game_objects_[0];
    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time*500.0;
    float motion_increment = 0.001*speed;
    float angle_increment = (glm::pi<float>() / 1800.0f)*speed;

    // Check for player input and make changes accordingly
    // CHANGE: Prevent player from moving when dead
    if (!game_ending_) {
        glm::vec3 velocity = glm::vec3(0, 0, 0);

        // Add acceleration based on which key is pressed
        if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
            velocity += glm::vec3(0, 1, 0);
        }
        if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
            velocity += glm::vec3(0, -1, 0);
        }
        if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
            velocity += glm::vec3(1, 0, 0);
        }
        if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
            velocity += glm::vec3(-1, 0, 0);
        }

        // Get unit vector of acceleration
        if (glm::length(velocity) > 0) {
			velocity = glm::normalize(velocity);
        }

        player->SetVelocity(velocity);

        // Get player mouse position if left mouse button is down
        // Most of this code is from assignment 4
        if(glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double mouse_x, mouse_y;

            glfwGetCursorPos(window_, &mouse_x, &mouse_y);

            float w = window_width_g;
            float h = window_height_g;
            float cursor_x_pos = 0.0;
            float cursor_y_pos = 0.0;

            if(w > h) {
                float aspect_ratio = w/h;
                cursor_x_pos = ((2.0f * mouse_x - w) * aspect_ratio) / w;
                cursor_y_pos = (-2.0f * mouse_y + h) / h;
            }
            else {
                float aspect_ratio  = h/w;
                cursor_x_pos = (2.0f * mouse_x - w) / w;
                cursor_y_pos = ((-2.0f * mouse_y + h) * aspect_ratio) / h;
            }

            ((PlayerGameObject*)player)->Shoot(glm::vec3((float)cursor_x_pos, (float)cursor_y_pos, 0), delta_time);
        }

        // Switch weapons with number keys
        if (glfwGetKey(window_, GLFW_KEY_1) == GLFW_PRESS) {
            ((PlayerGameObject*)player)->SwitchWeapons(0);
        }
        else if (glfwGetKey(window_, GLFW_KEY_2) == GLFW_PRESS) {
            ((PlayerGameObject*)player)->SwitchWeapons(1);
        }
    }

    if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
        player->SetPosition(curpos - motion_increment*player->GetRight());
    }
    if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
        player->SetPosition(curpos + motion_increment*player->GetRight());
    }



    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
}

void Game::Update(double delta_time)
{
    current_time_ += delta_time;

    // CHANGE: Check if a new enemy should be spawned
    if(!game_ending_) {
        spawner_->Update(delta_time);
        UpdateUI();

        PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);

        if(player) {
            if(player->GetWeaponCount() != 2 && spawner_->GetWaveCount() == 5) {
                GameObjectData* primary_particle_data = new GameObjectData(player_projectile_particles_, &player_particles_shader_, tex_[tex_player_projectile]);
                WeaponData* primary_weapon_data = new WeaponData(nullptr, 7, 0.5f);
                GameObjectData* primary_projectile_data = new GameObjectData(sprite_, &sprite_shader_, tex_[tex_player_projectile]);
                auto* primary_player_weapon = new DefaultPlayerWeapon(*primary_weapon_data, *primary_projectile_data, primary_particle_data);
                player->AddWeapon(primary_player_weapon);

                secondary_weapon_text_ = new TextGameObject(glm::vec3(-2.9f, -2.25f, 0.0f), sprite_, &text_shader_, tex_[tex_font]);
                secondary_weapon_text_->SetParent(game_objects_[0]);
                SpawnGameObject(secondary_weapon_text_);
                std::string str_secondary = "2: Long Ranged";
                secondary_weapon_text_->SetText(str_secondary);
                secondary_weapon_text_->SetScale(glm::vec2((float)str_secondary.length() / 4.0f, 0.25f));
            }
        }
    }

    // Check if end of game
    if(spawner_->GetWaveCount() == 12 && !game_ending_) {
        GameOver(true);
    }

    // CHANGE: Check if it is time for the game to end
    if (game_over_timer_ != nullptr && game_over_timer_->IsEnabled()) {
        if (game_over_timer_->Finished()) {
            delete(game_over_timer_);
            game_over_timer_ = nullptr;
            delete(game_objects_[0]);
            game_objects_.erase(game_objects_.begin());
            glfwSetWindowShouldClose(window_, true);
        }
    }

    // Update all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // Check if current game object should be destroyed
        if(current_game_object->ShouldDestroy()) {
            DestroyObject(i, current_game_object->ShouldExplode());

            continue;
        }

        // Update the current game object
        current_game_object->Update(delta_time);

        // Skip distance check if current game object has collisions off
        if (!current_game_object->IsCollisionOn()) continue;

        // Check for collision with other game objects
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world
        for (int j = i + 1; j < (game_objects_.size()-1); j++) {
            GameObject* other_game_object = game_objects_[j];

            // Skip distance check if one of the objects has collisions off
            if (!other_game_object->IsCollisionOn()) continue;

            // Check which type of colliders both objects are using

            // Circle-to-circle
            if(current_game_object->GetCollisionType() == CollisionType::circle_ &&
                other_game_object->GetCollisionType() == CollisionType::circle_) {

                // Compute distance between object i and object j
                float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());
                // If distance is below a threshold, we have a collision
                if (distance < (current_game_object->GetColliderRadius() + other_game_object->GetColliderRadius())) {
                    // CHANGE: Collision behaviour
                    // Check if objects can collide
                    if(!current_game_object->CanCollide(*other_game_object) || !other_game_object->CanCollide(*current_game_object))
                        continue;

                    current_game_object->OnCollision(*other_game_object);
                    other_game_object->OnCollision(*current_game_object);
                }
            }
            // Ray-to-circle
            else if((current_game_object->GetCollisionType() == CollisionType::ray_ &&
                    other_game_object->GetCollisionType() == CollisionType::circle_) ||
                    (current_game_object->GetCollisionType() == CollisionType::circle_ &&
                    other_game_object->GetCollisionType() == CollisionType::ray_)) {
                if(!current_game_object->CanCollide(*other_game_object) || !other_game_object->CanCollide(*current_game_object))
                    continue;

                // Find the circle and the ray
                GameObject* circle = nullptr;
                GameObject* ray = nullptr;
                if(current_game_object->GetCollisionType() == CollisionType::circle_) {
                    circle = current_game_object;
                    ray = other_game_object;
                }
                else {
                    circle = other_game_object;
                    ray = current_game_object;
                }

                // Calculate a, b and c for the quadratic equation
                glm::vec3 direction = ray->GetBearing();
                glm::vec3 point = ray->GetPosition();
                glm::vec3 center = circle->GetPosition();
                float a = glm::dot(direction, direction);
                float b = glm::dot(2.0f * direction, point - center);
                // Given radius of 0.5
                float c = glm::dot(point - center, point - center) - 0.25f;

                // Get the discriminant of the quadratic formula
                float discriminant = b * b - (4 * a * c);

                // If determinant is negative, no collision, else there is a collision
                if(discriminant < 0) continue;

                // Calculate t using quadratic formula
                float t1 = (-b + std::sqrt(discriminant))/2 * a;
                float t2 = (-b - std::sqrt(discriminant))/2 * a;

                if((t1 > 0 && t1 <= 1) ||
                   t2 > 0 && t2 <= 1) {
                    current_game_object->OnCollision(*other_game_object);
                    other_game_object->OnCollision(*current_game_object);
                }
            }
        }
    }

    // Update all particles
    for(int i = 0; i < particle_systems_.size(); i++) {
        if(particle_systems_[i]->ShouldDestroy()) {
            DestroyParticles(i);
            continue;
        }
        particle_systems_[i]->Update(delta_time);
    }
}

void Game::AddObjective(Objective objective) {
    switch(objective) {
        case Objective::damage_taken:
            damage_taken_++;
            break;
        case Objective::collectible_collected:
            collectibles_collected_++;
            break;
        case Objective::enemies_killed:
            enemies_killed_++;
            break;
        case Objective::waves_cleared:
            waves_cleared_++;
            break;
        default:
            break;
    }

    CalculateScore(false);
}

void Game::CalculateScore(bool outputToConsole) {
    score = damage_taken_ * -100 +
            collectibles_collected_ * 100 +
            enemies_killed_ * 10 +
            waves_cleared_ * 200;

    if(outputToConsole) {
        std::cout << "Damage Taken (" << damage_taken_ << "x-100): " << damage_taken_ * -100 << std::endl;
        std::cout << "Items Collected (" << collectibles_collected_ << "x100): " << collectibles_collected_ * 100 << std::endl;
        std::cout << "Enemies Defeated (" << enemies_killed_ << "x10): " << enemies_killed_ * 10 << std::endl;
        std::cout << "Waves Cleared (" << waves_cleared_ << "x200): " << waves_cleared_ * 200 << std::endl;
    }
}

void Game::Render(void){
    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix;
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        float aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    // Set view to zoom out, centered by default at 0,0
    float camera_zoom = 0.25f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));

    // CHANGE: Make camera move with player
    glm::mat4 transl_matrix = glm::translate(glm::mat4(1.0f), -game_objects_[0]->GetPosition());

    view_matrix = window_scale_matrix * camera_zoom_matrix * transl_matrix;

    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        if(background && game_objects_[i] == background)
        {
            continue;
        }
        game_objects_[i]->Render(view_matrix, current_time_);
    }

    if(background) {
        background->Render(view_matrix, current_time_);
    }

    for(int i = 0; i < particle_systems_.size(); i++) {
        particle_systems_[i]->Render(view_matrix, current_time_);
    }
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        HandleControls(delta_time);

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
    gamePtr = this;
}

Game* Game::GetInstance() { return gamePtr; }

void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());

    // CHANGE: Initialize background sprite shader
    background_sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/background_fragment_shader.glsl")).c_str());

    // CHANGE: Initialize ghost sprite shader
    ghost_sprite_shader_.Init((resources_directory_g+std::string("/ghost_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/ghost_fragment_shader.glsl")).c_str());

    text_shader_.Init((resources_directory_g+std::string("/ghost_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/text_fragment_shader.glsl")).c_str());

    // Initialize hit particle geometry
    Particles* hit_particles_temp = new Particles();
    hit_particles_temp->CreateGeometry(500, Particles::Shape::radial);
    hit_particles_ = hit_particles_temp;

    // Initialize hit particle shader
    hit_particles_shader_.Init((resources_directory_g+std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/particle_fragment_shader.glsl")).c_str());

    // Initialize player projectile particle geometry
    Particles* player_projectile_particles_temp = new Particles();
    player_projectile_particles_temp->CreateGeometry(1000, Particles::Shape::directional);
    player_projectile_particles_ = player_projectile_particles_temp;

    // Initialize player projectile particle shader
    player_particles_shader_.Init((resources_directory_g+std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/particle_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;
}


Game::~Game()
{
    // Free memory used by game world
    DestroyGameWorld();

    // Free rendering resources
    delete sprite_;

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
    window_width_g = width;
    window_height_g = height;
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
}


void Game::LoadTextures(std::vector<std::string> &textures)
{
    // Allocate a buffer for all texture references
    int num_textures = textures.size();
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+textures[i]).c_str());
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}

void Game::SpawnGameObject(GameObject* gameObject, int decrement) {
    // Insert game object right before the background
    if(gameObject != nullptr) {
        game_objects_.insert(game_objects_.end() - 2, gameObject);
    }
}

void Game::SpawnParticles(game::GameObject *gameObject, int decrement) {
    // Insert game object right before the background
    if(gameObject != nullptr) {
        particle_systems_.push_back(gameObject);
    }
}

void Game::DestroyObject(int index, bool shouldExplode) {
    GameObject* obj = game_objects_[index];

    if(obj->HasTag("EnemyGameObject")) {
        spawner_->OnEnemyDeath();
    }

    // Check if the parent is dark fairy queen
    if(obj->GetParent() && !obj->GetParent()->ShouldDestroy()) {
        GameObject* parent = obj->GetParent();
        if(parent->HasTag("DarkFairyQueen")) {
            ((DarkFairyQueen*)parent)->RemoveSpawnedObject(obj);
        }
    }

    // Check if the object that is exploding is the player
    // If it is, then start a timer to end the game
    if (index == 0) {
        GameOver(false);
    }

    if (shouldExplode) {
        // Spawn an explosion at the position of obj with duration 5
        GameObject* explosion = new ExplosionGameObject(obj->GetPosition(), sprite_, &sprite_shader_, tex_[tex_explosion], 5);
        game_objects_[index] = explosion;
    }
    else {
        game_objects_[index] = nullptr;
        game_objects_.erase(game_objects_.begin() + index);
    }

    // Destroy obj
    delete(obj);

}

void Game::DestroyParticles(int index) {
    GameObject* obj = particle_systems_[index];
    particle_systems_.erase(particle_systems_.begin() + index);
    delete(obj);
}

void Game::SetupUI() {
    health_text_ = new TextGameObject(glm::vec3(-3.5f, -3.25f, 0.0f), sprite_, &text_shader_, tex_[tex_font]);
    health_text_->SetParent(game_objects_[0]);
    SpawnGameObject(health_text_);

    wave_text_= new TextGameObject(glm::vec3(-3.8f, 3.25f, 0.0f), sprite_, &text_shader_, tex_[tex_font]);
    wave_text_->SetParent(game_objects_[0]);
    SpawnGameObject(wave_text_);

    enemies_left_text_ = new TextGameObject(glm::vec3(-2.8f, 2.75f, 0.0f), sprite_, &text_shader_, tex_[tex_font]);
    enemies_left_text_->SetParent(game_objects_[0]);
    SpawnGameObject(enemies_left_text_);

    primary_weapon_text_ = new TextGameObject(glm::vec3(-3.4f, -2.75f, 0.0f), sprite_, &text_shader_, tex_[tex_font]);
    primary_weapon_text_->SetParent(game_objects_[0]);
    SpawnGameObject(primary_weapon_text_);
    std::string str_primary = "1: Shotgun";
    primary_weapon_text_->SetText(str_primary);
    primary_weapon_text_->SetScale(glm::vec2((float)str_primary.length() / 4.0f, 0.25f));

    score_text_ = new TextGameObject(glm::vec3(-3.45f, 2.25f, 0.0f), sprite_, &text_shader_, tex_[tex_font]);
    score_text_->SetParent(game_objects_[0]);
    SpawnGameObject(score_text_);
}

void Game::UpdateUI() {
    if(!health_text_->GetParent()) return;

    PlayerGameObject* player = (PlayerGameObject*)health_text_->GetParent();

    std::string str_health = "Health: " + std::to_string(player->GetHealth());
    health_text_->SetText(str_health);
    health_text_->SetScale(glm::vec2((float)str_health.length() / 4.0f, 0.25f));

    if(spawner_) {
        std::string str_wave = "Wave: " + std::to_string(spawner_->GetWaveCount());
        wave_text_->SetText(str_wave);
        wave_text_->SetScale(glm::vec2((float)str_wave.length() / 4.0f, 0.25f));

        std::string str_enemies_left = "Enemies Left: " + std::to_string(spawner_->GetRemainingEnemies());
        enemies_left_text_->SetText(str_enemies_left);
        enemies_left_text_->SetScale(glm::vec2((float)str_enemies_left.length() / 4.0f, 0.25f));
    }

    std::string str_score = "Score: " + std::to_string(score);
    score_text_->SetText(str_score);
    score_text_->SetScale(glm::vec2((float)str_score.length() / 4.0f, 0.25f));
}

void Game::GameOver(bool won) {
    // Calculate and output end score
    game_over_timer_ = new Timer();
    game_over_timer_->Start(GAME_OVER_TIME);
    game_ending_ = true;

    // If won, calculate time
    if(won) {
        std::cout << "Victory!" << std::endl;
        int time_bonus = -(glfwGetTime() - start_time_) + 2000;

        if(time_bonus < 0) time_bonus = 0;
        score += time_bonus;

        std::cout << "Time Bonus: " << time_bonus << std::endl;
    }
    else {
        std::cout << "Game Over!" << std::endl;
    }

    CalculateScore(true);

    std::cout << "Total Score: " << score << std::endl;
}

} // namespace game
