#ifndef FAIRY_DUST_COLLECTIBLE_H
#define FAIRY_DUST_COLLECTIBLE_H

#include "../collectible_game_object.h"

namespace game {
    class FairyDustCollectible : public CollectibleGameObject {
    public:
        void OnConsumed(GameObject& player) override;

    private:

    };
}


#endif
