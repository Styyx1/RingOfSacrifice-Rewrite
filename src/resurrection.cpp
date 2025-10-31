#include "resurrection.h"
#include "config.h"
#include "formloader.h"
#include "events.h"
#include "stresshandler.h"
namespace Mod {
	bool Resurrect::should_resurrect(RE::Actor* a) const
    {
        return (a->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kRing) == 
            Forms::Loader::resurrect_ring) && 
            !ActorUtil::IsEffectActive(a, Forms::Loader::cd_effect);
    }
    void Resurrect::resurrect(RE::Actor* a)
    {
        RE::PlayerCharacter* const player = RE::PlayerCharacter::GetSingleton();
        RE::TESForm* const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold);
        if (a == player) {
            MagicUtil::ApplySpell(a, a, Forms::Loader::cooldown_spell);
            StressHandler::Handler::GetSingleton()->ApplyStressOnDeath(15.0f, 15.0f);
            a->RemoveItem(Forms::Loader::resurrect_ring, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
            if (player->GetItemCount(Forms::Loader::resurrect_ring) > 0) {
                auto eqman = RE::ActorEquipManager::GetSingleton();
				eqman->UnequipObject(a, Forms::Loader::resurrect_ring, nullptr, 1, nullptr, true, false, true, true, nullptr);
            }
            if (Settings::allow_broken_ring.GetValue()) {
				player->AddObjectToContainer(Forms::Loader::resurrect_ring_broken, nullptr, 1, nullptr);
            }
            a->DrawWeaponMagicHands(false);            
            ActorUtil::FullyHealActor(a);
            resurrectEnemiesOnDeath(a->GetParentCell());
            Teleport::GetSingleton()->TeleportToRandomInn(player);
            a->RemoveItem(gold->As<RE::TESBoundObject>(), static_cast<uint32_t>(Forms::Loader::inn_price->value), RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
            return;
        }
        else {
            MagicUtil::ApplySpell(a, a, Forms::Loader::cooldown_spell);
            a->RemoveItem(Forms::Loader::resurrect_ring, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
            if (Settings::allow_broken_ring.GetValue()) {
                player->AddObjectToContainer(Forms::Loader::resurrect_ring_broken, nullptr, 1, nullptr);
            }
            ActorUtil::FullyHealActor(a);
            return;
        }
    }
	//resurrect all dead non-unique, non-quest, non-boss enemies in the cell and npcs without the no-resurrect keyword
    void Resurrect::resurrectEnemiesOnDeath(RE::TESObjectCELL* a_cell)
    {
        if (!a_cell)
            return;
        for (auto& cell : a_cell->references) {
            auto actor = cell.get()->As<RE::Actor>();
            if (actor && !actor->IsPlayerRef() && actor->IsDead() && !actor->IsPlayerTeammate() && actor->GetActorBase() && !actor->GetActorBase()->IsUnique()) {
                if (actor->extraList.HasQuestObjectAlias())
                    continue;
                if (ActorUtil::StartsDead(actor)) {					
                    continue;
                }
                if (actor->HasKeywordString(Settings::no_resurrect_keyword.GetValue()) || actor->GetActorBase()->HasKeywordString(Settings::no_resurrect_keyword.GetValue())) {
                    continue;
                }
                bool is_boss = false;
				auto loc = a_cell->GetLocation();
                if (loc && !loc->promotedRefs.empty()) {
                    for (auto& ref : loc->promotedRefs){
                        if(ref.get().get() && ref.get().get() == actor) {
                            is_boss = true;
                            break;
                        }                        
                    }
                }
                if (is_boss)
                    continue;

                actor->Resurrect(true, true);               
            }
        }
    }

	//name is misleading, this function teleports to respawn marker if set, otherwise random inn. The name is basically legacy
    void Teleport::TeleportToRandomInn(RE::PlayerCharacter* player)
    {       
        if (auto marker_cell = Forms::Loader::respawn_marker->GetParentCell(); marker_cell != Forms::Loader::teleporter_marker_cell) {
            PacifyEnemies(ActorUtil::GetPlayerCell(player));
			player->SetPosition(Forms::Loader::respawn_marker->GetPosition(), true);
            RE::ImageSpaceModifierInstanceForm::Trigger(Forms::Loader::fade_to_black, 1.0, player->As<RE::NiAVObject>());
            return;
        }
        //fallback if no respawn marker is set. should be impossible, but the code already exists, so, whatever
        auto cell = ActorUtil::GetPlayerCell(player);
        if (!cell) {
            return;
        }

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
            
            int rng = RandomiserUtil::GetRandomInt(0, static_cast<int>(set.size() - 1));
            auto it = set.begin();
            std::advance(it, rng);
            RE::TESObjectCELL* random_cell = *it;
            REX::DEBUG("name of cell is {}", random_cell->GetName());
            player->CenterOnCell(random_cell);
            RE::ImageSpaceModifierInstanceForm::Trigger(Forms::Loader::fade_to_black, 1.0, player->As<RE::NiAVObject>());
        }
        
    }

	//stop combat on all enemies in the cell
    void Teleport::PacifyEnemies(RE::TESObjectCELL* a_cell)
    {
        if (!a_cell)
            return;

        for (auto& cell : a_cell->references) {
            if (cell) {
                if (auto actor = cell.get()->As<RE::Actor>(); actor && actor->IsInCombat() && !actor->IsPlayerRef()) {
                    const auto process = RE::ProcessLists::GetSingleton();
                    //process->runDetection = false;
                    process->ClearCachedFactionFightReactions();
                    process->StopCombatAndAlarmOnActor(actor, true);
                    //process->runDetection = true;
                    actor->StopCombat();
                }
            }
        }
    }
}
