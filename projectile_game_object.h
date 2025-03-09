#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "game_object.h"
#include "data/game_object_data.h"

namespace game {
	class Projectile : public GameObject {
		public:
			Projectile(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, float lifetime = 2.0f);
            Projectile(const glm::vec3 &position, GameObjectData& data, float lifetime = 2.0f);

            void Update(double delta_time) override;

            bool CanCollide(GameObject& other) override;

        private:
            Timer* destroy_timer_;
            float lifetime_;
	};
} // namespace game

#endif