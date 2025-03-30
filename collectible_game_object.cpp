#include "collectible_game_object.h"
#include "player_game_object.h"
#include "game.h"

namespace game {
    CollectibleGameObject::CollectibleGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, float duration)
    : GameObject(position, geom, shader, texture, 0, true) {
        should_explode_ = false;
        damage_ = 0;
        collected = false;
        duration_ = duration;
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
        if(!GameObject::CanCollide(other)) return false;
        if (other.HasTag("PlayerGameObject")) return true;
        return false;
    }

    void CollectibleGameObject::OnCollision(GameObject& other) {
        collision_on_ = false;
        should_destroy_ = true;
        collected = true;
        Game::GetInstance()->AddObjective(Game::Objective::collectible_collected);
    }

} // namespace game