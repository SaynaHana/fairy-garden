#include "dying_earth_enemy.h"
#include "dying_earth_enemy_link.h"
#include "../game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace game {
    DyingEarthEnemy::DyingEarthEnemy(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                     GLuint texture, int health, game::MoveData &move_data, game::Weapon *weapon)
                                     : EnemyGameObject(position, geom, shader, texture, health, move_data, weapon) {
        swing_angle_ = 0;
        reverse_swing_ = false;
        SetupLinks();
        chase_range_ = 1;
        tags.insert("DyingEarthEnemy");
        flee_speed_ = 0.5f;
    }

    DyingEarthEnemy::DyingEarthEnemy(const glm::vec3 &position, game::GameObjectData &data, int health,
                                     game::MoveData &move_data, game::Weapon *weapon)
                                     : DyingEarthEnemy(position, data.geom_, data.shader_, data.texture_, health, move_data, weapon) {

    }

    void DyingEarthEnemy::Update(double delta_time) {
        if (should_destroy_) return;

        Move(delta_time);

        if (!HasChildren()) {
        }

        // Rotate the enemy
        // Cycle between going counter-clockwise and clockwise
        if(!reverse_swing_) {
            swing_angle_ += delta_time * swing_speed_;
        }
        else {
            swing_angle_ -= delta_time * swing_speed_;
        }

        if(swing_angle_ > 3.14f) {
            reverse_swing_ = true;
        }
        else if(swing_angle_ < 0) {
            reverse_swing_ = false;
        }

        // Get angle to player
        float player_angle_ = 0;

        if(target_ != nullptr) {
            glm::vec3 diff = target_->GetPosition() - GetPosition();

            if(glm::length(diff) != 0) {
                diff = glm::normalize(diff);
            }

            player_angle_ = std::atan(diff.y / diff.x);

            if(diff.x < 0) {
                player_angle_ += 3.14f;
            }
        }

        glm::mat4 translate = glm::mat4(1);
        translate = glm::translate(translate, position_);
        glm::mat4 rotate = glm::mat4(1);
        rotate = glm::rotate(rotate, swing_angle_ + player_angle_ - (3.14f / 2), glm::vec3(0, 0, 1));

        glm::mat4 transformation = glm::mat4(1);
        transformation = translate * rotate;


        for(int i = 0; i < children_.size(); i++) {
            if (!children_[i]) continue;
            ((DyingEarthEnemyLink*)children_[i])->Move(transformation, delta_time);
        }
    }

    void DyingEarthEnemy::Move(double delta_time) {
        if (target_ == nullptr) return;

        // If the enemy still has links, move towards player,
        // Otherwise flee and shoot magic missiles every few seconds
        if (HasChildren()) {
            glm::vec3 diff = target_->GetPosition() - GetPosition();

            
            if (glm::length(diff) > chase_range_) {
                if (glm::length(diff) != 0) {
                    diff = glm::normalize(diff);
                }

                SetPosition(GetPosition() + diff * speed_ * (float)delta_time);
            }
        }
        else {
            // Flee
            glm::vec3 diff = GetPosition() - target_->GetPosition();

            if (glm::length(diff) != 0) {
                diff = glm::normalize(diff);
            }

            SetPosition(GetPosition() + diff * flee_speed_ * (float)delta_time);
        }
    }

    void DyingEarthEnemy::SetupLinks() {
        // First link setup
        GameObjectData* data = new GameObjectData(geometry_, shader_, Game::GetInstance()->getTexture(Game::tex_dying_earth_link));
        MoveData* link_one_move_data = new MoveData(2, nullptr);
        DyingEarthEnemyLink* firstLink = new DyingEarthEnemyLink(position_, *data, 1, *link_one_move_data, nullptr);
        firstLink->SetParent(this);
        AddChild(firstLink);
        Game::GetInstance()->SpawnGameObject(firstLink);

        // Second link setup
        MoveData* link_two_move_data = new MoveData(3, nullptr);
        DyingEarthEnemyLink* secondLink = new DyingEarthEnemyLink(position_, *data, 1, *link_two_move_data, nullptr);
        secondLink->SetParent(firstLink);
        firstLink->AddChild(secondLink);
        Game::GetInstance()->SpawnGameObject(secondLink);

        MoveData* link_three_move_data = new MoveData(4, nullptr);
        DyingEarthEnemyLink* thirdLink = new DyingEarthEnemyLink(position_, *data, 1, *link_three_move_data, nullptr);
        thirdLink->SetParent(secondLink);
        secondLink->AddChild(thirdLink);
        Game::GetInstance()->SpawnGameObject(thirdLink);
    }

    void DyingEarthEnemy::OnCollision(game::GameObject &other) {
        for(int i = 0; i < children_.size(); i++) {
            if (!children_[i]) continue;
            ((DyingEarthEnemyLink*)children_[i])->DestroyChildren();
            children_[i] = nullptr;
        }

        GameObject::OnCollision(other);
    }

    bool DyingEarthEnemy::HasChildren() {
        for (int i = 0; i < children_.size(); i++) {
            if (!children_[i]) return false;
        }

        return true;
    }
}
