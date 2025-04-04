#include "wall.h"

namespace game {
    Wall::Wall(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader, GLuint texture)
        : GameObject(position, geom, shader, texture, 0, true) {

    }

    Wall::Wall(const glm::vec3 &position, game::GameObjectData *data)
        : Wall(position, data->geom_, data->shader_, data->texture_) {

    }
}
