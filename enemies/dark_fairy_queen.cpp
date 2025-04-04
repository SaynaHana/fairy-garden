#include "dark_fairy_queen.h"
#include "../projectiles/magic_missile_projectile.h"
#include "../game.h"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace game {
    DarkFairyQueen::DarkFairyQueen(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                     GLuint texture, int health, game::MoveData &move_data,
                                     GameObjectData* magic_missile_data, GameObjectData* water_wave_data)
            : EnemyGameObject(position, geom, shader, texture, health, move_data, nullptr) {
        phase_ = 1;
        attack_duration_ = 10.0f;
        new_attack_timer_ = new Timer();
        new_attack_timer_->Start(attack_duration_);
        choose_new_attacks_ = true;
        num_attacks_ = 2;

        // Magic missiles
        magic_missile_data_ = magic_missile_data;
        magic_missiles_spawned_ = 0;
        magic_missiles_to_spawn_ = 0;
        magic_missile_speed_ = 1.0f;

        // Water waves
        water_wave_data_ = water_wave_data;
        water_wave_spawn_interval_ = 1.5f;
        water_wave_speed_ = 5;
        water_wave_timer_ = new Timer();
        water_wave_timer_->Start(0);

        tags.insert("DarkFairyQueen");
    }

    DarkFairyQueen::DarkFairyQueen(const glm::vec3 &position, game::GameObjectData &data, int health,
                                     game::MoveData &move_data, GameObjectData* magic_missile_data,
                                     GameObjectData* water_wave_data)
            : DarkFairyQueen(position, data.geom_, data.shader_, data.texture_, health, move_data, magic_missile_data,
                             water_wave_data) {
    }

    void DarkFairyQueen::Update(double delta_time) {
        CheckIFrames();
        if(choose_new_attacks_) {
            Reset();
            GetRandomAttacks(curr_attacks_);
            choose_new_attacks_ = false;
        }

        // Determines how often to spawn new attacks
        if(new_attack_timer_) {
            if(new_attack_timer_->Finished()) {
                choose_new_attacks_ = true;
                new_attack_timer_->Start(attack_duration_);
            }
        }

        PerformAttacks();
    }

    void DarkFairyQueen::PerformAttacks() {
        for(int i = 0; i < curr_attacks_.size(); i++) {
            // Choose attack to perform
            if(curr_attacks_[i] == 1) {
                MagicMissileAttack();
            }
            else if(curr_attacks_[i] == 2) {
                WaterWaveAttack();
            }
        }
    }

    void DarkFairyQueen::Reset() {
        curr_attacks_.clear();
        magic_missiles_spawned_ = 0;
        magic_missiles_to_spawn_ = 0;

        // Destroy all previously spawned objects
        for(int i = 0; i < spawned_objects_.size(); i++) {
            spawned_objects_[i]->SetShouldDestroy(true);
        }
    }

    void DarkFairyQueen::MagicMissileAttack() {
        // Spawn magic missiles that home towards the player
        if(magic_missiles_spawned_ < magic_missiles_to_spawn_) {
            glm::vec3 spawnPosition = GetPosition();
            MagicMissileProjectile* projectile = new MagicMissileProjectile(spawnPosition, *magic_missile_data_, attack_duration_);
            projectile->SetTarget(target_);
            projectile->SetSpeed(magic_missile_speed_);
            projectile->SetScale(glm::vec2(2, 2));
            projectile->SetColliderRadius(1);

            projectile->SetParent(this);
            spawned_objects_.push_back(projectile);
            Game::GetInstance()->SpawnGameObject(projectile);
            magic_missiles_spawned_++;
        }
    }

    void DarkFairyQueen::WaterWaveAttack() {
        // Spawns water wave attack (same as water wave enemy) every interval
        if(water_wave_timer_->Finished()) {
            water_wave_timer_->Start(water_wave_spawn_interval_);

            if(water_wave_data_ == nullptr || target_ == nullptr) return;

            // Spawn projectile
            glm::vec3 spawnPosition = GetPosition();
            WaterWaveProjectile* projectile1 = new WaterWaveProjectile(spawnPosition, *water_wave_data_, attack_duration_, GetBearing());
            projectile1->SetTarget(target_);
            projectile1->SetSpeed(water_wave_speed_);
            projectile1->SetTowardsPlayerSpeed(1.0f);
            WaterWaveProjectile* projectile2 = new WaterWaveProjectile(spawnPosition, *water_wave_data_, attack_duration_, GetBearing(), 3.14f);
            projectile2->SetTarget(target_);
            projectile2->SetSpeed(water_wave_speed_);
            projectile2->SetTowardsPlayerSpeed(1.5f);

            spawned_objects_.push_back(projectile1);
            spawned_objects_.push_back(projectile2);
            Game::GetInstance()->SpawnGameObject(projectile1);
            Game::GetInstance()->SpawnGameObject(projectile2);
        }
    }

    void DarkFairyQueen::GetRandomAttacks(std::vector<int>& attacks) {
        // Choose random numbers
        for(int i = 0; i < phase_; i++) {
            int attack = rand() % num_attacks_ + 1;
            attacks.push_back(attack);

            if(attack == 1) {
                magic_missiles_to_spawn_++;
            }
        }
    }

    void DarkFairyQueen::RemoveSpawnedObject(GameObject* spawned_object) {
        if(spawned_object == nullptr) return;
        auto iter = std::find(spawned_objects_.begin(), spawned_objects_.end(), spawned_object);

        if(iter == spawned_objects_.end()) return;

        if(spawned_object->HasTag("MagicMissileProjectile")) {
            magic_missiles_spawned_--;
        }

        spawned_objects_.erase(iter);
    }
}