#include "enemy_game_object.h"
#include "player_game_object.h"
#include "projectile_game_object.h"

namespace game {
	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, int health, MoveData& move_data, PatrolData& patrol_data) 
	: GameObject(position, geom, shader, texture, 1, true) {
		state_ = state_patrol;
        idle_range_ = 3;
        chase_range_ = 5;
        flee_range_ = 2;
		intercept_speed_ = move_data.GetSpeed();
		target_ = move_data.GetTarget();
		intercept_direction_ = glm::vec3(0, 0, 0);
		intercept_timer_ = new Timer();
		patrol_width_ = patrol_data.GetWidth();
		patrol_height_ = patrol_data.GetHeight();
		patrol_center_point_ = patrol_data.GetCenterPoint();
		patrol_t_ = 0;
        speed_ = move_data.GetSpeed();
        tags.insert("EnemyGameObject");
	}

	EnemyGameObject::~EnemyGameObject() {
		if (intercept_timer_ != nullptr) {
			delete intercept_timer_;
		}

		GameObject::~GameObject();
	}


	void EnemyGameObject::Update(double delta_time) {
		Detect();

        if(state_ == EnemyMoveState::state_chase) {
            Chase();
        }
        else if(state_ == EnemyMoveState::state_flee) {
            Flee();
        }

		GameObject::Update(delta_time);
	}

	bool EnemyGameObject::CanCollide(GameObject& other) {
		// Enemy only collides with player and projectiles
		PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(&other);
        Projectile* projectile = dynamic_cast<Projectile*>(&other);

		return player != nullptr || projectile != nullptr;
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


	void EnemyGameObject::Intercept(double delta_time) {
		if (target_ == nullptr) return;

		// Check if it is time to update target position
		if (intercept_timer_ != nullptr && intercept_timer_->Finished()) {
			// Get vector between this enemy and target
			intercept_direction_ = target_->GetPosition() - GetPosition();
			glm::normalize(intercept_direction_);

			intercept_timer_->Start(INTERCEPT_DIRECTION_INTERVAL);
		}

		// Move towards target
		SetPosition(intercept_direction_ * intercept_speed_ * (float)delta_time + GetPosition());
	}

	void EnemyGameObject::Patrol(float t) {
		// Create circle
		float x = patrol_width_ * cos(t) + patrol_center_point_.x;
		float y = patrol_height_ * sin(t) + patrol_center_point_.y;

		SetPosition(glm::vec3(x, y, 0));
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
