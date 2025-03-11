#include "collectible_game_object.h"
#include "player_game_object.h"

namespace game {
    CollectibleGameObject::CollectibleGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
    : GameObject(position, geom, shader, texture, 0, true) {
        should_explode_ = false;
        damage_ = 0;
        collected = false;
        tags.insert("Collectible");
    }

    void CollectibleGameObject::Update(double delta_time) {
        GameObject::Update(delta_time);
    }

    void CollectibleGameObject::Render(glm::mat4 view_matrix, double current_time) {
        // Check if ghost
        shader_->Enable();

        if(collected) {
            shader_->SetUniform1i("ghost", 1);
        }
        else {
            shader_->SetUniform1i("ghost", 0);
        }

        GameObject::Render(view_matrix, current_time);
    }


    bool CollectibleGameObject::CanCollide(game::GameObject &other) {
        // Can only collide with player
        PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(&other);

        return player != nullptr;
    }

    void CollectibleGameObject::OnCollision(GameObject& other) {
        collision_on_ = false;
        collected = true;
    }


} // namespace game