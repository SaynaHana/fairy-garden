#include <iostream>
#include "magic_missile_projectile.h"

namespace game {
	MagicMissileProjectile::MagicMissileProjectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, float lifetime)
	: Projectile(position, geom, shader, texture, lifetime) {
		tags.insert("CanDamagePlayer");
        tags.erase("CanDamageEnemy");
	}

    MagicMissileProjectile::MagicMissileProjectile(const glm::vec3 &position, game::GameObjectData &data, float lifetime)
    : MagicMissileProjectile(position, data.geom_, data.shader_, data.texture_, lifetime) {

    }

	void MagicMissileProjectile::Move(double delta_time) {
		if (target_ == nullptr) return;

		// Move towards target
		glm::vec3 diff = target_->GetPosition() - this->GetPosition();

		if (glm::length(diff) > 0) {
			diff = glm::normalize(diff);
		}

        SetAcceleration(diff * speed_);

        Projectile::Move(delta_time);
	}

}