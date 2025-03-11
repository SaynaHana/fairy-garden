#include "interval_weapon.h"

namespace game {
    IntervalWeapon::IntervalWeapon(WeaponData &data, bool can_attack) : Weapon(data) {
        attack_interval_ = data.GetAttackInterval();
        can_attack_ = can_attack;
        attack_timer_ = new Timer();
        attack_timer_->Start(attack_interval_);
    }

    IntervalWeapon::~IntervalWeapon() {
        delete attack_timer_;
    }

    void IntervalWeapon::ExecuteAttack(double delta_time, glm::vec3 parent_position, glm::vec3 parent_bearing) {
        Weapon::ExecuteAttack(delta_time, parent_position, parent_bearing);

        if(can_attack_) {
            Attack(delta_time);
            can_attack_ = false;

            if(!attack_timer_) return;
            attack_timer_->Start(attack_interval_);
        }
        else {
            if(attack_timer_) {
                if(attack_timer_->Finished()) {
                    can_attack_ = true;
                }
            }
        }
    }
}
