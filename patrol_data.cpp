#include "patrol_data.h"

namespace game {
	PatrolData::PatrolData(float width, float height, glm::vec3 center_point) {
		width_ = width;
		height_ = height;
		center_point_ = center_point;
	}

	PatrolData::PatrolData() {
		width_ = 0;
		height_ = 0;
		center_point_ = glm::vec3(0, 0, 0);
	}
} // namespace game