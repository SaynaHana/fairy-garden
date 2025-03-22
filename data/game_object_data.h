#ifndef GAME_OBJECT_DATA_H
#define GAME_OBJECT_DATA_H

#include <glm/glm.hpp>
#include "../geometry.h"
#include "../shader.h"

namespace game {
    class GameObjectData {
    public:
        GameObjectData(Geometry* geom, Shader* shader, GLuint texture, float lifetime = 2.0f, float collider_radius = 0.5f) {
            geom_ = geom;
            shader_ = shader;
            texture_ = texture;
            lifetime_ = lifetime;
            collider_radius_ = collider_radius;
        }

        Geometry* geom_;
        Shader* shader_;
        GLuint texture_;
        float lifetime_;
        float collider_radius_;
    };
}


#endif
