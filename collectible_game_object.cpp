#include "collectible_game_object.h"

namespace game {
    CollectibleGameObject::CollectibleGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
    : GameObject(position, geom, shader, texture, 0, true) {
        should_explode_ = false;
        damage_ = 0;
    }

    void CollectibleGameObject::OnCollision(GameObject& other) {
        should_destroy_ = true;
    }


} // namespace game