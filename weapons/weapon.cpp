#include "weapon.h"

namespace game {
    Weapon::Weapon(WeaponData &data) {
        target_ = data.getTarget();
    }

    Weapon::~Weapon() {

    }

    void Weapon::ExecuteAttack(double delta_time) {
        Attack(delta_time);
    }
}