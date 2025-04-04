#include "dark_fairy_queen.h"
#include <GLFW/glfw3.h>

namespace game {
    DarkFairyQueen::DarkFairyQueen(const glm::vec3 &position, game::Geometry *geom, game::Shader *shader,
                                     GLuint texture, int health, game::MoveData &move_data)
            : EnemyGameObject(position, geom, shader, texture, health, move_data, nullptr) {
        phase_ = 1;
        attack_duration_ = 10.0f;
        new_attack_timer_ = new Timer();
        new_attack_timer_->Start(attack_duration_);
        choose_new_attacks_ = true;
        num_attacks_ = 1;
    }

    DarkFairyQueen::DarkFairyQueen(const glm::vec3 &position, game::GameObjectData &data, int health,
                                     game::MoveData &move_data)
            : DarkFairyQueen(position, data.geom_, data.shader_, data.texture_, health, move_data) {
    }

    void DarkFairyQueen::Update(double delta_time) {
        if(choose_new_attacks_) {
            curr_attacks_.clear();
            GetRandomAttacks(curr_attacks_);
            choose_new_attacks_ = false;
        }

        if(new_attack_timer_) {
            if(new_attack_timer_->Finished()) {
                choose_new_attacks_ = true;
                new_attack_timer_->Start(attack_duration_);
            }
        }
    }

    void DarkFairyQueen::GetRandomAttacks(std::vector<int>& attacks) {
        // Choose random numbers
        srand(glfwGetTime());
        for(int i = 0; i < phase_; i++) {
            int attack = rand() % num_attacks_ + 1;
            attacks.push_back(attack);
        }
    }
}