#ifndef WEAPON_DATA_H
#define WEAPON_DATA_H

#include "../game_object.h"

namespace game {
    class WeaponData {
    public:
        WeaponData(GameObject& target) {
            target_ = &target;
        }

        // Getters
        GameObject* getTarget() const { return target_; }

    private:
        GameObject* target_;
    };
}


#endif
