#ifndef SHOTGUN_PLAYER_WEAPON_H
#define SHOTGUN_PLAYER_WEAPON_H

#include "interval_weapon.h"

namespace game {
    class ShotgunWeapon : public IntervalWeapon {
    public:
        ShotgunWeapon(WeaponData& data, GameObjectData& projectile_data, float angle_increment = 5,
                      int half_num_projectiles = 2);
        ~ShotgunWeapon() override;

        void Attack(double delta_time) override;

    private:
        GameObjectData* projectile_data_;
        float speed_;
        float angle_increment_; // How spaced should the projectiles be in degrees
        int half_num_projectiles_;
    };
}


#endif
