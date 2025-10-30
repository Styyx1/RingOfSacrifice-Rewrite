#pragma once

#include "mod_data.h"
using namespace DATA;
struct Settings : public REX::Singleton<Settings> {
	static inline REX::TOML::Bool allow_broken_ring{ TOGGLES, "bAllowRingBreaking", true };
	static inline REX::TOML::Str no_resurrect_keyword{ MAIN_SEC, "sNoResurrectKeyword", std::string("NotResurrect") };
	static inline REX::TOML::F32 stress_on_death_percentage{ MAIN_SEC, "fStressOnDeathPercentage", 15.0f };
	static inline REX::TOML::F32 stress_on_death_minimum{ MAIN_SEC, "fStressOnDeathMinimum", 15.0f };

	inline void LoadSettings() {
		const auto toml = REX::TOML::SettingStore::GetSingleton();
		toml->Init(toml_file_path_default.data(), toml_file_path_custom.data());
		toml->Load();
	};
	inline void SaveSettings() {
		const auto toml = REX::TOML::SettingStore::GetSingleton();
		toml->Init(toml_file_path_default.data(), toml_file_path_custom.data());
		toml->Save();
	};
};