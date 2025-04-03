#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include "data/game_object_data.h"

#include "shader.h"
#include "geometry.h"

#include "timer.h"

#include "Constants.h"

#include <string>

#include <vector>
#include <unordered_set>

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            // CHANGE: Added health and collision_on parameters
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, int health, bool collision_on, float collider_radius = 0.5f, float speed = 2);
            GameObject(const glm::vec3& position, GameObjectData& data, int health, bool collision_on);

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject 
            virtual void Render(glm::mat4 view_matrix, double current_time);

            // Getters
            inline glm::vec3 GetPosition(void) const { return position_; }
            inline glm::vec2 GetScale(void) const { return scale_; }
            inline float GetRotation(void) const { return angle_; }
            // CHANGE: Added getters for new properties
            inline bool IsCollisionOn(void) const { return collision_on_; }
            inline bool ShouldExplode(void) const { return should_explode_; }
            inline bool ShouldDestroy(void) const { return should_destroy_; }
            inline int GetDamage(void) const { return damage_; }
            inline int GetCollisionType(void) const { return collision_type_; }
            inline float GetColliderRadius(void) const { return collider_radius_; }
            int GetHealth() const { return health_; }
            GameObject* GetParent() const { return parent_; }

            // Get bearing direction (direction in which the game object
            // is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

            bool HasTag(const std::string& tag) const {
                return tags.find(tag) != tags.end();
            }

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(glm::vec2 scale) { scale_ = scale; }
            void SetRotation(float angle);
            // CHANGE: Added SetAcceleration function for physically based motion
            inline void SetVelocity(glm::vec3 velocity) { velocity_ = velocity; }
            inline void SetAcceleration(glm::vec3 acceleration) { acceleration_ = acceleration; };
            inline void SetSpeed(float speed) { speed_ = speed; }
            void SetHealth(int health) {
                if(health >= 5) return;

                health_ = health;
            }
            void SetColliderRadius(float size) { collider_radius_ = size; }
            void SetParent(GameObject* parent) { parent_ = parent; }
            void AddChild(GameObject* child) { children_.push_back(child); }
            void SetShouldDestroy(bool should_destroy) { should_destroy_ = should_destroy; }

            // CHANGE: Added function that gets called when this game object collides with another
            virtual void OnCollision(GameObject& other);

            // CHANGE: Added function to select which objects this game object can collide with
            virtual bool CanCollide(GameObject& other);


        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            // CHANGE: Changed scale_ from float to vec2
            glm::vec2 scale_;
            float angle_;
            glm::vec3 velocity_;
            glm::vec3 acceleration_;
            float speed_;

            std::vector<GameObject*> children_;
            GameObject* parent_ = nullptr;

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
            CollisionType collision_type_;
            float collider_radius_;

            // Tags, used for identifying object types without having to cast them
            std::unordered_set<std::string> tags;

            virtual void Move(double delta_time);

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
