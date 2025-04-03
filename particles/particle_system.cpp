#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "particle_system.h"
#include <iostream>


namespace game {

ParticleSystem::ParticleSystem(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GameObject *parent,
                               const glm::vec3& color, float lifetime, bool finite_lifetime)
	: GameObject(position, geom, shader, texture, 0, false){
    lifetime_ = lifetime;
    finite_lifetime_ = finite_lifetime;
    parent_ = parent;
    should_explode_ = false;
    color_ = color;

    if(finite_lifetime_) {
        lifetime_timer_ = new Timer();
        lifetime_timer_->Start(lifetime_);
    }

    tags.insert("ParticleSystem");
}


void ParticleSystem::Update(double delta_time) {

    // Call the parent's update method to move the object in standard way, if desired
    GameObject::Update(delta_time);

    // If there is finite lifetime, then count down
    if(lifetime_timer_) {
        if(lifetime_timer_->Finished()) {
            should_destroy_ = true;
        }
    }
}


void ParticleSystem::Render(glm::mat4 view_matrix, double current_time){

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);
    shader_->SetUniform3f("base_color", color_);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0f));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    //glm::mat4 translation_matrix = glm::mat4(1);
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0), position_);

    // Set up the parent transformation matrix
    glm::mat4 parent_rotation_matrix = glm::mat4(1);
    glm::mat4 parent_translation_matrix = glm::mat4(1);
    glm::mat4 parent_transformation_matrix = glm::mat4(1);
    if(parent_) {
        parent_rotation_matrix = glm::rotate(glm::mat4(1.0f), parent_->GetRotation(), glm::vec3(0.0, 0.0, 1.0));
        parent_translation_matrix = glm::translate(glm::mat4(1.0f), parent_->GetPosition());
        parent_transformation_matrix = parent_translation_matrix * parent_rotation_matrix;
    }
    else {
        // Set up the translation matrix for the shader
        //translation_matrix = glm::translate(glm::mat4(1.0f), position_);
    }

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = parent_transformation_matrix * translation_matrix * rotation_matrix * scaling_matrix;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set the time in the shader
    shader_->SetUniform1f("time", current_time);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the particle texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
