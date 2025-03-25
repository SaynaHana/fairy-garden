#include "fairy_dust_collectible.h"
#include "../player_game_object.h"

namespace game {
    FairyDustCollectible::FairyDustCollectible(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                               GLuint texture, float duration)
        : CollectibleGameObject(position, geom, shader, texture, duration) {

    }

    void FairyDustCollectible::OnConsumed(GameObject& player) {
        PlayerGameObject* playerObj = dynamic_cast<PlayerGameObject*>(&player);

        if(!playerObj) return;

        playerObj->SetAttackSpeedMultiplier(1.5f);
        playerObj->SetHealth(playerObj->GetHealth() + 1);
    }
}
