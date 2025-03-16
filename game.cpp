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

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "COMP2501 Final Project - Aria Wong";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
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

    // CHANGE: Moved textures enum to header file so I can use it outside of this function

    // CHANGE: changed the textures of the destroyers to smiley, neutral and frowny
    textures.push_back("/textures/smiley.png"); 
    textures.push_back("/textures/neutral.png"); 
    textures.push_back("/textures/frowny.png");
    textures.push_back("/textures/stars.png");
    textures.push_back("/textures/orb.png");
    textures.push_back("/textures/explosion.png");
    textures.push_back("/textures/coin.png");
    textures.push_back("/textures/invincible_smiley.png");
    textures.push_back("/textures/bullet.png");
    textures.push_back("/textures/tex_enemy_projectile.png");
    // Load textures
    LoadTextures(textures);


    // **** Setup all the game objects in the world

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector
    MoveData player_move_data = MoveData(2, nullptr);
    GameObjectData player_obj_data = GameObjectData(sprite_, &sprite_shader_, tex_[tex_red_ship]);

    // Create player weapon
    WeaponData* primary_weapon_data = new WeaponData(nullptr, 10, 0.25f);
    GameObjectData* primary_projectile_data = new GameObjectData(sprite_, &sprite_shader_, tex_[tex_enemy_projectile]);
    DefaultPlayerWeapon* primary_player_weapon = new DefaultPlayerWeapon(*primary_weapon_data, *primary_projectile_data);
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), player_obj_data,
                                                 tex_[tex_invincible_ship], player_move_data,
                                                 primary_player_weapon, 3, true));
    float pi_over_two = glm::pi<float>() / 2.0f;
    game_objects_[0]->SetRotation(pi_over_two);

    // CHANGE: Removed rotation from enemies
    // Setup other objects
    // game_objects_.push_back(new GameObject(glm::vec3(-1.0f, 1.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_green_ship], 1, true));
    // game_objects_.push_back(new GameObject(glm::vec3(1.0f, -0.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_blue_ship], 1, true));

    MoveData move_data = MoveData(0.5, game_objects_[0]);
    PatrolData patrol_data = PatrolData(1, 2, glm::vec3(-3.0f, 0.5f, 0.0f));


    //game_objects_.push_back(new EnemyGameObject(glm::vec3(-3.0f, -0.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_blue_ship], 1, move_data, patrol_data));

    // CHANGE: Setup collectible items
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(2.0f, 2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(3.0f, 1.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(-2.0f, -2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(1.0f, -3.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, 2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(3.0f, -2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(-3.0f, -2.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(3.0f, 1.0f, 0.0f), sprite_, &ghost_sprite_shader_, tex_[tex_coin]));

    // Gameobject to show the vec2 scale
    GameObject *scaled = new GameObject(glm::vec3(2.0f, -1.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_invincible_ship], 1, false);
    scaled->SetScale(glm::vec2(3.0f, 1.0f));
    game_objects_.push_back(scaled);

    WeaponData* weapon_data = new WeaponData(game_objects_[0], 2, 1.75f);
    GameObjectData* magic_missile_data = new GameObjectData(sprite_, &sprite_shader_, tex_[tex_enemy_projectile], 5);
    MagicMissileWeapon* magic_missile_weapon = new MagicMissileWeapon(*weapon_data, *magic_missile_data);
    GameObjectData enemy_data = GameObjectData(sprite_, &sprite_shader_, tex_[tex_green_ship]);
    game_objects_.push_back(new EnemyGameObject(glm::vec3(2.0, 1.0, 0.0), enemy_data, 2, move_data, magic_missile_weapon));

    WeaponData* water_weapon_data = new WeaponData(game_objects_[0], 5.0f, 5);
    GameObjectData* water_projectile_data = new GameObjectData(sprite_, &sprite_shader_, tex_[tex_enemy_projectile], 10);
    WaterWaveWeapon* water_wave_weapon = new WaterWaveWeapon(*water_weapon_data, *water_projectile_data);
    GameObjectData water_enemy_data = GameObjectData(sprite_, &sprite_shader_, tex_[tex_blue_ship]);
    game_objects_.push_back(new EnemyGameObject(glm::vec3(-2.0f, 1.0f, 0.0f), water_enemy_data, 2, move_data, water_wave_weapon));


    // Setup background
    // In this specific implementation, the background is always the
    // last object
    // CHANGE: Change shader of background to make it repeat more
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &background_sprite_shader_, tex_[tex_stars], 1, true);
    // CHANGE: Increase size of background
    background->SetScale(glm::vec2(12 * 9, 12 * 9));
    game_objects_.push_back(background);


    // CHANGE: Enemy spawn timer setup
    spawn_timer_ = new Timer();
    spawn_timer_->Start(ENEMY_SPAWN_TIME);

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

    // Free memory for timer
    delete spawn_timer_;
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
        if(glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double mouse_x, mouse_y;

            glfwGetCursorPos(window_, &mouse_x, &mouse_y);
            ((PlayerGameObject*)player)->Shoot(glm::vec3((float)mouse_x, (float)mouse_y, 0), delta_time);
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
    // CHANGE: Check if a new enemy should be spawned
    if (spawn_timer_->Finished()) {
        // Spawn new enemy
        // Get random position
        float random_x = ((double)rand() / RAND_MAX * 6.0 * 3) - 9.0;
        float random_y = ((double)rand() / RAND_MAX * 6.0 * 3) - 9.0;

        MoveData move_data = MoveData(2, game_objects_[0]);
        PatrolData patrol_data = PatrolData(2, 1, glm::vec3(random_x, random_y, 0));

        /*
        game_objects_.insert(game_objects_.end() - 1, 
							new EnemyGameObject(glm::vec3(random_x, random_y, 0), sprite_, &sprite_shader_, tex_[2], 1, move_data));
         */

        spawn_timer_->Start(ENEMY_SPAWN_TIME);
    }

    // CHANGE: Check if it is time for the game to end
    if (game_over_timer_ != nullptr && game_over_timer_->IsEnabled()) {
        if (game_over_timer_->Finished()) {
            delete(game_over_timer_);
            game_over_timer_ = nullptr;
            std::cout << "Game over!" << std::endl;
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
                if (distance < 1.0f) {
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
        game_objects_[i]->Render(view_matrix, current_time_);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

void Game::SpawnGameObject(GameObject* gameObject) {
    // Insert game object right before the background
    if(gameObject != nullptr) {
        game_objects_.insert(game_objects_.end() - 2, gameObject);
    }
}

void Game::DestroyObject(int index, bool shouldExplode) {
    GameObject* obj = game_objects_[index];

    // Check if the object that is exploding is the player
    // If it is, then start a timer to end the game
    if (index == 0) {
        game_over_timer_ = new Timer();
        game_over_timer_->Start(GAME_OVER_TIME);
        game_ending_ = true;
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

} // namespace game
