#ifndef ENEMY_GAME_OBJECT_H
#define ENEMY_GAME_OBJECT_H

#include "../game_object.h"
#include "../patrol_data.h"
#include <iostream>
#include "../data/move_data.h"
#include "../attack_data.h"
#include "../weapons/weapon.h"

namespace game {
    class EnemyGameObject : public GameObject {
		public:
            EnemyGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon);
            EnemyGameObject(const glm::vec3& position, GameObjectData& data, int health, MoveData& move_data, Weapon* weapon);

            virtual ~EnemyGameObject();

            void Update(double delta_time) override;

            bool CanCollide(GameObject& other) override;
            void OnCollision(GameObject& other) override;

		protected:
    		int state_;
            float chase_range_;
            float idle_range_;
            float flee_range_;

            Weapon* weapon_;


    		void Detect(); // Changes the state of the enemy depending on distance to target

			GameObject* target_;

			// Chase
			void Chase();

			// Flee
			void Flee();

            // Iframes
            Timer* iframe_timer_;
            float iframe_duration_;
            bool is_invincible_;

            void CheckIFrames();
    };
}



#endif //ENEMY_GAME_OBJECT_H
