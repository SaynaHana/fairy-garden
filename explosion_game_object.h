//
// Created by Aerisia on 25/01/25.
//

#ifndef EXPLOSION_GAME_OBJECT_H
#define EXPLOSION_GAME_OBJECT_H
#include "game_object.h"

namespace game {
    class ExplosionGameObject : public GameObject {
        public:
            // CHANGE: Constructor that makes explosion disappear after certain amount of time
            ExplosionGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int duration);

            void Update(double delta_time) override;

        protected:
            int duration_; // Time until this game object disappears
            Timer* timer_;
    };

} // namespace game




#endif //EXPLOSION_GAME_OBJECT_H
