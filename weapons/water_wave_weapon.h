#ifndef WATER_WAVE_WEAPON_H
#define WATER_WAVE_WEAPON_H

#include "interval_weapon.h"
#include "../projectiles/water_wave_projectile.h"

namespace game {
    class WaterWaveWeapon : public IntervalWeapon {
    public:
        WaterWaveWeapon(WeaponData& data, GameObjectData& projectile_data);

        void Attack(double delta_time) override;

    private:
        GameObjectData* projectile_data_;
        float speed_;
    };
}


#endif
