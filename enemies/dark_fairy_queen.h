#ifndef DARK_FAIRY_QUEEN_H
#define DARK_FAIRY_QUEEN_H

#include "enemy_game_object.h"

namespace game {
    class DarkFairyQueen : public EnemyGameObject {
        public:
            DarkFairyQueen(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data,
                           GameObjectData* magic_missile_data);
            DarkFairyQueen(const glm::vec3& position, GameObjectData& data, int health, MoveData& move_data,
                           GameObjectData* magic_missile_data);

            void Update(double delta_time) override;

            // Required to remove object from spawned_objects
            void RemoveSpawnedObject(GameObject* spawned_object);

        private:
            std::vector<GameObject*> spawned_objects_; // The objects that this enemy spawned
            std::vector<int> curr_attacks_;

            void GetRandomAttacks(std::vector<int>& attacks);
            void PerformAttacks();
            void MagicMissileAttack();
            void Reset();

            // Attack data
            GameObjectData* magic_missile_data_;

            int phase_; // Determines how many attacks the enemy will use at once
            float attack_duration_; // How long each attack should last
            Timer* new_attack_timer_; // How long until the enemy should choose new attacks
            bool choose_new_attacks_;
            int num_attacks_;

            int magic_missiles_spawned_;
            int magic_missiles_to_spawn_;
    };
}

#endif
