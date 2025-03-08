#ifndef WEAPON_H
#define WEAPON_H

#include "../data/weapon_data.h"
#include "../game_object.h"

namespace game {
    class Weapon {
    public:
        Weapon(WeaponData& data);

        virtual ~Weapon();

        // What needs to happen to execute the attack
        virtual void ExecuteAttack(double delta_time) = 0;

    protected:
        GameObject* target_;
        WeaponBehaviour& behaviour_;
    };
}

#endif
