#ifndef WEAPON_DATA_H
#define WEAPON_DATA_H

#include "../game_object.h"
#include "../weapon_behaviour/weapon_behaviour.h"

namespace game {
    class WeaponData {
    public:
        WeaponData(GameObject& target, WeaponBehaviour& behaviour, float attack_interval)
        : behaviour_(behaviour) {
            target_ = &target;
            attack_interval_ = attack_interval;
        }

        // Getters
        GameObject* getTarget() const { return target_; }
        WeaponBehaviour& getBehaviour() const { return behaviour_; }
        float getAttackInterval() const { return attack_interval_; }

    private:
        GameObject* target_;

        WeaponBehaviour& behaviour_;

        float attack_interval_;
    };
}


#endif
