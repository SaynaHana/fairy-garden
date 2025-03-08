#ifndef WEAPON_H
#define WEAPON_H

#include <glm/glm.hpp>
#include "../data/weapon_data.h"
#include "../game_object.h"

namespace game {
    class Weapon {
    public:
        Weapon(WeaponData& data, WeaponBehaviour& behaviour_);

        virtual ~Weapon();

        // Setters
        void setParentPosition(glm::vec3& parent_position) { parent_position_ = parent_position; }

        // What needs to happen to execute the attack
        virtual void ExecuteAttack(double delta_time);


    protected:
        WeaponBehaviour& behaviour_;
        glm::vec3 parent_position_;
    };
}

#endif
