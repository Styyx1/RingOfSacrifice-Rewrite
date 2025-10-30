#pragma once

namespace StressHandler {
	struct Handler : public REX::Singleton<Handler> {
		void ApplyStressOnDeath(float stress_increase_percentage, float min_amount_applied) const;
		float GetStressLevel() const;
		bool CanApplyStress() const;
	};
}