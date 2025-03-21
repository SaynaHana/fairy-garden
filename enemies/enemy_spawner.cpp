#include "enemy_spawner.h"
#include "../game.h"

namespace game {
	EnemySpawner::EnemySpawner(int initial_cost, int cost_increment, GameObject* player, GameObjectData* obj_data) {
        data_ = obj_data;
        player_ = player;
		cost_ = initial_cost;
		cost_increment_ = cost_increment;
        started_ = false;
        round_count_ = 0;
        enemy_count_ = 0;

        minSpawnDist = 1;

        enemy_costs_.insert({ 1, "MagicMissileEnemy" });
	}

    bool EnemySpawner::Start() {
        if(started_) return false;

        NextRound();
        started_ = true;

        return true;
    }

    void EnemySpawner::Update(double delta_time) {
        if(!started_) return;

        // Check if there are any enemies left
        if(enemy_count_ == 0) {
            // Start new round
            round_count_++;

            NextRound();
        }
    }

    void EnemySpawner::OnEnemyDeath() {
        enemy_count_--;
    }

    void EnemySpawner::NextRound() {
        int counter = cost_ + cost_increment_;

        if(round_count_ == 0) {
            counter -= cost_increment_;
        }

        cost_ = counter;

        while(counter > 0) {
            for(int i = counter; i > 0; i--) {
                // Try to find highest cost can afford
                if(enemy_costs_.find(i) == enemy_costs_.end()) continue;

                counter -= i;

                SpawnEnemy(enemy_costs_[i]);
            }
        }
    }

    void EnemySpawner::SpawnEnemy(const std::string &name) {
        MoveData move_data = MoveData(0.5, player_);
        Game* game = Game::GetInstance();
        EnemyGameObject* enemy = nullptr;

        if(name == "MagicMissileEnemy") {
            WeaponData* weapon_data = new WeaponData(player_, 2, 1.75f);
            GameObjectData* magic_missile_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_enemy_projectile), 5);
            MagicMissileWeapon* magic_missile_weapon = new MagicMissileWeapon(*weapon_data, *magic_missile_data);
            GameObjectData enemy_data = GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_green_ship));
            auto* magic_missile_enemy = new EnemyGameObject(GetLocationAroundPlayer(), enemy_data, 2, move_data, magic_missile_weapon);
            enemy = magic_missile_enemy;
            game->SpawnGameObject(magic_missile_enemy);
        }

        if(enemy) {
            enemy_count_++;
        }
    }

    glm::vec3 EnemySpawner::GetLocationAroundPlayer() {
        if(player_ == nullptr) return {0, 0, 0};

        glm::vec3 pos = player_->GetPosition();

        while(glm::length(pos - player_->GetPosition()) < minSpawnDist) {
            // Get random location around player
            float randX = ((float)rand() / RAND_MAX) * 10 - 5;
            float randY = ((float)rand() / RAND_MAX) * 10 - 5;

            pos = glm::vec3(randX, randY, 0) + player_->GetPosition();
        }

        return pos;
    }
	
}