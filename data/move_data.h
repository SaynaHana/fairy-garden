#include "../game_object.h"
#ifndef MOVE_DATA_H
#define MOVE_DATA_H

namespace game {
	class MoveData {
		public:
			MoveData(float speed, GameObject* target);
			MoveData();

			inline float GetSpeed() const { return speed_; }
			inline GameObject* GetTarget() const { return target_; }

		private:
			float speed_;
			GameObject* target_;
	};
}

#endif
