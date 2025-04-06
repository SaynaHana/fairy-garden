#include "dark_fairy_queen.h"
#include "../projectiles/magic_missile_projectile.h"
#include "../game.h"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace game {
    DarkFairyQueen::DarkFairyQueen(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                     GLuint texture, int health, game::MoveData &move_data,
                                     GameObjectData* text_data, GameObjectData* magic_missile_data, 
                                     GameObjectData* water_wave_data)
            : EnemyGameObject(position, geom, shader, texture, health, move_data, nullptr) {
        phase_ = 1;
        attack_duration_ = 10.0f;
        new_attack_timer_ = new Timer();
        new_attack_timer_->Start(attack_duration_);
        choose_new_attacks_ = true;
        num_attacks_ = 2;
        max_health_ = health;

        min_spawn_dist_ = -1.0f;
        max_spawn_dist_ = 1.0f;

        // Magic missiles
        magic_missile_data_ = magic_missile_data;
        magic_missiles_spawned_ = 0;
        magic_missiles_to_spawn_ = 0;
        magic_missile_speed_ = 1.0f;

        // Water waves
        water_wave_data_ = water_wave_data;
        water_wave_spawn_interval_ = 2.5f;
        water_wave_speed_ = 5;
        water_wave_timer_ = new Timer();
        water_wave_timer_->Start(0);
        performed_water_attack_ = false;
        water_waves_ = 0;

        tags.insert("DarkFairyQueen");

        // Setup health text
        health_text_ = new TextGameObject(glm::vec3(position.x, position.y - 3.5f, 0), text_data->geom_, text_data->shader_, text_data->texture_);
        health_text_->SetParent(this);
        Game::GetInstance()->SpawnGameObject(health_text_);
    }

    DarkFairyQueen::DarkFairyQueen(const glm::vec3 &position, GameObjectData &data, int health,
                                     MoveData &move_data, GameObjectData* text_data, GameObjectData* magic_missile_data,
                                     GameObjectData* water_wave_data)
            : DarkFairyQueen(position, data.geom_, data.shader_, data.texture_, health, move_data, text_data,
							magic_missile_data, water_wave_data) {
    }

    void DarkFairyQueen::Update(double delta_time) {
        // Update text
        if (health_text_) {
			std::string str_health = "HP: " + std::to_string(health_);
			health_text_->SetText(str_health);
			health_text_->SetScale(glm::vec2((float)str_health.length() / 4.0f, 0.25f));
        }

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
            else {
                if(spawned_objects_.size() == 0) {
                    choose_new_attacks_ = true;
                    new_attack_timer_->Start(attack_duration_);
                }
            }
        }

        PerformAttacks();
    }

    void DarkFairyQueen::OnCollision(game::GameObject &other) {
        EnemyGameObject::OnCollision(other);

        // If health is at 2/3, then go to next phase
        if((float)health_ <= 2.0f / 3.0f * (float)max_health_ && phase_ == 1) {
            phase_++;
        }
        // If health is at 1/3, then go to final phase
        else if((float)health_ <= 1.0f / 3.0f * (float)max_health_ && phase_ == 2) {
            phase_++;
        }

    }

    void DarkFairyQueen::PerformAttacks() {
        performed_water_attack_ = false;
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
        water_waves_ = 0;

        // Destroy all previously spawned objects
        for(int i = 0; i < spawned_objects_.size(); i++) {
            spawned_objects_[i]->SetShouldDestroy(true);
        }
    }

    void DarkFairyQueen::MagicMissileAttack() {
        // Spawn magic missiles that home towards the player
        if(magic_missiles_spawned_ < magic_missiles_to_spawn_) {
            glm::vec3 spawnPosition = GetRandomLocation();
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
        if(water_waves_ <= 0) return;
        if(performed_water_attack_) return;
        // Spawns water wave attack (same as water wave enemy) every interval
        if(water_wave_timer_->Finished()) {
            water_wave_timer_->Start(water_wave_spawn_interval_ / (float)water_waves_);

            if(water_wave_data_ == nullptr || target_ == nullptr) return;

            // Spawn projectile
            glm::vec3 spawnPosition = GetLocationAroundBoss();
            WaterWaveProjectile* projectile1 = new WaterWaveProjectile(spawnPosition, *water_wave_data_, attack_duration_, GetBearing());
            projectile1->SetTarget(target_);
            projectile1->SetSpeed(water_wave_speed_);
            projectile1->SetTowardsPlayerSpeed(1.0f);
            WaterWaveProjectile* projectile2 = new WaterWaveProjectile(spawnPosition, *water_wave_data_, attack_duration_, GetBearing(), 3.14f);
            projectile2->SetTarget(target_);
            projectile2->SetSpeed(water_wave_speed_);
            projectile2->SetTowardsPlayerSpeed(1.5f);

            projectile1->SetParent(this);
            projectile2->SetParent(this);
            spawned_objects_.push_back(projectile1);
            spawned_objects_.push_back(projectile2);
            Game::GetInstance()->SpawnGameObject(projectile1);
            Game::GetInstance()->SpawnGameObject(projectile2);
        }

        performed_water_attack_ = true;
    }

    void DarkFairyQueen::GetRandomAttacks(std::vector<int>& attacks) {
        // Choose random numbers that represent attacks
        for(int i = 0; i < phase_; i++) {
            int attack = rand() % num_attacks_ + 1;
            attacks.push_back(attack);

            // Magic missiles
            if(attack == 1) {
                magic_missiles_to_spawn_++;
            }
            // Water waves
            else if(attack == 2) {
                water_waves_++;
            }
        }
    }

    glm::vec3 DarkFairyQueen::GetRandomLocation() {
        float x = ((float)(rand())) / ((float(RAND_MAX)/(max_spawn_dist_ - min_spawn_dist_)));
        float y = ((float)(rand())) / ((float(RAND_MAX)/(max_spawn_dist_ - min_spawn_dist_)));
        return glm::vec3(x, y, 0) + GetPosition();
    }

    void DarkFairyQueen::RemoveSpawnedObject(GameObject* spawned_object) {
        if(spawned_object == nullptr) return;
        auto iter = std::find(spawned_objects_.begin(), spawned_objects_.end(), spawned_object);

        if(iter == spawned_objects_.end()) return;

        spawned_objects_.erase(iter);
    }

    glm::vec3 DarkFairyQueen::GetLocationAroundBoss() {
        glm::vec3 pos = GetPosition();

        while(glm::length(pos - GetPosition()) < 5) {
            // Get random location around player
            float randX = ((float)rand() / RAND_MAX) * 20 - 10;
            float randY = ((float)rand() / RAND_MAX) * 20 - 10;

            pos = glm::vec3(randX, randY, 0) + GetPosition();
        }

        return pos;
    }
}