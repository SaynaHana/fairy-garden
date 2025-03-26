#include "dying_earth_enemy.h"
#include "dying_earth_enemy_link.h"
#include "../game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace game {
    DyingEarthEnemy::DyingEarthEnemy(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                     GLuint texture, int health, game::MoveData &move_data, game::Weapon *weapon)
                                     : EnemyGameObject(position, geom, shader, texture, health, move_data, weapon) {
        SetupLinks();
    }

    DyingEarthEnemy::DyingEarthEnemy(const glm::vec3 &position, game::GameObjectData &data, int health,
                                     game::MoveData &move_data, game::Weapon *weapon)
                                     : DyingEarthEnemy(position, data.geom_, data.shader_, data.texture_, health, move_data, weapon) {

    }

    void DyingEarthEnemy::Update(double delta_time) {
        EnemyGameObject::Update(delta_time);

        glm::mat4 transformation = glm::mat4(1);
        transformation = glm::translate(transformation, position_);

        for(int i = 0; i < children_.size(); i++) {
            ((DyingEarthEnemyLink*)children_[i])->Move(transformation);
        }
    }

    void DyingEarthEnemy::SetupLinks() {
        // First link setup
        GameObjectData* data = new GameObjectData(geometry_, shader_, Game::GetInstance()->getTexture(Game::tex_dark_fairy_dust));
        MoveData* moveData = new MoveData(2, nullptr);
        DyingEarthEnemyLink* firstLink = new DyingEarthEnemyLink(position_, *data, 1, *moveData, nullptr);
        firstLink->SetParent(this);
        AddChild(firstLink);
        Game::GetInstance()->SpawnGameObject(firstLink);

        // Second link setup
        DyingEarthEnemyLink* secondLink = new DyingEarthEnemyLink(position_, *data, 1, *moveData, nullptr);
        secondLink->SetParent(firstLink);
        firstLink->AddChild(secondLink);
        Game::GetInstance()->SpawnGameObject(secondLink);
    }
}
