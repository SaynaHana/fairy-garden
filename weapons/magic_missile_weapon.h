#ifndef MAGIC_MISSILE_WEAPON_H
#define MAGIC_MISSILE_WEAPON_H

#include "interval_weapon.h"

namespace game {
    class MagicMissileWeapon : public IntervalWeapon {
    public:
        MagicMissileWeapon(WeaponData& data, GameObjectData& projectileData);

        void Attack(double delta_time) override;

    private:
        GameObjectData* projectileData_;
    };
}

#endif
