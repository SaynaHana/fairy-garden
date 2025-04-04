#ifndef WALL_H
#define WALL_H

#include "../game_object.h"

namespace game {
    class Wall : public GameObject {
        public:
            Wall(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, float length, float width);
            Wall(const glm::vec3& position, GameObjectData* data, float length, float width);

        private:
            float length_;
            float width_; // How thick is the wall
    };
}

#endif
