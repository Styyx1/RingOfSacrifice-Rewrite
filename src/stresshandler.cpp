#include "stresshandler.h"
#include "formloader.h"

namespace StressHandler {
	void Handler::ApplyStressOnDeath(float stress_increase_percentage, float min_amount_applied) const
	{
		if (CanApplyStress()) {
			auto current_stress = GetStressLevel();
			float stress_increase = current_stress * (stress_increase_percentage / 100.0f);
			if (stress_increase < min_amount_applied) {
				stress_increase = min_amount_applied;
			}
			Forms::Loader::stress_total_global->value += stress_increase;
		}		
	}
	float Handler::GetStressLevel() const
	{
		return Forms::Loader::stress_total_global->value;
	}
	bool Handler::CanApplyStress() const
	{
		auto stress_global = Forms::Loader::stress_enabled_global;
		if(stress_global && stress_global->Is(FormType::Global) && Forms::Loader::stress_enabled_global->value > 0.0f) {
			return true;
		}
		REX::WARN("Can not find stress global or stress is disabled");
		return false;
	}
}