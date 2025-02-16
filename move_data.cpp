#include "move_data.h"

namespace game {
	MoveData::MoveData(float speed, GameObject* target, float detection_range) {
		speed_ = speed;
		target_ = target;
		detection_range_ = detection_range;
	}

	MoveData::MoveData() {
		speed_ = 0;
		target_ = nullptr;
		detection_range_ = 0;
	}
}