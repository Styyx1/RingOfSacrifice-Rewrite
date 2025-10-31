#pragma once
#include "mod_data.h"

using namespace DATA;

namespace Forms {
	struct Loader : public REX::Singleton<Loader> {
        inline static TESObjectCELL* kynesgrove_inn{ nullptr };
        inline static TESObjectCELL* windhelm_inn{ nullptr };
        inline static TESObjectCELL* falkreath_inn{ nullptr };
        inline static TESObjectCELL* dragonbridge_inn{ nullptr };
        inline static TESObjectCELL* solitude_inn{ nullptr };
        inline static TESObjectCELL* nightgate_inn{ nullptr };
        inline static TESObjectCELL* dawnstar_inn{ nullptr };
        inline static TESObjectCELL* markarth_inn{ nullptr };
        inline static TESObjectCELL* hroldan_inn{ nullptr };
        inline static TESObjectCELL* riften_inn{ nullptr };
        inline static TESObjectCELL* ivarstead_inn{ nullptr };
        inline static TESObjectCELL* rorikstead_inn{ nullptr };
        inline static TESObjectCELL* riverwood_inn{ nullptr };
        inline static TESObjectCELL* whiterun_inn{ nullptr };
        inline static TESObjectCELL* winterhold_inn{ nullptr };
        inline static TESObjectCELL* whiterun_drunken_huntsman_inn{ nullptr };
        inline static TESObjectCELL* solstheim_inn{ nullptr };
        inline static TESObjectCELL* morthal_inn{ nullptr };
		inline static TESObjectCELL* teleporter_marker_cell{ nullptr };

        inline static SpellItem* cooldown_spell;
        inline static SpellItem* heal_spell;
        inline static TESObjectARMO* resurrect_ring;
        inline static TESObjectARMO* resurrect_ring_broken;
        inline static EffectSetting* cd_effect;
        inline static TESWorldSpace* tamriel_world;
        inline static TESImageSpaceModifier* fade_to_black;
        inline static TESGlobal* ring_of_sacrifice_repair_gold;
		inline static TESGlobal* ring_of_sacrifice_repair_gold_increase;
		inline static TESGlobal* ring_of_sacrifice_times_repaired;
		inline static BGSConstructibleObject* recipe_repair_ring;
        inline static BGSListForm* allowed_bed_list;
		inline static TESObjectREFR* respawn_marker;

        //stress globals
		inline static TESGlobal* stress_enabled_global;
		inline static TESGlobal* stress_total_global;

        inline static std::unordered_set<TESObjectCELL*> teleport_cells;

        inline void AdjustRepairPrice() {
            int req_count = 0;
            TESBoundObject* const gold = BGSDefaultObjectManager::GetSingleton()->GetObject(DEFAULT_OBJECT::kGold)->As<TESBoundObject>();
            recipe_repair_ring->requiredItems.ForEachContainerObject([&](ContainerObject const& entry) -> BSContainer::ForEachResult {
                if (entry.obj != gold->As<TESBoundObject>()) {
                    return BSContainer::ForEachResult::kContinue;
                }
                else {
                    req_count = entry.count;
                    REX::INFO("Updated recipe to requires {} gold to repair", req_count);
                    return BSContainer::ForEachResult::kStop;
                }
                return BSContainer::ForEachResult::kContinue;
                });
            int32_t new_amount = static_cast<int32_t>(Forms::Loader::ring_of_sacrifice_repair_gold->value + Forms::Loader::ring_of_sacrifice_repair_gold_increase->value * Forms::Loader::ring_of_sacrifice_times_repaired->value);

            Forms::Loader::recipe_repair_ring->requiredItems.RemoveObjectFromContainer(gold->As<TESBoundObject>(), req_count);
            Forms::Loader::recipe_repair_ring->requiredItems.AddObjectToContainer(gold->As<TESBoundObject>(), new_amount, nullptr);
        
        };

        inline void LoadForms() {
            const auto dh = TESDataHandler::GetSingleton();
            // Worldspace, global, and other

            if(const auto file = dh->LookupModByName(mod_name); !file || file->compileIndex == 0xFF) {
                REX::FAIL(std::format("Could not find {}, please enable the mod", mod_name));
                return;
			}

            tamriel_world = dh->LookupForm<TESWorldSpace>(tamriel_worldspace_ID, base_name);
            cooldown_spell = dh->LookupForm<SpellItem>(cooldown_spell_ID, mod_name);
            heal_spell = dh->LookupForm<SpellItem>(heal_spell_ID, mod_name);
            resurrect_ring = dh->LookupForm<TESObjectARMO>(resurrect_ring_ID, mod_name);
            cd_effect = dh->LookupForm<EffectSetting>(cooldown_effect_ID, mod_name);
            fade_to_black = dh->LookupForm<TESImageSpaceModifier>(fade_to_black_ID, base_name);
			resurrect_ring_broken = dh->LookupForm<TESObjectARMO>(resurrect_ring_broken_ID, mod_name);
			ring_of_sacrifice_repair_gold = dh->LookupForm<TESGlobal>(ring_of_sacrifice_repair_gold_ID, mod_name);
			ring_of_sacrifice_repair_gold_increase = dh->LookupForm<TESGlobal>(ring_of_sacrifice_repair_increase_ID, mod_name);
			recipe_repair_ring = dh->LookupForm<BGSConstructibleObject>(recipe_repair_ring_ID, mod_name);
			ring_of_sacrifice_times_repaired = dh->LookupForm<TESGlobal>(ring_of_sacrifice_times_repaired_ID, mod_name);
			allowed_bed_list = dh->LookupForm<BGSListForm>(allowed_bed_list_ID, mod_name);
			respawn_marker = dh->LookupForm<TESObjectREFR>(respawn_marker_ID, mod_name);

            // Inns / cells
            kynesgrove_inn = dh->LookupForm<TESObjectCELL>(kynesgrove_inn_ID, base_name);
            windhelm_inn = dh->LookupForm<TESObjectCELL>(windhelm_inn_ID, base_name);
            falkreath_inn = dh->LookupForm<TESObjectCELL>(falkreath_inn_ID, base_name);
            dragonbridge_inn = dh->LookupForm<TESObjectCELL>(dragonbridge_inn_ID, base_name);
            solitude_inn = dh->LookupForm<TESObjectCELL>(solitude_inn_ID, base_name);
            nightgate_inn = dh->LookupForm<TESObjectCELL>(nightgate_inn_ID, base_name);
            dawnstar_inn = dh->LookupForm<TESObjectCELL>(dawnstar_inn_ID, base_name);
            markarth_inn = dh->LookupForm<TESObjectCELL>(markarth_inn_ID, base_name);
            hroldan_inn = dh->LookupForm<TESObjectCELL>(hroldan_inn_ID, base_name);
            riften_inn = dh->LookupForm<TESObjectCELL>(riften_inn_ID, base_name);
            ivarstead_inn = dh->LookupForm<TESObjectCELL>(ivarstead_inn_ID, base_name);
            rorikstead_inn = dh->LookupForm<TESObjectCELL>(rorikstead_inn_ID, base_name);
            riverwood_inn = dh->LookupForm<TESObjectCELL>(riverwood_inn_ID, base_name);
            whiterun_inn = dh->LookupForm<TESObjectCELL>(whiterun_inn_ID, base_name);
            winterhold_inn = dh->LookupForm<TESObjectCELL>(winterhold_inn_ID, base_name);
            whiterun_drunken_huntsman_inn = dh->LookupForm<TESObjectCELL>(whiterun_drunken_huntsman_inn_ID, base_name);
            solstheim_inn = dh->LookupForm<TESObjectCELL>(solstheim_inn_ID, dragonborn_plugin);
            morthal_inn = dh->LookupForm<TESObjectCELL>(morthal_inn_ID, base_name);
			teleporter_marker_cell = dh->LookupForm<TESObjectCELL>(teleporter_marker_cell_ID, mod_name);
            
			//stress globals
			stress_enabled_global = dh->LookupForm<TESGlobal>(stress_enabled_global_ID, stress_mod_name.data());
            stress_total_global = dh->LookupForm<TESGlobal>(stress_level_global_ID, stress_mod_name.data());

			REX::INFO("Respawn Marker Form: {}", respawn_marker->GetName());

			REX::INFO("Forms loaded successfully.");

            AdjustRepairPrice();

            teleport_cells = {
            kynesgrove_inn,
            windhelm_inn,
            falkreath_inn,
            dragonbridge_inn,
            solitude_inn,
            nightgate_inn,
            dawnstar_inn,
            markarth_inn,
            hroldan_inn,
            riften_inn,
            ivarstead_inn,
            rorikstead_inn,
            riverwood_inn,
            whiterun_inn,
            winterhold_inn,
            whiterun_drunken_huntsman_inn,
            solstheim_inn,
            morthal_inn
            };

        };


	};
}
