#pragma once
#include <API/ResurrectionAPI.h>


namespace Mod{
	struct Resurrect : public REX::Singleton<Resurrect> , public ResurrectionAPI {
		bool should_resurrect(RE::Actor* a) const override;
		void resurrect(RE::Actor* a) override;
		
	};
	struct Teleport : REX::Singleton<Teleport> {
		void TeleportToRandomInn(RE::PlayerCharacter* player);
		void PacifyEnemies(RE::TESObjectCELL* a_cell);
	};
}