#include "resurrection.h"
#include "config.h"
#include "formloader.h"
#include "events.h"
#include "serialisation.h"

void addSubscriber()
{
	if (auto pluginHandle = REX::W32::GetModuleHandleA("ResurrectionAPI.dll")) {
		if (auto AddSubscriber = (AddSubscriber_t)GetProcAddress(pluginHandle, "ResurrectionAPI_AddSubscriber")) {
			AddSubscriber(std::make_unique<Mod::Resurrect>());
		}
	}
}

void Listener(SKSE::MessagingInterface::Message* a_msg) {
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		Forms::Loader::GetSingleton()->LoadForms();
		ResEvent::RegisterAll();		
		addSubscriber();
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
		Forms::Loader::GetSingleton()->AdjustRepairPrice();
		break;
	}
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

	Settings::GetSingleton()->LoadSettings();
	SKSE::GetMessagingInterface()->RegisterListener(Listener);
	if (auto serialization = SKSE::GetSerializationInterface())
	{
		serialization->SetUniqueID(Serialisation::ID);
		serialization->SetSaveCallback(&Serialisation::OnSave);
		serialization->SetLoadCallback(&Serialisation::OnLoad);
		serialization->SetRevertCallback(&Serialisation::OnRevert);
	}

	return true;
}
