#include "dark_fairy_queen.h"
#include "../projectiles/magic_missile_projectile.h"
#include "../game.h"
#include <GLFW/glfw3.h>
#include <algorithm>

namespace game {
    DarkFairyQueen::DarkFairyQueen(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                     GLuint texture, int health, game::MoveData &move_data,
                                     GameObjectData* magic_missile_data)
            : EnemyGameObject(position, geom, shader, texture, health, move_data, nullptr) {
        phase_ = 1;
        attack_duration_ = 10.0f;
        new_attack_timer_ = new Timer();
        new_attack_timer_->Start(attack_duration_);
        choose_new_attacks_ = true;
        num_attacks_ = 1;

        magic_missile_data_ = magic_missile_data;
        magic_missiles_spawned_ = 0;
        magic_missiles_to_spawn_ = 0;

        tags.insert("DarkFairyQueen");
    }

    DarkFairyQueen::DarkFairyQueen(const glm::vec3 &position, game::GameObjectData &data, int health,
                                     game::MoveData &move_data, GameObjectData* magic_missile_data)
            : DarkFairyQueen(position, data.geom_, data.shader_, data.texture_, health, move_data, magic_missile_data) {
    }

    void DarkFairyQueen::Update(double delta_time) {
        CheckIFrames();
        if(choose_new_attacks_) {
            Reset();
            GetRandomAttacks(curr_attacks_);
            choose_new_attacks_ = false;
        }

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
        if(magic_missiles_spawned_ < magic_missiles_to_spawn_) {
            glm::vec3 spawnPosition = GetPosition();
            MagicMissileProjectile* projectile = new MagicMissileProjectile(spawnPosition, *magic_missile_data_, attack_duration_);
            projectile->SetTarget(target_);
            projectile->SetSpeed(0.5f);
            projectile->SetScale(glm::vec2(2, 2));
            projectile->SetColliderRadius(1);

            projectile->SetParent(this);
            spawned_objects_.push_back(projectile);
            Game::GetInstance()->SpawnGameObject(projectile);
            magic_missiles_spawned_++;
        }
    }

    void DarkFairyQueen::GetRandomAttacks(std::vector<int>& attacks) {
        // Choose random numbers
        srand(glfwGetTime());
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