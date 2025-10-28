#pragma once

namespace ResEvent {
	using Result = RE::BSEventNotifyControl;
	void RegisterAll();
	void FillFormID();
	void FindRefWithID(RE::FormID a_ID);

	struct SleepEvent : public REX::Singleton<SleepEvent>, public RE::BSTEventSink<RE::TESSleepStopEvent> {

		void Register();
		Result ProcessEvent(const RE::TESSleepStopEvent* a_event, RE::BSTEventSource<RE::TESSleepStopEvent>*) override;
		inline static RE::FormID bed_ID{ 0x0 };
		RE::TESObjectREFR* last_used_bed{nullptr};
		RE::TESObjectCELL* last_cell{nullptr};
	};

	struct ItemCraftedEvent : public REX::Singleton<ItemCraftedEvent>, public RE::BSTEventSink<ItemCrafted::Event> {
		void Register();
		Result ProcessEvent(const ItemCrafted::Event* a_event, RE::BSTEventSource<ItemCrafted::Event>*) override;
		uint32_t item_crafted_count{0};
		int32_t GetRequiredGoldCount();
	};

}
