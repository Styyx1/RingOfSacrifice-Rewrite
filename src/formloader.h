#pragma once
#include "mod_data.h"

using namespace DATA;

namespace Forms {
	struct Loader : public REX::Singleton<Loader> {
        inline static RE::TESObjectCELL* kynesgrove_inn{ nullptr };
        inline static RE::TESObjectCELL* windhelm_inn{ nullptr };
        inline static RE::TESObjectCELL* falkreath_inn{ nullptr };
        inline static RE::TESObjectCELL* dragonbridge_inn{ nullptr };
        inline static RE::TESObjectCELL* solitude_inn{ nullptr };
        inline static RE::TESObjectCELL* nightgate_inn{ nullptr };
        inline static RE::TESObjectCELL* dawnstar_inn{ nullptr };
        inline static RE::TESObjectCELL* markarth_inn{ nullptr };
        inline static RE::TESObjectCELL* hroldan_inn{ nullptr };
        inline static RE::TESObjectCELL* riften_inn{ nullptr };
        inline static RE::TESObjectCELL* ivarstead_inn{ nullptr };
        inline static RE::TESObjectCELL* rorikstead_inn{ nullptr };
        inline static RE::TESObjectCELL* riverwood_inn{ nullptr };
        inline static RE::TESObjectCELL* whiterun_inn{ nullptr };
        inline static RE::TESObjectCELL* winterhold_inn{ nullptr };
        inline static RE::TESObjectCELL* whiterun_drunken_huntsman_inn{ nullptr };
        inline static RE::TESObjectCELL* solstheim_inn{ nullptr };
        inline static RE::TESObjectCELL* morthal_inn{ nullptr };
        inline static RE::TESGlobal* inn_price;

        inline static RE::SpellItem* cooldown_spell;
        inline static RE::SpellItem* heal_spell;
        inline static RE::TESObjectARMO* resurrect_ring;
        inline static RE::TESObjectARMO* resurrect_ring_broken;
        inline static RE::EffectSetting* cd_effect;
        inline static RE::TESWorldSpace* tamriel_world;
        inline static RE::TESImageSpaceModifier* fade_to_black;
        inline static RE::TESGlobal* ring_of_sacrifice_repair_gold;
		inline static RE::TESGlobal* ring_of_sacrifice_repair_gold_increase;
		inline static RE::TESGlobal* ring_of_sacrifice_times_repaired;
		inline static RE::BGSConstructibleObject* recipe_repair_ring;

        inline static std::unordered_set<RE::TESObjectCELL*> teleport_cells;

        inline void AdjustRepairPrice() {
            int req_count = 0;
            TESBoundObject* const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold)->As<TESBoundObject>();
            recipe_repair_ring->requiredItems.ForEachContainerObject([&](RE::ContainerObject const& entry) -> RE::BSContainer::ForEachResult {
                if (entry.obj != gold->As<TESBoundObject>()) {
                    return RE::BSContainer::ForEachResult::kContinue;
                }
                else {
                    req_count = entry.count;
                    REX::INFO("Updated recipe to requires {} gold to repair", req_count);
                    return RE::BSContainer::ForEachResult::kStop;
                }
                return RE::BSContainer::ForEachResult::kContinue;
                });
            int32_t new_amount = static_cast<int32_t>(Forms::Loader::ring_of_sacrifice_repair_gold->value + Forms::Loader::ring_of_sacrifice_repair_gold_increase->value * Forms::Loader::ring_of_sacrifice_times_repaired->value);

            Forms::Loader::recipe_repair_ring->requiredItems.RemoveObjectFromContainer(gold->As<TESBoundObject>(), req_count);
            Forms::Loader::recipe_repair_ring->requiredItems.AddObjectToContainer(gold->As<TESBoundObject>(), new_amount, nullptr);
        
        };

        inline void LoadForms() {
            const auto dh = RE::TESDataHandler::GetSingleton();
            // Worldspace, global, and other
            tamriel_world = dh->LookupForm<RE::TESWorldSpace>(tamriel_worldspace_ID, base_name);
            inn_price = dh->LookupForm<RE::TESGlobal>(inn_price_ID, base_name);
            cooldown_spell = dh->LookupForm<RE::SpellItem>(cooldown_spell_ID, mod_name);
            heal_spell = dh->LookupForm<RE::SpellItem>(heal_spell_ID, mod_name);
            resurrect_ring = dh->LookupForm<RE::TESObjectARMO>(resurrect_ring_ID, mod_name);
            cd_effect = dh->LookupForm<RE::EffectSetting>(cooldown_effect_ID, mod_name);
            fade_to_black = dh->LookupForm<RE::TESImageSpaceModifier>(fade_to_black_ID, base_name);
			resurrect_ring_broken = dh->LookupForm<RE::TESObjectARMO>(resurrect_ring_broken_ID, mod_name);
			ring_of_sacrifice_repair_gold = dh->LookupForm<RE::TESGlobal>(ring_of_sacrifice_repair_gold_ID, mod_name);
			ring_of_sacrifice_repair_gold_increase = dh->LookupForm<RE::TESGlobal>(ring_of_sacrifice_repair_increase_ID, mod_name);
			recipe_repair_ring = dh->LookupForm<RE::BGSConstructibleObject>(recipe_repair_ring_ID, mod_name);
			ring_of_sacrifice_times_repaired = dh->LookupForm<RE::TESGlobal>(ring_of_sacrifice_times_repaired_ID, mod_name);

            // Inns / cells
            kynesgrove_inn = dh->LookupForm<RE::TESObjectCELL>(kynesgrove_inn_ID, base_name);
            windhelm_inn = dh->LookupForm<RE::TESObjectCELL>(windhelm_inn_ID, base_name);
            falkreath_inn = dh->LookupForm<RE::TESObjectCELL>(falkreath_inn_ID, base_name);
            dragonbridge_inn = dh->LookupForm<RE::TESObjectCELL>(dragonbridge_inn_ID, base_name);
            solitude_inn = dh->LookupForm<RE::TESObjectCELL>(solitude_inn_ID, base_name);
            nightgate_inn = dh->LookupForm<RE::TESObjectCELL>(nightgate_inn_ID, base_name);
            dawnstar_inn = dh->LookupForm<RE::TESObjectCELL>(dawnstar_inn_ID, base_name);
            markarth_inn = dh->LookupForm<RE::TESObjectCELL>(markarth_inn_ID, base_name);
            hroldan_inn = dh->LookupForm<RE::TESObjectCELL>(hroldan_inn_ID, base_name);
            riften_inn = dh->LookupForm<RE::TESObjectCELL>(riften_inn_ID, base_name);
            ivarstead_inn = dh->LookupForm<RE::TESObjectCELL>(ivarstead_inn_ID, base_name);
            rorikstead_inn = dh->LookupForm<RE::TESObjectCELL>(rorikstead_inn_ID, base_name);
            riverwood_inn = dh->LookupForm<RE::TESObjectCELL>(riverwood_inn_ID, base_name);
            whiterun_inn = dh->LookupForm<RE::TESObjectCELL>(whiterun_inn_ID, base_name);
            winterhold_inn = dh->LookupForm<RE::TESObjectCELL>(winterhold_inn_ID, base_name);
            whiterun_drunken_huntsman_inn = dh->LookupForm<RE::TESObjectCELL>(whiterun_drunken_huntsman_inn_ID, base_name);
            solstheim_inn = dh->LookupForm<RE::TESObjectCELL>(solstheim_inn_ID, dragonborn_plugin);
            morthal_inn = dh->LookupForm<RE::TESObjectCELL>(morthal_inn_ID, base_name);

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
