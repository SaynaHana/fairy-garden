#include "default_player_weapon.h"
#include "../projectiles/player_projectile.h"
#include "../game.h"
#include "../particles/particle_system.h"

namespace game {
    DefaultPlayerWeapon::DefaultPlayerWeapon(WeaponData &data, GameObjectData& projectile_data, GameObjectData* particle_data)
    : IntervalWeapon(data) {
        projectile_data_ = &projectile_data;
        speed_ = data.GetSpeed();
        particle_data_ = particle_data;
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

        if(particle_data_) {
            // Spawn particle system
            glm::vec3 particle_spawn = glm::vec3(0);

            if(parent_bearing_.x < 0) {
                particle_spawn = -(0.3f * parent_bearing_);
            }
            else {
                particle_spawn = -parent_bearing_;
            }

            GameObject* particle_system = new ParticleSystem(particle_spawn, particle_data_->geom_,
                                                                 particle_data_->shader_, particle_data_->texture_,
                                                                 projectile, glm::vec3(0, 1.0f, 0),
                                                                 projectile_data_->lifetime_, true);

            // Calculate particle system rotation
            float theta = std::atan(parent_bearing_.y / parent_bearing_.x) + (3.14f / 2.0f);
            particle_system->SetRotation(theta);

            particle_system->SetScale(glm::vec2(0.2));
            // When the projectile gets destroyed, so will the particle system
            projectile->AddChild(particle_system);
            Game::GetInstance()->SpawnParticles(particle_system);
        }
    }
}