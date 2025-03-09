#ifndef GAME_OBJECT_DATA_H
#define GAME_OBJECT_DATA_H

#include <glm/glm.hpp>
#include "../geometry.h"
#include "../shader.h"

namespace game {
    class GameObjectData {
    public:
        GameObjectData(Geometry* geom, Shader* shader, GLuint texture) {
            geom_ = geom;
            shader_ = shader;
            texture_ = texture;
        }

        Geometry* geom_;
        Shader* shader_;
        GLuint texture_;
    };
}


#endif
