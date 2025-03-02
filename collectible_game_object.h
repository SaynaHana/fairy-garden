#ifndef COLLECTIBLE_GAME_OBJECT_H
#define COLLECTIBLE_GAME_OBJECT_H
#include "game_object.h"
#include <iostream>


namespace game {
    class CollectibleGameObject : public GameObject {
        public:
            CollectibleGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            void Update(double delta_time) override;

            void Render(glm::mat4 view_matrix, double current_time) override;

            void OnCollision(GameObject& other) override;

            bool CanCollide(game::GameObject &other) override;

        private:
            // CHANGE: Added collected flag to set ghost shader
            bool collected;
    };
} // namespace game



#endif //COLLECTIBLE_GAME_OBJECT_H
