#include "move_data.h"

namespace game {
	MoveData::MoveData(float speed, GameObject* target) {
		speed_ = speed;
		target_ = target;
	}

	MoveData::MoveData() {
		speed_ = 0;
		target_ = nullptr;
	}
}