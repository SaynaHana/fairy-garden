#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "timer.h"
#include "data/move_data.h"

#define INVINCIBLE_ITEM_COUNT 5
#define INVINCIBLE_DURATION 10
#define SHOOT_COOLDOWN 0.4


namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint invincible_texture, MoveData& moveData, int health, bool collision_on);
            PlayerGameObject(const glm::vec3 &position, GameObjectData& objData, GLuint invincible_texture, MoveData& moveData, int health, bool collision_on);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            void OnCollision(GameObject &other) override;

            bool CanCollide(GameObject& other) override;

            GameObject* Shoot(glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);


        private:
            // CHANGE: Collectible items
            int itemCount_;

            // CHANGE: Makes the player not take damage for 10 seconds
            void SetInvincible(bool invincible);
            bool invincible_;
            Timer* invincible_timer_;
            GLuint normal_texture_;
            GLuint invincible_texture_;

            // CHANGE: Projectiles
            void SetCanShoot(bool can_shoot);
            Timer* projectile_timer_;
            bool can_shoot_;

            void Move(double delta_time) override;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
