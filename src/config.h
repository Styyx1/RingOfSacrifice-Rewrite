#pragma once

#include "mod_data.h"
using namespace DATA;
struct Settings : public REX::Singleton<Settings> {
	static inline REX::TOML::Bool only_allow_inn_beds{ TOGGLES, "bOnlyAllowInns", false };
	static inline REX::TOML::Bool allow_broken_ring{ TOGGLES, "bAllowRingBreaking", true };

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