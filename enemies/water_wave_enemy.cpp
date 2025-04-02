#include "water_wave_enemy.h"

namespace game {
    WaterWaveEnemy::WaterWaveEnemy(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                     GLuint texture, int health, game::MoveData &move_data, game::Weapon *weapon)
                                     : EnemyGameObject(position, geom, shader, texture, health, move_data, weapon) {
        t_ = 0;
        circle_radius_ = 1.0f;
    }

    WaterWaveEnemy::WaterWaveEnemy(const glm::vec3 &position, game::GameObjectData &data, int health,
                                     game::MoveData &move_data, game::Weapon *weapon)
                                     : WaterWaveEnemy(position, data.geom_, data.shader_, data.texture_, health, move_data, weapon) {

    }

    void WaterWaveEnemy::Move(double delta_time) {
        if (target_ == nullptr) return;

        // Move the enemy in a circle and towards the player at the same time
        // Get direction to player
        glm::vec3 diff = target_->GetPosition() - GetPosition();

        if (glm::length(diff) != 0) {
            diff = glm::normalize(diff);
        }

        glm::vec3 pos = glm::vec3(circle_radius_ * std::cos(t_), circle_radius_ * std::sin(t_), 0);
        t_ += (float)delta_time * speed_;

        SetPosition(position_ + diff * (float)delta_time + (pos * (float)delta_time));
    }
}