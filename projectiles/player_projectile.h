#ifndef PLAYER_PROJECTILE_H
#define PLAYER_PROJECTILE_H

#include "../projectile_game_object.h"

namespace game {
    class PlayerProjectile : public Projectile {
    public:
        PlayerProjectile(const glm::vec3& position, GameObjectData& data, float lifetime = 2.0f, const glm::vec3& direction = glm::vec3(0, 0, 0));

        void Move(double delta_time) override;

        bool CanCollide(GameObject& other) override;
        void OnCollision(GameObject& other) override;

    private:
        glm::vec3 direction_;
    };
}


#endif
