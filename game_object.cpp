#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "game_object.h"

namespace game {

GameObject::GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, bool collision_on)
{

    // Initialize all attributes
    position_ = position;
    scale_ = glm::vec2(1.0, 1.0);
    angle_ = 0.0;
    geometry_ = geom;
    shader_ = shader;
    texture_ = texture;
    health_ = health;
    collision_on_ = collision_on;
    should_explode_ = true;
    should_destroy_ = false;
    damage_ = 1;
    velocity_ = glm::vec3(0, 0, 0);
    acceleration_ = glm::vec3(0, 0, 0);
    speed_ = 2;
    collision_type_ = CollisionType::circle_;
}

glm::vec3 GameObject::GetBearing(void) const {

    glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
    return dir;
}


glm::vec3 GameObject::GetRight(void) const {

    float pi_over_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(angle_ - pi_over_two), sin(angle_ - pi_over_two), 0.0);
    return dir;
}


void GameObject::SetRotation(float angle){ 

    // Set rotation angle of the game object
    // Make sure angle is in the range [0, 2*pi]
    float two_pi = 2.0f*glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0){
        angle += two_pi;
    }
    angle_ = angle;
}

void GameObject::Update(double delta_time) {
    velocity_ = velocity_ + acceleration_ * (float)delta_time;

    if(glm::length(velocity_) > speed_) {
        velocity_ = speed_ * glm::normalize(velocity_);
    }

    if(glm::length(velocity_) > 0) {
        SetPosition(position_ + velocity_ * (float)delta_time);
    }
}


void GameObject::Render(glm::mat4 view_matrix, double current_time){

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

void GameObject::OnCollision(GameObject& other) {
    health_ -= other.GetDamage();

    if (health_ <= 0 && collision_on_) {
		collision_on_ = false;
        should_destroy_ = true;
    }
}

bool GameObject::CanCollide(GameObject& other) {
    return collision_on_;
}

} // namespace game
