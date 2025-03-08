#ifndef GAME_OBJECT_DATA_H
#define GAME_OBJECT_DATA

#include <glm/glm.hpp>
#include "../geometry.h"
#include "../shader.h"

namespace game {
    class GameObjectData {
    public:
        GameObjectData(glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) {
            position_ = position;
            geom_ = geom;
            shader_ = shader;
            texture_ = texture;
        }

        glm::vec3 position_;
        Geometry* geom_;
        Shader* shader_;
        GLuint texture_;
    };
}


#endif
