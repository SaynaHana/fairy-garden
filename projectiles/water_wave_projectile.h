#ifndef WATER_WAVE_PROJECTILE_H
#define WATER_WAVE_PROJECTILE_H

#include "../projectile_game_object.h"

namespace game {
    class WaterWaveProjectile : public Projectile {
    public:
        WaterWaveProjectile(const glm::vec3 position, GameObjectData& data, float lifetime = 5.0f, const glm::vec3& parent_bearing = glm::vec3(0, 0, 0),
                            float offset = 0.0f);

        void Move(double delta_time) override;

        void SetTarget(GameObject* target) { target_ = target; }

        bool CanCollide(GameObject& other) override;

    private:
        glm::vec3 parent_bearing_;
        bool init_diff_set_;
        glm::vec3 diff_;
        float t_;
        glm::vec3 init_position_; // Where does the projectile start?
        GameObject* target_;
    };
}

#endif
