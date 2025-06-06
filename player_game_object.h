#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "timer.h"
#include "data/move_data.h"
#include "weapons/weapon.h"
#include "collectible_game_object.h"

#define INVINCIBLE_ITEM_COUNT 5
#define INVINCIBLE_DURATION 10
#define SHOOT_COOLDOWN 0.4

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture,
                             GLuint invincible_texture, MoveData& moveData, std::vector<Weapon*> weapons, int health,
                             bool collision_on, float collider_radius, GameObjectData* hit_particle_data);
            PlayerGameObject(const glm::vec3 &position, GameObjectData& objData, GLuint invincible_texture,
                             MoveData& moveData, std::vector<Weapon*> weapons, int health, bool collision_on,
                             GameObjectData* hit_particle_data);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            void OnCollision(GameObject &other) override;

            bool CanCollide(GameObject& other) override;

            void Shoot(const glm::vec3& mouse_pos, double delta_time);

            bool SwitchWeapons(int index);

            int GetWeaponCount() const { return weapons_.size(); }

            void UseCollectible(CollectibleGameObject* collectible);
            void ResetStats();
            void SetAttackSpeedMultiplier(float multiplier) { attack_speed_multiplier = multiplier; }
            void SetInvincible(bool invincible);
            void SetMovementSpeed(int speed) { speed_ = speed; }
            void AddWeapon(Weapon* weapon) { weapons_.push_back(weapon); }

        private:
            // CHANGE: Makes the player not take damage for 10 seconds
            bool invincible_;
            Timer* invincible_timer_;
            GLuint normal_texture_;
            GLuint invincible_texture_;
            float init_speed_;
            float init_collider_radius_;
            float i_frame_duration_;
            glm::vec2 map_boundaries_;

            GameObjectData* hit_particle_data_;

            // CHANGE: Projectiles
            void SetCanShoot(bool can_shoot);
            Timer* projectile_timer_;
            bool can_shoot_;

            std::vector<Weapon*> weapons_;
            Weapon* weapon_;
            float attack_speed_multiplier = 1;
            Timer* collectible_timer_;
            bool collectible_active_ = false;

            void Move(double delta_time) override;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
