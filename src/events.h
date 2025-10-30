#pragma once

namespace ResEvent {
	using Result = RE::BSEventNotifyControl;
	void RegisterAll();

	struct ItemCraftedEvent : public REX::Singleton<ItemCraftedEvent>, public RE::BSTEventSink<ItemCrafted::Event> {
		void Register();
		Result ProcessEvent(const ItemCrafted::Event* a_event, RE::BSTEventSource<ItemCrafted::Event>*) override;
		uint32_t item_crafted_count{0};
		int32_t GetRequiredGoldCount();
	};
	struct CellChangeEvent : public REX::Singleton<CellChangeEvent>, public RE::BSTEventSink<BGSActorCellEvent> {
		void Register();
		Result ProcessEvent(const RE::BGSActorCellEvent* a_event, RE::BSTEventSource<RE::BGSActorCellEvent>*) override;
		std::optional<bool> last_cell_interior;
	};

}
