#ifndef WEAPON_DATA_H
#define WEAPON_DATA_H

#include "../game_object.h"

namespace game {
    class WeaponData {
    public:
        WeaponData(GameObject* target, float speed, float attack_interval) {
            target_ = target;
            speed_ = speed;
            attack_interval_ = attack_interval;
        }

        // Getters
        GameObject* GetTarget() const { return target_; }
        float GetSpeed() const { return speed_; };
        float GetAttackInterval() const { return attack_interval_; }

    private:
        GameObject* target_;
        float speed_;
        float attack_interval_;
    };
}


#endif
