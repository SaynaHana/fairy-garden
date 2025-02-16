#ifndef PATROL_DATA_H
#define PATROL_DATA_H

#include <glm/glm.hpp>

namespace game {
	class PatrolData {
		public:
			PatrolData(float width, float height, glm::vec3 center_point);
			PatrolData();

			inline float GetWidth() const { return width_; }
			inline float GetHeight() const { return height_; }
			inline glm::vec3 GetCenterPoint() const { return center_point_; }

		private:
			float width_;
			float height_;
			glm::vec3 center_point_;
	};
} // namespace game

#endif

