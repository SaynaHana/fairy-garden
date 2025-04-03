#ifndef PARTICLES_H_
#define PARTICLES_H_

#include "../geometry.h"

namespace game {

    // A set of particles that can be rendered
    class Particles : public Geometry {

        public:
            Particles(void);

            enum Shape {
                radial = 0,
                directional = 1
            };

            // Create the geometry (called once)
            void CreateGeometry(int num_particles, Shape shape);

            // Use the geometry
            void SetGeometry(GLuint shader_program);

    }; // class Particles
} // namespace game

#endif // PARTICLES_H_
