#ifndef WALL_H
#define WALL_H

#include "../game_object.h"

namespace game {
    class Wall : public GameObject {
        public:
            Wall(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);
            Wall(const glm::vec3& position, GameObjectData* data);
    };
}

#endif
