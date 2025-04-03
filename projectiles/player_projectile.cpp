#include "player_projectile.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace game {
    PlayerProjectile::PlayerProjectile(const glm::vec3 &position, GameObjectData &data, float lifetime, const glm::vec3& direction)
        : Projectile(position, data, lifetime) {
        direction_ = direction;
    }

    void PlayerProjectile::Move(double delta_time) {
        // Move in direction
        SetPosition(position_ + direction_ * (float)delta_time * speed_);
    }

    bool PlayerProjectile::CanCollide(GameObject& other) {
        if (!Projectile::CanCollide(other)) return false;
        if (other.HasTag("EnemyGameObject")) return true;

        return false;
    }

    void PlayerProjectile::OnCollision(game::GameObject &other) {
        for(int i = 0; i < children_.size(); i++) {
            children_[i]->SetShouldDestroy(true);
            children_[i]->SetScale(glm::vec2(0));
        }

        Projectile::OnCollision(other);
    }
}
