#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "geometry.h"

#include "timer.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            // CHANGE: Added health and collision_on parameters
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, bool collision_on);

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject 
            virtual void Render(glm::mat4 view_matrix, double current_time);

            // Getters
            inline glm::vec3 GetPosition(void) const { return position_; }
            inline float GetScale(void) const { return scale_; }
            inline float GetRotation(void) const { return angle_; }
            // CHANGE: Added getters for new properties
            inline bool IsCollisionOn(void) const { return collision_on_; }
            inline bool ShouldExplode(void) const { return should_explode_; }
            inline bool ShouldDestroy(void) const { return should_destroy_; }
            inline int GetDamage(void) const { return damage_; }

            // Get bearing direction (direction in which the game object
            // is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(float scale) { scale_ = scale; }
            void SetRotation(float angle);

            // CHANGE: Added function that gets called when this game object collides with another
            virtual void OnCollision(GameObject& other);

            // CHANGE: Added function to select which objects this game object can collide with
            virtual bool CanCollide(GameObject& other);

        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            float scale_;
            float angle_;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;

            // CHANGE
            int health_;
            int damage_;

            // CHANGE: Collisions
            bool collision_on_;
            bool should_explode_;
            bool should_destroy_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
