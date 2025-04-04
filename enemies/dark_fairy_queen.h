#ifndef DARK_FAIRY_QUEEN_H
#define DARK_FAIRY_QUEEN_H

#include "enemy_game_object.h"

namespace game {
    class DarkFairyQueen : public EnemyGameObject {
        public:
            DarkFairyQueen(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data);
            DarkFairyQueen(const glm::vec3& position, GameObjectData& data, int health, MoveData& move_data);

            void Update(double delta_time) override;

        private:
            std::vector<GameObject*> spawned_objects_; // The objects that this enemy spawned
            std::vector<int> curr_attacks_;

            void GetRandomAttacks(std::vector<int>& attacks);

            int phase_; // Determines how many attacks the enemy will use at once
            float attack_duration_; // How long each attack should last
            Timer* new_attack_timer_; // How long until the enemy should choose new attacks
            bool choose_new_attacks_;
            int num_attacks_;
    };
}

#endif
