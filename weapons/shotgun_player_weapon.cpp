#include "shotgun_player_weapon.h"
#include "../projectiles/player_projectile.h"
#include "../game.h"

namespace game {
    ShotgunWeapon::ShotgunWeapon(WeaponData &data, GameObjectData& projectile_data, float angle_increment, int half_num_projectiles)
            : IntervalWeapon(data) {
        projectile_data_ = &projectile_data;
        speed_ = data.GetSpeed();
        angle_increment_ = angle_increment;
        half_num_projectiles_ = half_num_projectiles;
    }

    ShotgunWeapon::~ShotgunWeapon() {
        IntervalWeapon::~IntervalWeapon();
    }

    void ShotgunWeapon::Attack(double delta_time) {
        if(!projectile_data_) return;

        // Spawn projectile
        glm::vec3 spawnPosition = parent_bearing_ * 1.5f + parent_position_;
        PlayerProjectile* projectile = new PlayerProjectile(spawnPosition, *projectile_data_, projectile_data_->lifetime_,
                                                            parent_bearing_);
        projectile->SetSpeed(speed_);

        Game::GetInstance()->SpawnGameObject(projectile);

        // Get angle
        if(parent_bearing_.x == 0) {
            parent_bearing_.x = 1;
        }

        float angle = std::atan(parent_bearing_.y / parent_bearing_.x);

        if (parent_bearing_.x < 0) {
            angle += 3.14f;
        }

        for(int i = 1; i <= half_num_projectiles_ * 2; i++) {
            float new_angle = angle;
            if(i <= half_num_projectiles_) {
                new_angle = angle - (angle_increment_ * (3.14f / 180) * i);
            }
            else {
                new_angle = angle + (angle_increment_ * (3.14f / 180) * (i % half_num_projectiles_));
            }

            glm::vec3 direction = glm::vec3(std::cos(new_angle), std::sin(new_angle), 0);

            PlayerProjectile* new_projectile = new PlayerProjectile(spawnPosition, *projectile_data_, projectile_data_->lifetime_,
                                                                    direction);

            new_projectile->SetSpeed(speed_);

            Game::GetInstance()->SpawnGameObject(new_projectile);
        }
    }
}
