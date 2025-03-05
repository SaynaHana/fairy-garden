#include "magic_missile_projectile.h"

namespace game {
	MagicMissileProjectile::MagicMissileProjectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, float lifetime = 2.0f) 
	: Projectile(position, geom, shader, texture, lifetime) {
		tags.insert("CanDamagePlayer");
	}

	void MagicMissileProjectile::Move(double delta_time) {
		if (target_ == nullptr) return;

		// Move towards target
		glm::vec3 diff = target_->GetPosition() - this->GetPosition();

		if (glm::length(diff) != 0) {
			diff = glm::normalize(diff);
		}

		SetVelocity(diff * speed_);
	}

}