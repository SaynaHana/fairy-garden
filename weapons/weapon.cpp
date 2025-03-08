#include "weapon.h"

namespace game {
    Weapon::Weapon(WeaponData &data) : behaviour_(data.getBehaviour()) {
        target_ = data.getTarget();
    }

    Weapon::~Weapon() {

    }
}