#pragma once
#include <API/ResurrectionAPI.h>


namespace Mod{
	struct Resurrect : public REX::Singleton<Resurrect> , public ResurrectionAPI {
		bool should_resurrect(RE::Actor* a) const override;
		void resurrect(RE::Actor* a) override;
		void resurrectEnemiesOnDeath(RE::TESObjectCELL* a_cell);
		bool StartsDead(const RE::Actor* actor) const;
		
	};
	struct Teleport : REX::Singleton<Teleport> {
		void TeleportToRandomInn(RE::PlayerCharacter* player);
		void PacifyEnemies(RE::TESObjectCELL* a_cell);
	};
}