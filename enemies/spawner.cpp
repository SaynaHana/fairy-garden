#include "spawner.h"
#include "../game.h"
#include "../collectible_game_object.h"
#include "../collectibles/fairy_dust_collectible.h"

namespace game {
	Spawner::Spawner(int initial_cost, int cost_increment, GameObject* player, GameObjectData* obj_data) {
        data_ = obj_data;
        player_ = player;
		cost_ = initial_cost;
		cost_increment_ = cost_increment;
        started_ = false;
        round_count_ = 0;
        enemy_count_ = 0;

        min_spawn_dist_ = 1;

        spawn_interval_ = 1;
        can_spawn_ = false;
        spawn_timer_ = new Timer();
        spawn_timer_->Start(spawn_interval_);

        enemy_costs_.insert({ 1, "MagicMissileEnemy" });
        enemy_costs_.insert({2, "WaterWaveEnemy"});

        collectible_spawn_interval_ = 15.0f;
        collectible_spawn_timer_ = new Timer();
	}

    bool Spawner::Start() {
        if(started_) return false;

        NextRound();
        started_ = true;

        return true;
    }

    void Spawner::Update(double delta_time) {
        if(!started_) return;

        // Check if there are any enemies left
        if(enemy_count_ == 0) {
            // Start new round
            round_count_++;

            NextRound();
        }
        else {
            if(!spawn_queue_.empty()) {
                if(can_spawn_) {
                    SpawnEnemy(spawn_queue_.front());
                    spawn_queue_.pop();
                    can_spawn_ = false;

                    if(!spawn_timer_) return;
                    spawn_timer_->Start(spawn_interval_);
                }
                else {
                    if(spawn_timer_) {
                        if(spawn_timer_->Finished()) {
                            can_spawn_ = true;
                        }
                    }
                }
            }
        }

        // Collectibles
        if(collectible_spawn_timer_) {
            if(collectible_spawn_timer_->Finished()) {
                collectible_spawn_timer_->Start(collectible_spawn_interval_);
                SpawnCollectible();
            }
        }
    }

    void Spawner::OnEnemyDeath() {
        enemy_count_--;
    }

    void Spawner::NextRound() {
        std::cout << "Wave: " << round_count_ << std::endl;
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

                spawn_queue_.push(enemy_costs_[i]);
                enemy_count_++;
            }
        }
    }

    void Spawner::SpawnEnemy(const std::string &name) {
        MoveData move_data = MoveData(0.5, player_);
        Game* game = Game::GetInstance();
        EnemyGameObject* enemy = nullptr;
        WeaponData* weapon_data = nullptr;
        GameObjectData* projectile_data = nullptr;
        Weapon* weapon = nullptr;
        GameObjectData* enemy_data = nullptr;


        if(name == "MagicMissileEnemy") {
            weapon_data = new WeaponData(player_, 2, 3.0f);
            projectile_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_enemy_projectile), 5);
            weapon = new MagicMissileWeapon(*weapon_data, *projectile_data);
            enemy_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_green_ship));
        }
        else if(name == "WaterWaveEnemy") {
            weapon_data = new WeaponData(player_, 2, 5.0f);
            projectile_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_water_projectile), 5);
            weapon = new WaterWaveWeapon(*weapon_data, *projectile_data);
            enemy_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_green_ship));
        }

        if(enemy_data) {
            enemy = new EnemyGameObject(GetLocationAroundPlayer(), *enemy_data, 2, move_data, weapon);
        }

        if(enemy) {
            game->SpawnGameObject(enemy);
        }
    }

    void Spawner::SpawnCollectible() {
        CollectibleGameObject* collectible = nullptr;
        Game* game = Game::GetInstance();

        collectible = new FairyDustCollectible(GetLocationAroundPlayer(), data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_fairy_dust), 5.0f);
        if(collectible_count % 3) {
        }

        if(collectible) {
            game->SpawnGameObject(collectible);
        }
    }

    glm::vec3 Spawner::GetLocationAroundPlayer() {
        if(player_ == nullptr) return {0, 0, 0};

        glm::vec3 pos = player_->GetPosition();

        while(glm::length(pos - player_->GetPosition()) < min_spawn_dist_) {
            // Get random location around player
            float randX = ((float)rand() / RAND_MAX) * 20 - 10;
            float randY = ((float)rand() / RAND_MAX) * 20 - 10;

            pos = glm::vec3(randX, randY, 0) + player_->GetPosition();
        }

        return pos;
    }
	
}