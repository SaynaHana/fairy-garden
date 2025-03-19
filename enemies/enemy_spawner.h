#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <unordered_map>
#include "enemy_game_object.h"
#include "../weapons/magic_missile_weapon.h"
#include "../projectiles/magic_missile_projectile.h"
#include "../weapons/water_wave_weapon.h"
#include "../projectiles/water_wave_projectile.h"
#include "../game.h"

namespace game {
	class EnemySpawner {
	public:
		EnemySpawner(int initial_cost = 1, int cost_increment_ = 3, GameObject* player = nullptr, GameObjectData& data);
		
	private:
		std::unordered_map<GameObject*, int> enemy_costs_; // Stores how much each enemy costs to spawn
		int cost_;
		int cost_increment_; // How much does cost go up each round
	};
}

#endif