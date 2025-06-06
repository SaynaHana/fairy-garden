#include <glm/gtx/string_cast.hpp>
#include "player_game_object.h"

#include "collectible_game_object.h"
#include "projectile_game_object.h"
#include "ui/text_game_object.h"
#include "weapons/interval_weapon.h"
#include "game.h"
#include "particles/particle_system.h"

namespace game {
	/*
		PlayerGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture,
                                       GLuint invincibleTexture, MoveData& move_data, std::vector<Weapon*> weapons,
                                       int health, bool collision_on, float collider_radius,
                                       GameObjectData* hit_particle_data)
		: GameObject(position, geom, shader, texture, health, collision_on, collider_radius, move_data.GetSpeed()) {
		damage_ = 1;
		invincible_ = false;
		invincible_timer_ = new Timer();
        i_frame_duration_ = 1.0f;
		normal_texture_ = texture;
		invincible_texture_ = invincibleTexture;
		projectile_timer_ = nullptr;
		can_shoot_ = true;
        speed_ = move_data.GetSpeed();
        tags.insert("PlayerGameObject");
        tags.insert("CanDamageEnemy");
		weapons_ = weapons;
		weapon_ = nullptr;
		SwitchWeapons(0);
        collectible_timer_ = new Timer();
        collectible_active_ = false;
        init_speed_ = move_data.GetSpeed();
        init_collider_radius_ = collider_radius;
        map_boundaries_ = glm::vec2(47, 47);
        hit_particle_data_ = hit_particle_data;
	}

    PlayerGameObject::PlayerGameObject(const glm::vec3 &position, GameObjectData &obj_data,
                                       GLuint invincible_texture, MoveData &move_data, std::vector<Weapon*> weapons, int health,
                                       bool collision_on, GameObjectData* hit_particle_data)
                                       : PlayerGameObject(position, obj_data.geom_, obj_data.shader_, obj_data.texture_,
                                                          invincible_texture, move_data, weapons, health, collision_on, obj_data.collider_radius_,
                                                          hit_particle_data) {

    }

	// Update function for moving the player object around
	void PlayerGameObject::Update(double delta_time) {

		// Special player updates go here
		if (invincible_timer_ != nullptr) {
			if (invincible_timer_->Finished()) {
                if(invincible_ && !collectible_active_) {
                    SetInvincible(false);
                }
			}
		}

		if (projectile_timer_ != nullptr) {
			if (projectile_timer_->Finished()) {
				SetCanShoot(true);
			}
		}

        if(collectible_active_) {
            if(collectible_timer_->Finished()) {
                ResetStats();
            }
        }

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

    void PlayerGameObject::Move(double delta_time) {
        glm::vec3 pos =  velocity_ * speed_ * (float)delta_time + GetPosition();

        // Make sure the player doesn't go out of bounds
        if(pos.x >= map_boundaries_.x) return;
        if(pos.x <= -map_boundaries_.x) return;
        if(pos.y >= map_boundaries_.y) return;
        if(pos.y <= -map_boundaries_.y) return;

        SetPosition(pos);
    }

	void PlayerGameObject::OnCollision(GameObject &other) {
        // If the object can damage the player and the player is not invincible, show damage particle effect
        if(!invincible_ && other.HasTag("CanDamagePlayer")) {
            // Spawn particle effect
            if(hit_particle_data_) {
                GameObject* hit_particle_system = new ParticleSystem(GetPosition(), hit_particle_data_->geom_,
                                                                     hit_particle_data_->shader_, hit_particle_data_->texture_,
                                                                     nullptr, glm::vec3(1, 0.2f, 0), 1.0f, true);
                hit_particle_system->SetScale(glm::vec2(0.1));
                Game::GetInstance()->SpawnParticles(hit_particle_system);
            }
        }

        if(invincible_) {
            health_++;
        }
        else {
            Game::GetInstance()->AddObjective(Game::Objective::damage_taken);
        }

        if(other.HasTag("Collectible")) {
            CollectibleGameObject* collectible = dynamic_cast<CollectibleGameObject*>(&other);

            if(collectible) {
                UseCollectible(collectible);
            }
        }
        else {
            if(!invincible_) {
                SetInvincible(true);
                invincible_timer_->Start(i_frame_duration_);
            }
        }


        GameObject::OnCollision(other);

	}

    void PlayerGameObject::Shoot(const glm::vec3& mouse_pos, double delta_time) {
        if(weapon_ == nullptr) return;

        // Get direction from mouse
        glm::vec3 diff = mouse_pos;

        if(glm::length(diff) != 0) {
            diff = glm::normalize(diff);
        }

        IntervalWeapon* interval = dynamic_cast<IntervalWeapon*>(weapon_);

        if(interval) {
            interval->SetAttackSpeedMultiplier(attack_speed_multiplier);
        }

        weapon_->ExecuteAttack(delta_time, position_, diff);
	}

	bool PlayerGameObject::SwitchWeapons(int index) {
		if (index < 0 || index >= weapons_.size()) return false;

		weapon_ = weapons_[index];
        return true;
	}

	void PlayerGameObject::SetInvincible(bool invincible) {
		invincible_ = invincible;
	}

    void PlayerGameObject::UseCollectible(game::CollectibleGameObject *collectible) {
        if(!collectible) return;

        collectible->OnConsumed(*this);
        collectible_timer_->Start(collectible->GetDuration());
        collectible_active_ = true;
    }

    void PlayerGameObject::ResetStats() {
        attack_speed_multiplier = 1;
        SetInvincible(false);
        speed_ = init_speed_;
        collider_radius_ = init_collider_radius_;
        collectible_active_ = false;
    }

    bool PlayerGameObject::CanCollide(game::GameObject &other) {
		if (!GameObject::CanCollide(other)) return false;
		if (other.HasTag("CanDamagePlayer")) return true;
		if (other.HasTag("Collectible")) return true;

        return false;
    }

	void PlayerGameObject::SetCanShoot(bool can_shoot) {
		can_shoot_ = can_shoot;

		if (!can_shoot) {
			projectile_timer_ = new Timer();
			projectile_timer_->Start(SHOOT_COOLDOWN / attack_speed_multiplier);
		}
		else {
			projectile_timer_ = nullptr;
		}
	}

} // namespace game
