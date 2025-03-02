#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "game_object.h"

namespace game {
	class Projectile : public GameObject {
		public:
			Projectile(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int speed, float lifetime = 2.0f);

            void Update(double delta_time) override;

            bool CanCollide(GameObject& other) override;

        private:
            Timer* destroy_timer_;
            float lifetime_;
	};
} // namespace game

#endif