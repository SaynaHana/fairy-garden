#ifndef DEFAULT_PLAYER_WEAPON_H
#define DEFAULT_PLAYER_WEAPON_H

#include "interval_weapon.h"
#include "../data/game_object_data.h"

namespace game {
    class DefaultPlayerWeapon : public IntervalWeapon {
    public:
        DefaultPlayerWeapon(WeaponData& data, GameObjectData& projectile_data, GameObjectData* particle_data);
        ~DefaultPlayerWeapon() override;

        void Attack(double delta_time) override;

    private:
        GameObjectData* projectile_data_;
        float speed_;
        GameObjectData* particle_data_;
    };
}

#endif
