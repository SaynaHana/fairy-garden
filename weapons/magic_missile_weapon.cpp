#include <ostream>
#include <iostream>
#include "magic_missile_weapon.h"
#include "../projectiles/magic_missile_projectile.h"
#include "../game.h"

namespace game {
    MagicMissileWeapon::MagicMissileWeapon(game::WeaponData &data, game::GameObjectData &projectileData)
    : IntervalWeapon(data) {
        projectileData_ = &projectileData;
    }

    void MagicMissileWeapon::ExecuteAttack(double delta_time, glm::vec3 parent_position, glm::vec3 parent_bearing) {
        IntervalWeapon::ExecuteAttack(delta_time, parent_position, parent_bearing);
    }

    void MagicMissileWeapon::Attack(double delta_time) {
        std::cout << "WTF" << std::endl;
        if(target_ == nullptr || projectileData_ == nullptr) return;

        // Spawn projectile
        glm::vec3 spawnPosition = parent_bearing_ * 1.5f + parent_position_;
        MagicMissileProjectile* projectile = new MagicMissileProjectile(spawnPosition, *projectileData_);
        projectile->SetTarget(target_);

        Game::GetInstance()->SpawnGameObject(projectile);
    }
}
