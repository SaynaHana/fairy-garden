#ifndef DARK_FAIRY_DUST_COLLECTIBLE_H
#define DARK_FAIRY_DUST_COLLECTIBLE_H

#include "../collectible_game_object.h"

namespace game {
    class DarkFairyDustCollectible : public CollectibleGameObject {
        public:
            DarkFairyDustCollectible(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, float duration = 5.0f);
            void OnConsumed(GameObject& player) override;
    };
}

#endif
