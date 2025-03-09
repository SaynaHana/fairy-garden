#include "weapon.h"

namespace game {
    Weapon::Weapon(WeaponData& data) {
        target_ = data.GetTarget();
    }

    Weapon::~Weapon() {

    }

    void Weapon::ExecuteAttack(double delta_time, glm::vec3 parent_position, glm::vec3 parent_bearing) {
        parent_position_ = parent_position;
        parent_bearing_ = parent_bearing;
    }
}