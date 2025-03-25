#ifndef RAINBOW_FAIRY_DUST_COLLECTIBLE_H
#define RAINBOW_FAIRY_DUST_COLLECTIBLE_H

#include "../collectible_game_object.h"

namespace game {
    class RainbowFairyDustCollectible : public CollectibleGameObject {
    public:
        RainbowFairyDustCollectible(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, float duration = 5.0f);
        void OnConsumed(GameObject& player) override;
    };
}

#endif
