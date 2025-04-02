#include "magic_missile_enemy.h"

namespace game {
    MagicMissileEnemy::MagicMissileEnemy(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                     GLuint texture, int health, game::MoveData &move_data, game::Weapon *weapon)
                                     : EnemyGameObject(position, geom, shader, texture, health, move_data, weapon) {
		go_right_ = false;
    }

    MagicMissileEnemy::MagicMissileEnemy(const glm::vec3 &position, game::GameObjectData &data, int health,
                                     game::MoveData &move_data, game::Weapon *weapon)
                                     : MagicMissileEnemy(position, data.geom_, data.shader_, data.texture_, health, move_data, weapon) {

    }
	void MagicMissileEnemy::Move(double delta_time) {
		EnemyGameObject::Move(delta_time);

		glm::vec3 side_movement = glm::vec3(0);


		if (go_right_) {
			t_ += delta_time;
			side_movement = glm::vec3(t_, sin(t_), 0);

			if (t_ > 1) go_right_ = false;
		}
		else {
			t_ -= delta_time;
			side_movement = glm::vec3(-t_, sin(t_), 0);

			if (t_ < -1) go_right_ = true;
		}

		SetPosition(GetPosition() + side_movement);
	}
};