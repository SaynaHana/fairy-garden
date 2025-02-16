#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "timer.h"

#define INVINCIBLE_ITEM_COUNT 5
#define INVINCIBLE_DURATION 10


namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint invincible_texture, int health, bool collision_on);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            void OnCollision(GameObject &other) override;

        private:
            // CHANGE: Collectible items
            int itemCount_;

            // CHANGE: Makes the player not take damage for 10 seconds
            void SetInvincible(bool invincible);
            bool invincible_;
            Timer* invincible_timer_;
            GLuint normal_texture_;
            GLuint invincible_texture_;


    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
