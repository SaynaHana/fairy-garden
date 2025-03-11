#ifndef GAME_OBJECT_DATA_H
#define GAME_OBJECT_DATA_H

#include <glm/glm.hpp>
#include "../geometry.h"
#include "../shader.h"

namespace game {
    class GameObjectData {
    public:
        GameObjectData(Geometry* geom, Shader* shader, GLuint texture, float lifetime = 2.0f) {
            geom_ = geom;
            shader_ = shader;
            texture_ = texture;
            lifetime_ = lifetime;
        }

        Geometry* geom_;
        Shader* shader_;
        GLuint texture_;
        float lifetime_;
    };
}


#endif
