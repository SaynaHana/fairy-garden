#ifndef DYING_EARTH_ENEMY_H
#define DYING_EARTH_ENEMY_H

#include "enemy_game_object.h"

namespace game {
    class DyingEarthEnemy : public EnemyGameObject {
        public:
            DyingEarthEnemy(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon);
            DyingEarthEnemy(const glm::vec3& position, GameObjectData& data, int health, MoveData& move_data, Weapon* weapon);

            void Update(double delta_time) override;

        private:
            void SetupLinks();
    };
}

#endif
