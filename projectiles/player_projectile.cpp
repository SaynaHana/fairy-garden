#include "player_projectile.h"

namespace game {
    PlayerProjectile::PlayerProjectile(const glm::vec3 &position, GameObjectData &data, float lifetime, const glm::vec3& direction)
        : Projectile(position, data, lifetime) {
        if(glm::length(direction_) != 0) {
            direction_ = glm::normalize(direction);
        }
    }

    void PlayerProjectile::Move(double delta_time) {
        // Move in direction
        SetPosition(position_ + direction_ * (float)delta_time * speed_);
    }

    bool PlayerProjectile::CanCollide(GameObject& other) {
        if (!Projectile::CanCollide(other)) return false;
        if (other.GetTags().find("EnemyGameObject") != other.GetTags().end()) return false;
        return true;
    }
}
