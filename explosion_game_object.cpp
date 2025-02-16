#include "explosion_game_object.h"

namespace game {
    ExplosionGameObject::ExplosionGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int duration)
    : GameObject(position, geom, shader, texture, 0, false) {
        duration_ = duration;
    	should_explode_ = false;
    	damage_ = 0;

        timer_ = new Timer();
        timer_->Start(duration_);
    }

    void ExplosionGameObject::Update(double delta_time) {
    	GameObject::Update(delta_time);

		// Check if game object should be destroyed
		if (timer_ != nullptr) {
			if(timer_->IsEnabled() && timer_->Finished()) {
				/*
				 * I wanted to delete the object here, but then it wouldn't
				 * be removed from the game objects vector in game.cpp
				 * causing a segfault, so instead it will be deleted on the next tick
				 */
				should_destroy_ = true;
			}
		}
    }

} // namespace game

