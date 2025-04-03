#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "../game_object.h"

namespace game {

    // Inherits from GameObject
    class ParticleSystem : public GameObject {

        public:
            ParticleSystem(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GameObject *parent,
                           const glm::vec3& color, float lifetime = 0.5f, bool finite_lifetime = false);

            void Update(double delta_time) override;

            void Render(glm::mat4 view_matrix, double current_time);

            void SetFiniteLifeTime(bool finite) { finite_lifetime_ = finite; }

        private:
            GameObject *parent_;

            // Use if particle system should be destroyed after some time
            bool finite_lifetime_;
            Timer* lifetime_timer_;
            float lifetime_;
            glm::vec3 color_;

    }; // class ParticleSystem

} // namespace game

#endif // PARTICLE_SYSTEM_H_
