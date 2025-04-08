#ifndef DARK_FAIRY_QUEEN_H
#define DARK_FAIRY_QUEEN_H

#include "enemy_game_object.h"
#include "../ui/text_game_object.h"

namespace game {
    class DarkFairyQueen : public EnemyGameObject {
        public:
            DarkFairyQueen(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, MoveData& move_data,
                           GameObjectData* text_data, GameObjectData* magic_missile_data, GameObjectData* water_wave_data);
            DarkFairyQueen(const glm::vec3& position, GameObjectData& data, int health, MoveData& move_data,
                           GameObjectData* text_data, GameObjectData* magic_missile_data, GameObjectData* water_wave_data);

            void Update(double delta_time) override;

            void OnCollision(GameObject& other) override;

            void Move(double delta_time) override;

            // Required to remove object from spawned_objects
            void RemoveSpawnedObject(GameObject* spawned_object);

        private:
            std::vector<GameObject*> spawned_objects_; // The objects that this enemy spawned
            std::vector<int> curr_attacks_;
            int max_health_;

            void GetRandomAttacks(std::vector<int>& attacks);
            void PerformAttacks();
            void Reset();
            glm::vec3 GetRandomLocation();

            float min_spawn_dist_;
            float max_spawn_dist_;

            // Attack data
            GameObjectData* magic_missile_data_;
            GameObjectData* water_wave_data_;

            // Magic missiles
            int magic_missiles_spawned_;
            int magic_missiles_to_spawn_;
            float magic_missile_speed_;

            void MagicMissileAttack();

            // Water waves
            float water_wave_spawn_interval_;
            Timer* water_wave_timer_;
            float water_wave_speed_;
            bool performed_water_attack_;
            int water_waves_;

            void WaterWaveAttack();

            int phase_; // Determines how many attacks the enemy will use at once
            float attack_duration_; // How long each attack should last
            Timer* new_attack_timer_; // How long until the enemy should choose new attacks
            bool choose_new_attacks_;
            int num_attacks_;

            // Text to display health
            TextGameObject* health_text_;

            glm::vec3 GetLocationAroundBoss();

            // Wandering
            Timer* wandering_timer_;
            float wander_duration_; // How long the boss wanders in a certain direction
            float wander_angle_; // In which angle can the boss change directions in
    };
}

#endif
