#include "dying_earth_enemy_link.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

namespace game {
    DyingEarthEnemyLink::DyingEarthEnemyLink(const glm::vec3& position, Geometry* geom, Shader *shader, GLuint texture, int health, MoveData& move_data, Weapon* weapon)
            : GameObject(position, geom, shader, texture, 1, true, 0.5f, move_data.GetSpeed()) {
        tags.insert("CanDamagePlayer");
        reverse_swing_ = false;
        swing_speed_ = move_data.GetSpeed();
        swing_angle_ = 0;
    }

    DyingEarthEnemyLink::DyingEarthEnemyLink(const glm::vec3 &position, GameObjectData &data, int health, MoveData &move_data, Weapon* weapon)
            : DyingEarthEnemyLink(position, data.geom_, data.shader_, data.texture_, health, move_data, weapon) {
    }

    void DyingEarthEnemyLink::Move(const glm::mat4& parent_transformation, double delta_time) {
        if(!reverse_swing_) {
            swing_angle_ += delta_time * swing_speed_;
        }
        else {
            swing_angle_ -= delta_time * swing_speed_;
        }

        if(swing_angle_ > 3.14f / 2.0f) {
            reverse_swing_ = true;
        }
        else if(swing_angle_ < 0) {
            reverse_swing_ = false;
        }

        glm::mat4 tr1 = glm::translate(glm::mat4(1), glm::vec3(-1, 0, 0));
        glm::mat4 rot = glm::rotate(glm::mat4(1), swing_angle_, glm::vec3(0, 0, 1));
        glm::mat4 tr2 = glm::translate(glm::mat4(1), glm::vec3(1, 0, 0));

        glm::mat4 transformation = glm::mat4(1);
        transformation = parent_transformation * tr2 * rot * tr1;

        SetPosition(glm::vec3(transformation[3][0], transformation[3][1], transformation[3][2]));

        for(int i = 0; i < children_.size(); i++) {
            ((DyingEarthEnemyLink*)children_[i])->Move(transformation, delta_time);
        }
    }

    bool DyingEarthEnemyLink::CanCollide(game::GameObject &other) {
        if(!GameObject::CanCollide(other)) return false;
        if(other.HasTag("PlayerGameObject")) return true;

        return false;
    }

    void DyingEarthEnemyLink::OnCollision(game::GameObject &other) {
    }

    void DyingEarthEnemyLink::DestroyChildren() {
        for(int i = 0; i < children_.size(); i++) {
            ((DyingEarthEnemyLink*)children_[i])->DestroyChildren();
        }

        should_destroy_ = true;
    }
}
