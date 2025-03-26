#include "dark_fairy_dust_collectible.h"
#include "../player_game_object.h"

namespace game {
    DarkFairyDustCollectible::DarkFairyDustCollectible(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                                             GLuint texture, float duration)
            : CollectibleGameObject(position, geom, shader, texture, duration) {

    }

    void DarkFairyDustCollectible::OnConsumed(GameObject& player) {
        PlayerGameObject* playerObj = dynamic_cast<PlayerGameObject*>(&player);

        if(!playerObj) return;

        playerObj->SetHealth(playerObj->GetHealth() + 1);
        playerObj->SetInvincible(true);
        playerObj->SetMovementSpeed(5);
        playerObj->SetColliderRadius(3);
    }
}
