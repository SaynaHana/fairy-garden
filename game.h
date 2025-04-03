#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.h"
#include "game_object.h"
#include "Constants.h"
#include "enemies/spawner.h"
#include "ui/text_game_object.h"
#include "particles/particles.h"

#define ENEMY_SPAWN_TIME 10
#define GAME_OVER_TIME 5

namespace game {

    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game world (scene, game objects, etc.)
            void SetupGameWorld(void);

            // Destroy the game world
            void DestroyGameWorld(void);

            // Run the game (keep the game active)
            void MainLoop(void);

            // Spawns a game object
            void SpawnGameObject(GameObject* gameObject, int decrement = 2);
            void SpawnParticles(GameObject* gameObject, int decrement = 2);

            // Singleton from: https://www.geeksforgeeks.org/implementation-of-singleton-class-in-cpp/
            static Game* GetInstance();

            GLuint getTexture(int index) {
                return tex_[index];
            }

            enum {
                tex_red_ship = 0,
                tex_green_ship = 1,
                tex_blue_ship = 2,
                tex_stars = 3,
                tex_orb = 4,
                tex_explosion = 5,
                tex_coin = 6,
                tex_invincible_ship = 7,
                tex_projectile = 8,
                tex_enemy_projectile = 9,
                tex_player_projectile = 10,
                tex_water_projectile = 11,
                tex_fairy_dust = 12,
                tex_rainbow_fairy_dust = 13,
                tex_dark_fairy_dust = 14,
                tex_font = 15
            };

            enum Objective {
                damage_taken = 0,
                collectible_collected = 1,
                enemies_killed = 2,
                waves_cleared = 3
            };

            void AddObjective(Objective obj);

        private:
            // Singleton from: https://www.geeksforgeeks.org/implementation-of-singleton-class-in-cpp/
            static Game* gamePtr;

            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Sprite geometry
            Geometry *sprite_;

            // Shader for rendering sprites in the scene
            Shader sprite_shader_;

            // CHANGE: Sprite for background
            Shader background_sprite_shader_;

            // CHANGE: Sprite shader for player which has ghost mode
            Shader ghost_sprite_shader_;

            Shader text_shader_;

            // References to textures
            // This needs to be a pointer
            GLuint *tex_;

            // Objectives and scoring
            int damage_taken_;
            int collectibles_collected_;
            int enemies_killed_;
            int waves_cleared_;
            int score;

            void CalculateScore(bool outputToConsole);

            // List of game objects
            std::vector<GameObject*> game_objects_;
            std::vector<GameObject*> particle_systems_;

            // Store background to render it last
            GameObject* background;

            // Keep track of time
            double current_time_;

            // CHANGE: Enemy spawning
            Spawner* spawner_;

            // CHANGE: Game over
            Timer* game_over_timer_;
            bool game_ending_;

            double start_time_; // When the player started

            glm::mat4 view_matrix;

            // Particles
            Particles* hit_particles_;

            Shader hit_particles_shader_;

            // Handle user input
            void HandleControls(double delta_time);

            // Update all the game objects
            void Update(double delta_time);
 
            // Render the game world
            void Render(void);

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname);

            // Load all textures
            void LoadTextures(std::vector<std::string> &textures);

            // CHANGE: If the game object is flagged to explode, then it will spawn an explosion object
            // on top of the game object and the game object will disappear. If not, the game object will only
            // disappear
            void DestroyObject(int index, bool shouldExplode);
            void DestroyParticles(int index);

            // User interface
            void SetupUI();
            void UpdateUI();
            TextGameObject* health_text_;
            TextGameObject* wave_text_;
            TextGameObject* enemies_left_text_;
            TextGameObject* primary_weapon_text_;
            TextGameObject* secondary_weapon_text_;
            TextGameObject* score_text_;

            void GameOver(bool won);
    }; // class Game

} // namespace game

#endif // GAME_H_
