#ifndef PROJECTILE_WEAPON_BEHAVIOUR_H
#define PROJECTILE_WEAPON_BEHAVIOUR_H

#include "weapon_behaviour.h"
#include "../game_object.h"
#include "../data/game_object_data.h"

namespace game {
    class ProjectileWeaponBehaviour : WeaponBehaviour {
    public:
        ProjectileWeaponBehaviour(GameObjectData& projectile_data, GameObject& target, float speed);

        virtual void Execute(double delta_time) override;

        // Set the position of where the projectile will spawn
        void SetProjectilePosition(glm::vec3& position) {
            position_ = position;
        }

    private:
        GameObjectData projectile_data_;
        glm::vec3 position_;
        GameObject target_;
        float speed_;
    };
}


#endif
