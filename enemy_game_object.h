//
// Created by Aerisia on 26/01/25.
//

#ifndef ENEMY_GAME_OBJECT_H
#define ENEMY_GAME_OBJECT_H

#include "game_object.h"
#include "patrol_data.h"
#include <iostream>
#include "move_data.h"
#include "attack_data.h"

#define INTERCEPT_DIRECTION_INTERVAL 2 // How often the enemy changes their target position when intercepting

namespace game {
    class EnemyGameObject : public GameObject {
		public:
            EnemyGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data, PatrolData& patrol_data);
    		~EnemyGameObject();

            void Update(double delta_time) override;

            bool CanCollide(GameObject& other) override;

		protected:
    		int state_;
            float chase_range_;
            float idle_range_;
            float flee_range_;

    		void Detect(); // Changes the state of the enemy depending on distance to target

			GameObject* target_;

            // Intercepting
            float intercept_speed_;
    		glm::vec3 intercept_direction_;
    		Timer* intercept_timer_; // Checks if the enemy should update the target position

            void Intercept(double delta_time);

            // Patrolling
            float patrol_width_;
            float patrol_height_;
            glm::vec3 patrol_center_point_;
            float patrol_t_; // Keeps track of where we are in the parametric equation

            void Patrol(float t);

			// Chase
			void Chase();

			// Flee
			void Flee();
    };
}



#endif //ENEMY_GAME_OBJECT_H
