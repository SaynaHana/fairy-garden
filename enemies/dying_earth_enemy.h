#ifndef DYING_EARTH_ENEMY_H
#define DYING_EARTH_ENEMY_H

#include "enemy_game_object.h"

namespace game {
    class DyingEarthEnemy : public EnemyGameObject {
        public:
            DyingEarthEnemy(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon);
            DyingEarthEnemy(const glm::vec3& position, GameObjectData& data, int health, MoveData& move_data, Weapon* weapon);

            void Update(double delta_time) override;

            void Move(double delta_time) override;

            void OnCollision(GameObject& other) override;

        private:
            void SetupLinks();

            float swing_angle_;
            bool reverse_swing_; // If enemy should swing in opposite direction
            float swing_speed_ = 2.0f;
            float flee_speed_;

            bool HasChildren();

            // Magic missiles
            float missile_interval_;
            Timer* missile_timer_;
    };
}

#endif
