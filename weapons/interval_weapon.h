#ifndef INTERVAL_WEAPON_H
#define INTERVAL_WEAPON_H

#include "weapon.h"
#include "../timer.h"

namespace game {
    class IntervalWeapon : public Weapon {
    public:
        explicit IntervalWeapon(WeaponData& data);
        ~IntervalWeapon() override;

        void ExecuteAttack(double delta_time, glm::vec3 parent_position, glm::vec3 parent_bearing) override;

        void Attack(double delta_time) override = 0;

    private:
        Timer* attack_timer_;
        float attack_interval_;
        bool can_attack_;
    };
}

#endif
