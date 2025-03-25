#include "fairy_dust_collectible.h"
#include "../player_game_object.h"

namespace game {
    void FairyDustCollectible::OnConsumed(GameObject& player) {
        PlayerGameObject* playerObj = dynamic_cast<PlayerGameObject*>(&player);

        if(!playerObj) return;

        playerObj->SetAttackSpeedMultiplier(2);
    }
}
