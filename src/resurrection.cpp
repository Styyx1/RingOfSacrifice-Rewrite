#include "resurrection.h"
#include "utility.h"
#include "config.h"
#include "formloader.h"
#include "events.h"
namespace Mod {
	bool Resurrect::should_resurrect(RE::Actor* a) const
    {
        return (a->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kRing) == 
            Forms::Loader::resurrect_ring) && 
            !Util::Actor::IsEffectActive(a, Forms::Loader::cd_effect);
    }
    void Resurrect::resurrect(RE::Actor* a)
    {
        RE::PlayerCharacter* const player = RE::PlayerCharacter::GetSingleton();
        RE::TESForm* const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
        if (a == player) {
            Util::Magic::ApplySpell(a, a, Forms::Loader::cooldown_spell);
            a->RemoveItem(Forms::Loader::resurrect_ring, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
            if (player->GetItemCount(Forms::Loader::resurrect_ring) > 0) {
                auto eqman = RE::ActorEquipManager::GetSingleton();
				eqman->UnequipObject(a, Forms::Loader::resurrect_ring, nullptr, 1, nullptr, true, false, true, true, nullptr);
            }
            if (Settings::allow_broken_ring.GetValue()) {
				player->AddObjectToContainer(Forms::Loader::resurrect_ring_broken, nullptr, 1, nullptr);
            }
            a->DrawWeaponMagicHands(false);
            a->RestoreActorValue(RE::ActorValue::kHealth, Util::Actor::GetMaxHealth(a));
            a->RestoreActorValue(RE::ActorValue::kStamina, Util::Actor::GetMaxStamina(a));
            resurrectEnemiesOnDeath(a->GetParentCell());
            Teleport::GetSingleton()->TeleportToRandomInn(player);
            a->RemoveItem(gold->As<RE::TESBoundObject>(), static_cast<uint32_t>(Forms::Loader::inn_price->value), RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
            return;
        }
        else {
            Util::Magic::ApplySpell(a, a, Forms::Loader::cooldown_spell);
            a->RemoveItem(Forms::Loader::resurrect_ring, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
            a->RestoreActorValue(RE::ActorValue::kHealth, Util::Actor::GetMaxHealth(a));
            a->RestoreActorValue(RE::ActorValue::kStamina, Util::Actor::GetMaxStamina(a));
            return;
        }
    }
    void Resurrect::resurrectEnemiesOnDeath(RE::TESObjectCELL* a_cell)
    {
        if (!a_cell)
            return;
        for (auto& cell : a_cell->references) {
            auto actor = cell.get()->As<RE::Actor>();
            if (actor && actor->IsInCombat() && !actor->IsPlayerRef()) {
                if (actor->extraList.HasQuestObjectAlias())
                    return;
                auto extra_cell = a_cell->extraList.GetByType< ExtraPromotedRef>();
                if(extra_cell) {
                    for (auto& owner : extra_cell->promotedRefOwners) {
                        if (owner && owner == actor) {
                            return;
                        }
                    }
				}
                actor->Resurrect(true, true);
               
            }
        }
    }
    void Teleport::TeleportToRandomInn(RE::PlayerCharacter* player)
    {
        auto cell = player->GetParentCell();
        if (!cell) {
            cell = player->GetSaveParentCell();
        }
        if (!cell) {
            REX::WARN("no cell found, teleport not possible");
            return;
        }
        if (const auto marker_cell = Forms::Loader::respawn_marker->GetParentCell(); marker_cell && marker_cell != Forms::Loader::teleporter_marker_cell) {
            player->MoveTo(Forms::Loader::respawn_marker);
            RE::ImageSpaceModifierInstanceForm::Trigger(Forms::Loader::fade_to_black, 1.0, player->As<RE::NiAVObject>());
            return;
        }
		
        //fallback if no respawn marker is set. should be impossible, but the code already exists, so, whatever
        if (cell->IsInteriorCell()) {
            REX::DEBUG("{} is interior cell", cell->GetName());
            PacifyEnemies(cell);
            player->CenterOnCell(cell);                        
            RE::ImageSpaceModifierInstanceForm::Trigger(Forms::Loader::fade_to_black, 1.0, player->As<RE::NiAVObject>());
            REX::DEBUG("teleport interior happened");
            return;
        }
        else {
            PacifyEnemies(cell);
            auto& set = Forms::Loader::teleport_cells;
            if (set.empty()) {
                REX::WARN("Cell List is empty");
                player->CenterOnCell(Forms::Loader::riverwood_inn);
                RE::ImageSpaceModifierInstanceForm::Trigger(Forms::Loader::fade_to_black, 1.0, player->As<RE::NiAVObject>());
            }              
            
            int rng = Util::Randomiser::GetRandomInt(0, static_cast<int>(set.size() - 1));
            auto it = set.begin();
            std::advance(it, rng);
            RE::TESObjectCELL* random_cell = *it;
            REX::DEBUG("name of cell is {}", random_cell->GetName());
            player->CenterOnCell(random_cell);
            RE::ImageSpaceModifierInstanceForm::Trigger(Forms::Loader::fade_to_black, 1.0, player->As<RE::NiAVObject>());
        }
        
    }
    void Teleport::PacifyEnemies(RE::TESObjectCELL* a_cell)
    {
        if (!a_cell)
            return;

        for (auto& cell : a_cell->references) {
            if (cell) {
                if (auto actor = cell.get()->As<RE::Actor>(); actor && actor->IsInCombat() && !actor->IsPlayerRef()) {
                    const auto process = RE::ProcessLists::GetSingleton();
                    process->runDetection = false;
                    process->ClearCachedFactionFightReactions();
                    process->StopCombatAndAlarmOnActor(actor, true);
                    process->runDetection = true;
                    actor->StopCombat();
                }
            }
        }
    }
}
