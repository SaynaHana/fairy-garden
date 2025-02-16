//
// Created by Aerisia on 25/01/25.
//

#ifndef COLLECTIBLE_GAME_OBJECT_H
#define COLLECTIBLE_GAME_OBJECT_H
#include "game_object.h"


namespace game {
    class CollectibleGameObject : public GameObject {
        public:
            CollectibleGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            void OnCollision(GameObject& other) override;
    };
} // namespace game



#endif //COLLECTIBLE_GAME_OBJECT_H
