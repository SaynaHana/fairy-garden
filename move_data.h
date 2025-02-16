#include "game_object.h"
#ifndef MOVE_DATA_H
#define MOVE_DATA_H

namespace game {
	class MoveData {
		public:
			MoveData(float speed, GameObject* target, float detection_range);
			MoveData();

			inline float GetSpeed() const { return speed_; }
			inline GameObject* GetTarget() const { return target_; }
			inline float GetDetectionRange() const { return detection_range_; }

		private:
			float speed_;
			GameObject* target_;
			float detection_range_; // Radius to detect when this object should move towards target
	};
}

#endif
