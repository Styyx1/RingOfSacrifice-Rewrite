#pragma once

namespace DATA {
	
#pragma region FormData
	inline constexpr const char* mod_name = "RingOfSacrifice.esl";
	inline constexpr const char* base_name = "Skyrim.esm";
	inline constexpr const char* dragonborn_plugin = "Dragonborn.esm";

	inline constexpr int kynesgrove_inn_ID = 0x13907;
	inline constexpr int windhelm_inn_ID = 0x16789;
	inline constexpr int falkreath_inn_ID = 0x3A184;
	inline constexpr int dragonbridge_inn_ID = 0x13A7C;
	inline constexpr int solitude_inn_ID = 0x16A0E;
	inline constexpr int nightgate_inn_ID = 0x13A5D;
	inline constexpr int dawnstar_inn_ID = 0x13A7F;
	inline constexpr int markarth_inn_ID = 0x16DFE;
	inline constexpr int hroldan_inn_ID = 0x138BE;
	inline constexpr int riften_inn_ID = 0x16BDF;
	inline constexpr int ivarstead_inn_ID = 0x013A5C;
	inline constexpr int rorikstead_inn_ID = 0x13870;
	inline constexpr int riverwood_inn_ID = 0x133C6;
	inline constexpr int whiterun_inn_ID = 0x1605E;
	inline constexpr int winterhold_inn_ID = 0x13814;
	inline constexpr int whiterun_drunken_huntsman_inn_ID = 0x165B7;
	inline constexpr int solstheim_inn_ID = 0x17EC0;
	inline constexpr int morthal_inn_ID = 0x138CE;
	inline constexpr int teleporter_marker_cell_ID = 0x808;

	inline constexpr int heal_spell_ID = 0x805;
	inline constexpr int cooldown_spell_ID = 0x807;
	inline constexpr int resurrect_ring_ID = 0x803;
	inline constexpr int cooldown_effect_ID = 0x806;
	inline constexpr int tamriel_worldspace_ID = 0x3C;
	inline constexpr int fade_to_black_ID = 0xF756D;
	inline constexpr int resurrect_ring_broken_ID = 0x809;
	inline constexpr int ring_of_sacrifice_repair_gold_ID = 0x80B;
	inline constexpr int recipe_repair_ring_ID = 0x80A;
	inline constexpr int ring_of_sacrifice_repair_increase_ID = 0x80C;
	inline constexpr int ring_of_sacrifice_times_repaired_ID = 0x80D;
	inline constexpr int allowed_bed_list_ID = 0x80E;
	inline constexpr int respawn_marker_ID = 0x814;

	//stress mod integration
	inline constexpr std::string_view stress_mod_name = "Stress and Fear.esp";
	inline constexpr int stress_enabled_global_ID = 0x801;
	inline constexpr int stress_level_global_ID = 0x8A5;

#pragma endregion FormData
#pragma region ConfigData
	inline constexpr std::string_view toml_file_path_default = "Data/SKSE/Plugins/ring-of-sacrifice.toml";
	inline constexpr std::string_view toml_file_path_custom = "Data/SKSE/Plugins/ring-of-sacrifice_custom.toml";
	inline constexpr auto MAIN_SEC = "Settings";
	inline constexpr auto DEBUG_SEC = "Debugging";
	inline constexpr auto TOGGLES = "Toggles";
#pragma endregion ConfigData
}
