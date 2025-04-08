#include "spawner.h"
#include "../game.h"
#include "../collectible_game_object.h"
#include "../collectibles/fairy_dust_collectible.h"
#include "../collectibles/rainbow_fairy_dust_collectible.h"
#include "../collectibles/dark_fairy_dust_collectible.h"
#include "dying_earth_enemy.h"
#include "magic_missile_enemy.h"
#include "water_wave_enemy.h"
#include "dark_fairy_queen.h"

namespace game {
	Spawner::Spawner(int initial_cost, int cost_increment, GameObject* player, GameObjectData* obj_data, GameObjectData* text_data) {
        data_ = obj_data;
        player_ = player;
		cost_ = initial_cost;
		cost_increment_ = cost_increment;
        started_ = false;
        round_count_ = 0;
        enemy_count_ = 0;
        text_data_ = text_data;
        game_over_ = false;

        min_spawn_dist_ = 2;

        spawn_interval_ = 0.25f;
        can_spawn_ = false;
        spawn_timer_ = new Timer();
        spawn_timer_->Start(spawn_interval_);

        // Insert costs for each enemyt
        enemy_costs_.insert({ 1, "MagicMissileEnemy" });
        enemy_costs_.insert({ 2, "WaterWaveEnemy" });
        enemy_costs_.insert({ 10, "DyingEarthEnemy" });

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
        if (game_over_) return;

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
        Game::GetInstance()->AddObjective(Game::Objective::enemies_killed);
    }

    void Spawner::NextRound() {
        int counter = cost_ + cost_increment_;

        // Add to score for clearing a wave
        Game::GetInstance()->AddObjective(Game::Objective::waves_cleared);

        if(round_count_ == 0) {
            counter -= cost_increment_;
        }

        // Show text for getting weapon
        if (round_count_ == 5) {
            Game::GetInstance()->ShowCenterText("Long Ranged Weapon Unlocked!", "Press 2 to use", 5);
        }

        // Check if it is boss wave or game over
        if(round_count_ == 11) {
            SpawnBoss();
            enemy_count_++;
            return;
        }
        else if (round_count_ > 11) {
            Game::GetInstance()->ShowCenterText("Victory!", "Check the console for final score", 10);
            game_over_ = true;
            return;
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

        // Default enemy data
        MoveData move_data = MoveData(0.5, player_);
        Game* game = Game::GetInstance();
        EnemyGameObject* enemy = nullptr;
        WeaponData* weapon_data = nullptr;
        GameObjectData* projectile_data = nullptr;
        Weapon* weapon = nullptr;
        GameObjectData* enemy_data = nullptr;


        // Choose which enemy to spawn
        if(name == "MagicMissileEnemy") {
            weapon_data = new WeaponData(player_, 2, 3.0f);
            projectile_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_enemy_projectile), 5);
            weapon = new MagicMissileWeapon(*weapon_data, *projectile_data);
            enemy_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_green_ship));
            enemy = new MagicMissileEnemy(GetLocationAroundPlayer(), *enemy_data, 1, move_data, weapon);
        }
        else if(name == "WaterWaveEnemy") {
            MoveData water_wave_move_data = MoveData(2, player_);
            weapon_data = new WeaponData(player_, 2, 5.0f);
            projectile_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_water_projectile), 5);
            weapon = new WaterWaveWeapon(*weapon_data, *projectile_data);
            enemy_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_blue_ship));
            enemy = new WaterWaveEnemy(GetLocationAroundPlayer(), *enemy_data, 1, water_wave_move_data, weapon);
        }
        else if(name == "DyingEarthEnemy") {
            MoveData dying_earth_move_data = MoveData(1, player_);
            enemy_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_dying_earth_enemy));
            enemy = new DyingEarthEnemy(GetLocationAroundPlayer(), *enemy_data, 3, dying_earth_move_data, weapon);
        }

        if(enemy_data && !enemy) {
            enemy = new EnemyGameObject(GetLocationAroundPlayer(), *enemy_data, 1, move_data, weapon);
        }

        if(enemy) {
            game->SpawnGameObject(enemy);
        }
    }

    void Spawner::SpawnBoss() {
        // Teleport player to spawn
        player_->SetPosition(glm::vec3(0, 0, 0));

        // Spawn boss
        GameObjectData* magic_missile_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_enemy_projectile));
        GameObjectData* water_wave_data = new GameObjectData(data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_water_projectile));
        MoveData enemy_move_data = MoveData(0.5, player_);
        EnemyGameObject* queen = new DarkFairyQueen(glm::vec3(0, 3.0f, 0), data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_dark_fairy_queen), 75, enemy_move_data,
                                                    text_data_, magic_missile_data, water_wave_data);
        queen->SetScale(glm::vec2(2, 2));
        Game::GetInstance()->SpawnGameObject(queen);
    }

    void Spawner::SpawnCollectible() {
        CollectibleGameObject* collectible = nullptr;
        Game* game = Game::GetInstance();

        // Spawn collectible based on what collectible count is divisible by
        if(collectible_count % 3 == 0) {
            collectible = new RainbowFairyDustCollectible(GetLocationAroundPlayer(), data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_rainbow_fairy_dust), 10.0f);
        }
        else {
            collectible = new FairyDustCollectible(GetLocationAroundPlayer(), data_->geom_, data_->shader_, Game::GetInstance()->getTexture(Game::tex_fairy_dust), 5.0f);
        }

        if(collectible) {
            game->SpawnGameObject(collectible);
        }

        collectible_count++;
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