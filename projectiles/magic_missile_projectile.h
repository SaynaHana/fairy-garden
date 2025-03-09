#ifndef MAGIC_MISSILE_PROJECTILE_H
#define MAGIC_MISSILE_PROJECTILE_H

#include "../projectile_game_object.h"
#include "../data/game_object_data.h"

namespace game {
    class MagicMissileProjectile : public Projectile {
        public:
			MagicMissileProjectile(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, float lifetime = 2.0f);
            MagicMissileProjectile(const glm::vec3& position, GameObjectData& data, float lifetime = 2.0f);

            void SetTarget(GameObject* target) { target_ = target; }

        protected:
            virtual void Move(double delta_time) override;

        private:
            GameObject* target_;
    };
} // namespace game

#endif
