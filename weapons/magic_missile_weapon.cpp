#include "magic_missile_weapon.h"
#include "../projectiles/magic_missile_projectile.h"
#include "../game.h"

namespace game {
    MagicMissileWeapon::MagicMissileWeapon(game::WeaponData &data, game::GameObjectData &projectileData)
    : IntervalWeapon(data) {
        projectileData_ = &projectileData;
    }

    void MagicMissileWeapon::Attack(double delta_time) {
        if(target_ == nullptr || projectileData_ == nullptr) return;

        // Spawn projectile
        glm::vec3 spawnPosition = parent_bearing_ * 1.5f + parent_position_;
        MagicMissileProjectile* projectile = new MagicMissileProjectile(spawnPosition, *projectileData_);
        projectile->SetTarget(target_);

        Game::GetInstance()->SpawnGameObject(projectile);
    }
}
