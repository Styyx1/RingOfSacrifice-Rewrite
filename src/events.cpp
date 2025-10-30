#include "events.h"
#include "formloader.h"
#include "config.h"

namespace ResEvent {
	void SleepEvent::Register()
	{
		auto src = RE::ScriptEventSourceHolder::GetSingleton();
		src->AddEventSink(this);
		REX::INFO("Registered for {}", typeid(RE::TESSleepStopEvent).name());
	}

	Result CellChangeEvent::ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*)
	{
		if(!a_event || a_event->flags == RE::BGSActorCellEvent::CellFlag::kLeave)
			return Result::kContinue;

		auto m_cellID = a_event->cellID;
		auto cell = RE::TESForm::LookupByID(m_cellID)->As<RE::TESObjectCELL>();

		if(!cell)
			return Result::kContinue;

		const bool curr_cell_interior = cell->IsInteriorCell();

		if(auto ui = UI::GetSingleton(); ui && ui->IsMenuOpen(MainMenu::MENU_NAME)) {
			return Result::kContinue;
		}

		if (Forms::Loader::teleport_cells.contains(cell))
			return Result::kContinue;

		auto actor = a_event->actor.get().get();
		if (!actor || !actor->IsPlayerRef()) {
			return Result::kContinue;
		}
		//https://github.com/powerof3/LightPlacer/blob/a549263c61e37d07d0c1a36d9037bf73db44b319/src/Manager.cpp#L516
		if (last_cell_interior != curr_cell_interior) {
			Forms::Loader::respawn_marker->MoveTo(actor);
			REX::DEBUG("Moved respawn marker to {} in cell: {}", actor->GetName(), cell->GetName());
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
	Result SleepEvent::ProcessEvent(const RE::TESSleepStopEvent* a_event, RE::BSTEventSource<RE::TESSleepStopEvent>*)
	{
		if(!a_event)
			return Result::kContinue;
		auto player = RE::PlayerCharacter::GetSingleton();
		auto curr_cell = player->GetParentCell();	

		const bool in_allowed_cell = Forms::Loader::teleport_cells.contains(curr_cell);
		const bool allow_all_beds = !Settings::only_allow_inn_beds.GetValue();

		if (curr_cell && (in_allowed_cell || allow_all_beds)) {
			Forms::Loader::respawn_marker->MoveTo(player);
			curr_cell->ForEachReferenceInRange(player->GetPosition(), 200,
				([&](RE::TESObjectREFR* a_ref)->RE::BSContainer::ForEachResult
					{
						if (!a_ref->Is3DLoaded()) {
							return RE::BSContainer::ForEachResult::kContinue;
						}
						if (const auto base = a_ref->GetBaseObject(); base && base->GetFormType() == RE::FormType::Furniture) {
							std::string obj_base_name = base->GetName();
							auto lowercase_name = std::transform(obj_base_name.begin(), obj_base_name.end(), obj_base_name.begin(), ::tolower);
							REX::DEBUG("name of ref is: {}", obj_base_name);
							if (obj_base_name.find("bed") != std::string::npos) {
								last_used_bed = a_ref;
								last_cell = curr_cell;
								FillFormID();
								RE::BSContainer::ForEachResult::kStop;
							}
						}
						return RE::BSContainer::ForEachResult::kContinue;
				})
			);
		}
		
		return Result::kContinue;
	}
	void RegisterAll()
	{
		SleepEvent::GetSingleton()->Register();
		ItemCraftedEvent::GetSingleton()->Register();
		CellChangeEvent::GetSingleton()->Register();
	}
	void FillFormID()
	{
		if (auto& last_bed = SleepEvent::GetSingleton()->last_used_bed; last_bed) {
			SleepEvent::bed_ID = last_bed->GetFormID();
			REX::DEBUG("FormID is {}", SleepEvent::bed_ID);
		}
	}
	void FindRefWithID(RE::FormID a_ID)
	{
		if (a_ID != 0) {
			auto bed = RE::TESForm::LookupByID(a_ID);
			if (bed) {				
				SleepEvent::GetSingleton()->last_used_bed = bed->As<RE::TESObjectREFR>();
				REX::DEBUG("name of the reference found. it is: {}", SleepEvent::GetSingleton()->last_used_bed->GetName());
			}
			return;
		}
		else {
			REX::ERROR("no form id saved");
		}
		return;
		
	}
}


