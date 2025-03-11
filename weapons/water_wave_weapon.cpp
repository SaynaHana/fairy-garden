#include "water_wave_weapon.h"
#include "../game.h"

namespace game {
    WaterWaveWeapon::WaterWaveWeapon(game::WeaponData &data, game::GameObjectData &projectile_data)
    : IntervalWeapon(data, false) {
        this->projectile_data_ = &projectile_data;
        speed_ = data.GetSpeed();
    }

    void WaterWaveWeapon::Attack(double delta_time) {
        if(projectile_data_ == nullptr || target_ == nullptr) return;

        // Spawn projectile
        glm::vec3 spawnPosition = parent_bearing_ * 1.5f + parent_position_;
        WaterWaveProjectile* projectile1 = new WaterWaveProjectile(spawnPosition, *projectile_data_, projectile_data_->lifetime_, parent_bearing_);
        projectile1->SetTarget(target_);
        projectile1->SetSpeed(speed_);
        WaterWaveProjectile* projectile2 = new WaterWaveProjectile(spawnPosition, *projectile_data_, projectile_data_->lifetime_, parent_bearing_, 3.14f);
        projectile2->SetTarget(target_);
        projectile2->SetSpeed(speed_);

        Game::GetInstance()->SpawnGameObject(projectile1);
        Game::GetInstance()->SpawnGameObject(projectile2);
    }
}
