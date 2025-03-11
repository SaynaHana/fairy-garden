#include "player_game_object.h"

#include "collectible_game_object.h"
#include "projectile_game_object.h"

namespace game {
	/*
		PlayerGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GLuint invincibleTexture, MoveData& move_data, int health, bool collision_on)
		: GameObject(position, geom, shader, texture, health, collision_on) {
		damage_ = 1;
		itemCount_ = 0;
		invincible_ = false;
		invincible_timer_ = nullptr;
		normal_texture_ = texture;
		invincible_texture_ = invincibleTexture;
		projectile_timer_ = nullptr;
		can_shoot_ = true;
        speed_ = move_data.GetSpeed();
        tags.insert("PlayerGameObject");
	}

    PlayerGameObject::PlayerGameObject(const glm::vec3 &position, game::GameObjectData &obj_data,
                                       GLuint invincible_texture, game::MoveData &move_data, int health,
                                       bool collision_on)
                                       : PlayerGameObject(position, obj_data.geom_, obj_data.shader_, obj_data.texture_,
                                                          invincible_texture, move_data, health, collision_on) {

    }

	// Update function for moving the player object around
	void PlayerGameObject::Update(double delta_time) {

		// Special player updates go here
		if (invincible_timer_ != nullptr) {
			if (invincible_timer_->Finished()) {
				SetInvincible(false);
			}
		}

		if (projectile_timer_ != nullptr) {
			if (projectile_timer_->Finished()) {
				SetCanShoot(true);
			}
		}

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

    void PlayerGameObject::Move(double delta_time) {
        SetPosition(velocity_ * speed_ * (float)delta_time + GetPosition());
    }

	void PlayerGameObject::OnCollision(GameObject &other) {
		// If the other game object is a collectible
		CollectibleGameObject* collectible = dynamic_cast<CollectibleGameObject*>(&other);

		if (collectible != nullptr) {
			// Means that other is a collectible
			itemCount_++;

			if (itemCount_ >= INVINCIBLE_ITEM_COUNT) {
				itemCount_ = 0;
				SetInvincible(true);
			}
		}

		// If invincible, negate damage
		if (invincible_) {
			health_ += other.GetDamage();
		}

		GameObject::OnCollision(other);
	}

    GameObject* PlayerGameObject::Shoot(glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) {
        GameObject* projectile = nullptr;
		if (can_shoot_) {
            // Spawn projectile
            projectile = new Projectile(position, geom, shader, texture, 10);
            projectile->SetRotation(this->GetRotation());
            projectile->SetAcceleration(this->GetBearing() * 10.0f);

			SetCanShoot(false);
		}

        return projectile;
	}

	void PlayerGameObject::SetInvincible(bool invincible) {
		invincible_ = invincible;

		if (invincible) {
			invincible_timer_ = new Timer();
			invincible_timer_->Start(INVINCIBLE_DURATION);

			// Change texture to invincible texture
			texture_ = invincible_texture_;
		}
		else {
			invincible_timer_ = nullptr;

			// Change texture to normal texture
			texture_ = normal_texture_;
		}
	}

    bool PlayerGameObject::CanCollide(game::GameObject &other) {
        if(other.GetTags().find("CanDamagePlayer") != other.GetTags().end()) return true;
        if(other.GetTags().find("Collectible") != other.GetTags().end()) return true;

        return false;
    }

	void PlayerGameObject::SetCanShoot(bool can_shoot) {
		can_shoot_ = can_shoot;

		if (!can_shoot) {
			projectile_timer_ = new Timer();
			projectile_timer_->Start(SHOOT_COOLDOWN);
		}
		else {
			projectile_timer_ = nullptr;
		}
	}


} // namespace game
