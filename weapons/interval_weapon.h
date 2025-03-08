#ifndef INTERVAL_WEAPON_H
#define INTERVAL_WEAPON_H

#include "weapon.h"
#include "../timer.h"

namespace game {
    class IntervalWeapon : public Weapon {
    public:
        IntervalWeapon(WeaponData& data, WeaponBehaviour& behaviour);
        virtual ~IntervalWeapon();

        virtual void ExecuteAttack(double delta_time) override;

    private:
        Timer* attack_timer_;
        float attack_interval_;
        bool can_attack_;
    };
}

#endif
