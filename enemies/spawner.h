#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <unordered_map>
#include <string>
#include <queue>
#include "enemy_game_object.h"
#include "../weapons/magic_missile_weapon.h"
#include "../projectiles/magic_missile_projectile.h"
#include "../weapons/water_wave_weapon.h"
#include "../projectiles/water_wave_projectile.h"

namespace game {
	class Spawner {
	public:
		Spawner(int initial_cost = 1, int cost_increment_ = 3, GameObject* player = nullptr, GameObjectData* data = nullptr);

        bool Start();
        void Update(double delta_time);
        void OnEnemyDeath();

	private:
		std::unordered_map<int, std::string> enemy_costs_; // Stores how much each enemy costs to spawn
        GameObjectData* data_;
        GameObject* player_;
		int cost_;
		int cost_increment_; // How much does cost go up each round
        bool started_;
        int round_count_;
        int enemy_count_; // How many enemies are currently alive
        float min_spawn_dist_;
        float spawn_interval_;
        bool can_spawn_;
        Timer* spawn_timer_;
        std::queue<std::string> spawn_queue_;

        void SpawnEnemy(const std::string& name);
        void NextRound(); // Start next round
        glm::vec3 GetLocationAroundPlayer();
	};
}

#endif