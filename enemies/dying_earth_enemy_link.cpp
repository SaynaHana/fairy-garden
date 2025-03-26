#include "dying_earth_enemy_link.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

namespace game {
    DyingEarthEnemyLink::DyingEarthEnemyLink(const glm::vec3& position, Geometry* geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon)
            : GameObject(position, geom, shader, texture, 1, true) {
    }

    DyingEarthEnemyLink::DyingEarthEnemyLink(const glm::vec3 &position, GameObjectData &data, int health, MoveData &move_data, Weapon* weapon)
            : DyingEarthEnemyLink(position, data.geom_, data.shader_, data.texture_, health, move_data, weapon) {

    }

    void DyingEarthEnemyLink::Move(const glm::mat4& parent_transformation) {
        glm::mat4 translate = glm::mat4(1);
        translate = glm::translate(translate, glm::vec3(1.0f, 0, 0));

        glm::mat4 transformation = glm::mat4(1);
        transformation = parent_transformation * translate;

        SetPosition(transformation[3]);

        for(int i = 0; i < children_.size(); i++) {
            ((DyingEarthEnemyLink*)children_[i])->Move(transformation);
        }
    }

    bool DyingEarthEnemyLink::CanCollide(game::GameObject &other) {
        return false;
    }
}
