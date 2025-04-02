#ifndef WATER_WAVE_ENEMY_H
#define WATER_WAVE_ENEMY_H

#include "enemy_game_object.h"

namespace game {
	class WaterWaveEnemy : public EnemyGameObject {
		public:
            WaterWaveEnemy(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon);
            WaterWaveEnemy(const glm::vec3& position, GameObjectData& data, int health, MoveData& move_data, Weapon* weapon);

		protected:
			void Move(double delta_time) override;

			float t_;
			float circle_radius_;
	};
}

#endif