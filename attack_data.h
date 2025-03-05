#ifndef ATTACK_DATA_H
#define ATTACK_DATA_H

namespace game {
	class AttackData {
	public:
		AttackData(float attack_interval = 1);

		// Getters
		float GetAttackInterval() const { return attack_interval_;  }

	private:
		float attack_interval_;
	};
}

#endif