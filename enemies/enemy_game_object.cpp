#include "enemy_game_object.h"
#include "../player_game_object.h"
#include "../projectile_game_object.h"

namespace game {
	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon)
	: GameObject(position, geom, shader, texture, 1, true) {
        weapon_ = weapon;

        if(weapon_) {
        }

		state_ = state_patrol;
        idle_range_ = 3;
        chase_range_ = 5;
        flee_range_ = 2;
		target_ = move_data.GetTarget();
        speed_ = move_data.GetSpeed();
        tags.insert("EnemyGameObject");
        tags.insert("CanDamagePlayer");
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

	bool EnemyGameObject::CanCollide(GameObject& other) {
        if(!GameObject::CanCollide(other)) return false;
        if (other.HasTag("CanDamageEnemy")) return true;
        if (other.HasTag("PlayerGameObject")) return true;

        return false;
	}

	void EnemyGameObject::Detect() {
        if(target_ == nullptr) return;

        // Calculate distance from target
        float distance = glm::distance(target_->GetPosition(), GetPosition());

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
