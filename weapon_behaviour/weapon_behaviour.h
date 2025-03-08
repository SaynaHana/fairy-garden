#ifndef WEAPON_BEHAVIOUR_H
#define WEAPON_BEHAVIOUR_H

namespace game {
    class WeaponBehaviour {
    public:
        WeaponBehaviour();
        virtual ~WeaponBehaviour();

        virtual void Execute(double delta_time) = 0;
    };
}


#endif
