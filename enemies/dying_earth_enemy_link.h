#ifndef DYING_EARTH_ENEMY_LINK_H
#define DYING_EARTH_ENEMY_LINK_H

#include "enemy_game_object.h"

namespace game {
    class DyingEarthEnemyLink : public GameObject {
        public:
            DyingEarthEnemyLink(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon);
            DyingEarthEnemyLink(const glm::vec3& position, GameObjectData& data, int health, MoveData& move_data, Weapon* weapon);

            void Move(const glm::mat4& parent_transformation_);

            bool CanCollide(GameObject& other) override;
            void OnCollision(GameObject& other) override;

            void Move(double delta_time) override {}

            void DestroyChildren();
    };
}


#endif
