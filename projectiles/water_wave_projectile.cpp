#include "water_wave_projectile.h"

#include <cmath>

namespace game {
    WaterWaveProjectile::WaterWaveProjectile(const glm::vec3 position, game::GameObjectData &data, float lifetime, const glm::vec3& parent_bearing,
                                             float offset)
    : Projectile(position, data, lifetime), parent_bearing_(parent_bearing) {
        t_ = offset;
        init_position_ = position;
        init_diff_set_ = false;
        diff_ = glm::vec3(0, 0, 0);
        tags.insert("CanDamagePlayer");
        tags.erase("CanDamageEnemy");
    }

    void WaterWaveProjectile::Move(double delta_time) {
        if(target_ == nullptr) return;

        if(!init_diff_set_) {
            diff_ = target_->GetPosition() - position_;
            init_diff_set_ = true;
        }

        if(glm::length(diff_) > 0) {
            diff_ = glm::normalize(diff_);
        }

        glm::vec3 pos = glm::vec3(std::cos(t_), std::sin(t_), 0);
        t_ += (float)delta_time * speed_;

        SetPosition(position_ + diff_ * (float)delta_time + (pos * (float)delta_time * 3.0f));
    }

}