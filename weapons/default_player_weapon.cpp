#include "default_player_weapon.h"
#include "../projectiles/player_projectile.h"
#include "../game.h"

namespace game {
    DefaultPlayerWeapon::DefaultPlayerWeapon(WeaponData &data, GameObjectData& projectile_data)
    : IntervalWeapon(data) {
        projectile_data_ = &projectile_data;
        speed_ = data.GetSpeed();
    }

    DefaultPlayerWeapon::~DefaultPlayerWeapon() {
        IntervalWeapon::~IntervalWeapon();
    }

    void DefaultPlayerWeapon::Attack(double delta_time) {
        if(!projectile_data_) return;

        // Spawn projectile
        glm::vec3 spawnPosition = parent_bearing_ * 1.5f + parent_position_;
        PlayerProjectile* projectile = new PlayerProjectile(spawnPosition, *projectile_data_, projectile_data_->lifetime_,
                                                            parent_bearing_);
        projectile->SetSpeed(speed_);

        Game::GetInstance()->SpawnGameObject(projectile);
    }
}