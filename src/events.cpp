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

	void ItemCraftedEvent::Register()
	{		
		auto src = ItemCrafted::GetEventSource();
		src->AddEventSink(this);
		REX::INFO("Registered for {}", typeid(ItemCrafted::Event).name());
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
				REX::INFO("Updated recipe to requires {} gold to repair", req_count);
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
			curr_cell->ForEachReferenceInRange(player->GetPosition(), 200,
				([&](RE::TESObjectREFR* a_ref)->RE::BSContainer::ForEachResult
					{
						if (!a_ref->Is3DLoaded()) {
							return RE::BSContainer::ForEachResult::kContinue;
						}
						if (const auto base = a_ref->GetBaseObject(); base && base->GetFormType() == RE::FormType::Furniture) {
							std::string obj_base_name = base->GetName();
							auto lowercase_name = std::transform(obj_base_name.begin(), obj_base_name.end(), obj_base_name.begin(), ::tolower);
							REX::INFO("name of ref is: {}", base_name);
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
	}
	void FillFormID()
	{
		if (auto& last_bed = SleepEvent::GetSingleton()->last_used_bed; last_bed) {
			SleepEvent::bed_ID = last_bed->GetFormID();
			REX::INFO("FormID is {}", SleepEvent::bed_ID);
		}
	}
	void FindRefWithID(RE::FormID a_ID)
	{
		if (a_ID != 0) {
			auto bed = SleepEvent::GetSingleton()->last_used_bed;
			bed = RE::TESForm::LookupByID(a_ID)->AsReference();
			if (bed && bed->Is(RE::FormType::Reference) && bed->IsHandleValid()) {
				REX::INFO("name of the reference found. it is: {}", SleepEvent::GetSingleton()->last_used_bed->GetName());
				SleepEvent::GetSingleton()->last_used_bed = bed;
			}
			else
				return;
		}
		else {
			REX::INFO("no form id saved");
		}
		
	}
}


