#include "enemy_game_object.h"
#include "../player_game_object.h"
#include "../projectile_game_object.h"
#include "../game.h"
#include "../collectibles/dark_fairy_dust_collectible.h"

namespace game {
	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon)
	: GameObject(position, geom, shader, texture, health, true) {
        weapon_ = weapon;

        if(weapon_) {
        }

		state_ = state_chase;
        idle_range_ = 2;
        chase_range_ = 3;
        flee_range_ = 2;
		target_ = move_data.GetTarget();
        speed_ = move_data.GetSpeed();
        tags.insert("EnemyGameObject");
        tags.insert("CanDamagePlayer");

        iframe_timer_ = new Timer();
        iframe_duration_ = 0.25f;
        is_invincible_ = false;

        collectible_drop_chance_ = 5;
	}

    EnemyGameObject::EnemyGameObject(const glm::vec3 &position, GameObjectData &data, int health, MoveData &move_data, Weapon* weapon)
    : EnemyGameObject(position, data.geom_, data.shader_, data.texture_, health, move_data, weapon) {

    }

    EnemyGameObject::~EnemyGameObject() {
        if (weapon_ != nullptr) {
            delete weapon_;
        }
    }

	void EnemyGameObject::Update(double delta_time) {
        ChangeEnemyRotation();
        CheckIFrames();
		Detect();

        if(state_ == EnemyMoveState::state_chase) {
            Chase();
        }
        else if(state_ == EnemyMoveState::state_flee) {
            Flee();
        }

        // Weapon
        if(weapon_ != nullptr) {
            weapon_->ExecuteAttack(delta_time, GetPosition(), GetBearing());
        }

		GameObject::Update(delta_time);
	}

    void EnemyGameObject::ChangeEnemyRotation() {
        if (!target_) return;
        // Get direction to player
        glm::vec3 diff = target_->GetPosition() - position_;

        if (diff.x != 0) {
            float angle = std::atan(diff.y / diff.x);

            SetRotation(angle + 3.14f/2);

        }
    }

    void EnemyGameObject::CheckIFrames() {
        if(is_invincible_) {
            if(iframe_timer_->Finished()) {
                is_invincible_ = false;
            }
        }
    }

	bool EnemyGameObject::CanCollide(GameObject& other) {
        if(!GameObject::CanCollide(other)) return false;
        if (other.HasTag("CanDamageEnemy")) return true;
        if (other.HasTag("PlayerGameObject")) return true;

        return false;
	}

    void EnemyGameObject::OnCollision(game::GameObject &other) {
        if(other.HasTag("CanDamageEnemy")) {
            if(is_invincible_) {
                health_++;
            }
            else {
                iframe_timer_->Start(iframe_duration_);
                is_invincible_ = true;
            }
        }

        GameObject::OnCollision(other);

        // Check if should spawn collectible
        if (should_destroy_) {
            DropCollectible();
        }
    }

    void EnemyGameObject::DropCollectible() {
        // Check if enemy should drop collectible
        int num = rand() % 100 + 1;

        // If num <= collectible_drop_chance_, then drop collectible
        if (num <= collectible_drop_chance_) {
            CollectibleGameObject* collectible = new DarkFairyDustCollectible(position_, geometry_, shader_, Game::GetInstance()->getTexture(Game::tex_dark_fairy_dust), 10.0f);
            Game::GetInstance()->SpawnGameObject(collectible);
        }
    }

	void EnemyGameObject::Detect() {
        if(target_ == nullptr) return;

        // Calculate distance from target
        float distance = glm::distance(target_->GetPosition(), GetPosition());

        if(distance <= flee_range_) {

        }
        if(distance >= idle_range_ && distance < chase_range_) {
            SetAcceleration(glm::vec3(0));
            SetVelocity(glm::vec3(0));
        } else if(distance <= flee_range_) {
            state_ = EnemyMoveState::state_flee;
        }
        else if(distance >= chase_range_){
            state_ = EnemyMoveState::state_chase;
        }
	}

    void EnemyGameObject::Chase() {
        if(target_ == nullptr) return;

        // Calculate acceleration
        acceleration_ = target_->GetPosition() - GetPosition() - velocity_;

        SetAcceleration(acceleration_);
    }

    void EnemyGameObject::Flee() {
        if(target_ == nullptr) return;

        // Calculate acceleration
        acceleration_ = GetPosition() - target_->GetPosition() - velocity_;

        SetAcceleration(acceleration_);
    }


} // namespace game
