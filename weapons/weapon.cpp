#include "weapon.h"

namespace game {
    Weapon::Weapon(WeaponData& data, WeaponBehaviour& behaviour) : behaviour_(behaviour) {

    }

    Weapon::~Weapon() {

    }

    void Weapon::ExecuteAttack(double delta_time) {

    }
}