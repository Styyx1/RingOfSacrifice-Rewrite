#include "events.h"
#include "formloader.h"
#include "config.h"

namespace ResEvent {

	Result CellChangeEvent::ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*)
	{
		if(!a_event || a_event->flags == RE::BGSActorCellEvent::CellFlag::kLeave)
			return Result::kContinue;		

		auto m_cellID = a_event->cellID;
		auto cell = RE::TESForm::LookupByID(m_cellID)->As<RE::TESObjectCELL>();

		if (!cell) {
			return Result::kContinue;			
		}
		const bool curr_cell_interior = cell->IsInteriorCell();
		auto actor = RE::PlayerCharacter::GetSingleton();	

		//https://github.com/powerof3/LightPlacer/blob/a549263c61e37d07d0c1a36d9037bf73db44b319/src/Manager.cpp#L516
		if (last_cell_interior != curr_cell_interior) {			
			Forms::Loader::respawn_marker->MoveTo(actor);
		}
		last_cell_interior = curr_cell_interior;

		return Result::kContinue;
	}

	void ItemCraftedEvent::Register()
	{		
		auto src = ItemCrafted::GetEventSource();
		src->AddEventSink(this);
		REX::INFO("Registered for {}", typeid(ItemCrafted::Event).name());
	}

	void CellChangeEvent::Register()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		player->AddEventSink(this);
		REX::INFO("Registered for {}", typeid(RE::BGSActorCellEvent).name());
	}

	Result ItemCraftedEvent::ProcessEvent(const ItemCrafted::Event* a_event, RE::BSTEventSource<ItemCrafted::Event>*)
	{
		if(!a_event)
			return Result::kContinue;

		if(a_event->item != Forms::Loader::resurrect_ring)
			return Result::kContinue;
		TESBoundObject* const gold = RE::BGSDefaultObjectManager::GetSingleton()->GetObject(RE::DEFAULT_OBJECT::kGold)->As<TESBoundObject>();
		int32_t req_count = 0;
		Forms::Loader::recipe_repair_ring->requiredItems.ForEachContainerObject([&](RE::ContainerObject const& entry) -> RE::BSContainer::ForEachResult {
			if (entry.obj != gold->As<TESBoundObject>()) {
				return RE::BSContainer::ForEachResult::kContinue;
			}
			else {
				req_count = entry.count;
				REX::DEBUG("Updated recipe to requires {} gold to repair", req_count);
				return RE::BSContainer::ForEachResult::kStop;
			}
			return RE::BSContainer::ForEachResult::kContinue;
			});
		Forms::Loader::ring_of_sacrifice_times_repaired->value++;
		Forms::Loader::recipe_repair_ring->requiredItems.RemoveObjectFromContainer(gold, req_count);
		Forms::Loader::recipe_repair_ring->requiredItems.AddObjectToContainer(gold, GetRequiredGoldCount(), nullptr);
		
		return Result::kContinue;
	}
	int32_t ItemCraftedEvent::GetRequiredGoldCount()
	{
		return static_cast<int32_t>(Forms::Loader::ring_of_sacrifice_repair_gold->value + Forms::Loader::ring_of_sacrifice_repair_gold_increase->value * Forms::Loader::ring_of_sacrifice_times_repaired->value);
	}
	
	void RegisterAll()
	{
		ItemCraftedEvent::GetSingleton()->Register();
		CellChangeEvent::GetSingleton()->Register();
	}	
}


