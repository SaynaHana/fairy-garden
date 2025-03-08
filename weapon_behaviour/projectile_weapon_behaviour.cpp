#include "projectile_weapon_behaviour.h"

namespace game {
    ProjectileWeaponBehaviour::ProjectileWeaponBehaviour(GameObjectData& projectile_data, GameObject &target, float speed)
    : WeaponBehaviour(), projectile_data_(projectile_data), target_(target) {
        speed_ = speed;
    }

}
