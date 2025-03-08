#include "interval_weapon.h"

namespace game {
    IntervalWeapon::IntervalWeapon(game::WeaponData &data) : Weapon(data) {
        attack_interval_ = data.getAttackInterval();
        can_attack_ = true;
        attack_timer_ = new Timer();
    }

    IntervalWeapon::~IntervalWeapon() {
        delete attack_timer_;
    }

    void IntervalWeapon::ExecuteAttack(double delta_time) {
        if(can_attack_) {
            behaviour_.Execute(delta_time);
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
