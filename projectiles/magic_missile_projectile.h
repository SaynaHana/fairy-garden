//
// Created by aria on 3/4/25.
//

#ifndef MAGIC_MISSILE_PROJECTILE_H
#define MAGIC_MISSILE_PROJECTILE_H

#include "../projectile_game_object.h"

namespace game {
    class MagicMissileProjectile : public Projectile {
        public:
            MagicMissileProjectile();

            void SetTarget(GameObject* target) { target_ = target; }

        protected:
            virtual void Move(double delta_time) override;

        private:
            GameObject* target_;
    };
} // namespace game

#endif
