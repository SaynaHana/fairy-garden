#ifndef WEAPON_H
#define WEAPON_H

#include <glm/glm.hpp>
#include "../data/weapon_data.h"
#include "../game_object.h"
#include <iostream>

namespace game {
    class Weapon {
    public:
        explicit Weapon(WeaponData& data);

        virtual ~Weapon();

        // What needs to happen to execute the attack
        virtual void ExecuteAttack(double delta_time, glm::vec3 parent_position, glm::vec3 parent_bearing_);

        virtual void Attack(double delta_time) = 0;


    protected:
        glm::vec3 parent_position_{};
        glm::vec3 parent_bearing_{};
        GameObject* target_;
    };
}

#endif
