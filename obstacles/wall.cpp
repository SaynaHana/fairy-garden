#include "wall.h"

namespace game {
    Wall::Wall(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader, GLuint texture, float length, float width)
        : GameObject(position, geom, shader, texture, 0, true) {
        length_ = length;
        width_ = width;
    }

    Wall::Wall(const glm::vec3 &position, game::GameObjectData *data, float length, float width)
        : Wall(position, data->geom_, data->shader_, data->texture_, length, width) {

    }
}
