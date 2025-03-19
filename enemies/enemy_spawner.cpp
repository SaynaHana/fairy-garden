#include "enemy_spawner.h"

namespace game {
	EnemySpawner::EnemySpawner(int initial_cost, int cost_increment, GameObject* player, GameObjectData& objData) {
		cost_ = initial_cost;
		cost_increment_ = cost_increment;

		// Create enemy types
		MoveData move_data = MoveData(0.5, player);
		WeaponData* weapon_data = new WeaponData(player, 2, 1.75f);
		GameObjectData* magic_missile_data = new GameObjectData(objData.geom_, objData.shader_, Game::GetInstance()->getTexture(Game::tex_enemy_projectile), 5);
		MagicMissileWeapon* magic_missile_weapon = new MagicMissileWeapon(*weapon_data, *magic_missile_data);
		GameObjectData enemy_data = GameObjectData(objData.geom_, objData.shader_, Game::GetInstance()->getTexture(Game::tex_green_ship));
		auto* magic_missile_enemy = new EnemyGameObject(glm::vec3(2.0, 1.0, 0.0), enemy_data, 2, move_data, magic_missile_weapon);

		enemy_costs_.insert({ magic_missile_enemy, 1 });
	}

	
}