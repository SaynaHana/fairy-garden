#ifndef MAGIC_MISSILE_ENEMY_H
#define MAGIC_MISSILE_ENEMY_H

#include "enemy_game_object.h"

namespace game {
	class MagicMissileEnemy : public EnemyGameObject {
		public:
            MagicMissileEnemy(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon);
            MagicMissileEnemy(const glm::vec3& position, GameObjectData& data, int health, MoveData& move_data, Weapon* weapon);

		protected:
			void Move(double delta_time) override;

			float t_; // To do parametric movement
			bool go_right_;
	};
}

#endif