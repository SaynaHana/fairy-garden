#include <iostream>
#include "magic_missile_projectile.h"

namespace game {
	MagicMissileProjectile::MagicMissileProjectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, float lifetime)
	: Projectile(position, geom, shader, texture, lifetime) {
		tags.insert("CanDamagePlayer");
        tags.erase("CanDamageEnemy");
        collision_type_ = CollisionType::circle_;
        damage_ = 1;
	}

    MagicMissileProjectile::MagicMissileProjectile(const glm::vec3 &position, game::GameObjectData &data, float lifetime)
    : MagicMissileProjectile(position, data.geom_, data.shader_, data.texture_, lifetime) {

    }

    bool MagicMissileProjectile::CanCollide(game::GameObject &other) {
        if (!Projectile::CanCollide(other)) return false;
        if (other.HasTag("PlayerGameObject")) return true;

        return false;
    }

	void MagicMissileProjectile::Move(double delta_time) {
		if (target_ == nullptr) return;

		// Move towards target
		glm::vec3 diff = target_->GetPosition() - this->GetPosition();

		if (glm::length(diff) > 0) {
			diff = glm::normalize(diff);
		}

        // Get angle of rotation using arctan
        float angle = (float)atan(diff.y / diff.x);
        angle += 1/2 * 3.14f;

        // Change the angle
        SetRotation(angle);

        SetAcceleration(diff * speed_);

        Projectile::Move(delta_time);
	}

}