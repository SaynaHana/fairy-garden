#include "projectile_game_object.h"
#include "enemies/enemy_game_object.h"

namespace game {
	Projectile::Projectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, float lifetime)
	: GameObject(position, geom, shader, texture, 0, true) {
        destroy_timer_ = new Timer();
        lifetime_ = lifetime;
        destroy_timer_->Start(lifetime);
        should_explode_ = false;
        collision_type_ = CollisionType::ray_;
        tags.insert("CanDamageEnemy");
        tags.insert("Projectile");
	}

    Projectile::Projectile(const glm::vec3 &position, game::GameObjectData &data, float lifetime)
    : Projectile(position, data.geom_, data.shader_, data.texture_, lifetime) {

    }

    void Projectile::Update(double delta_time) {
        GameObject::Update(delta_time);

        if(destroy_timer_ != nullptr) {
            if(destroy_timer_->Finished()) {
                collision_on_ = false;
                should_destroy_ = true;
            }
        }
    }

    bool Projectile::CanCollide(game::GameObject &other) {
        // Only collides with enemies
        EnemyGameObject* enemy = dynamic_cast<EnemyGameObject*>(&other);

        return enemy != nullptr;
    }

} // namespace game
