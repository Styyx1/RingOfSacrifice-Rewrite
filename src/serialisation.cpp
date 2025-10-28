#include "serialisation.h"
#include "events.h"

namespace Serialisation {

	void OnSave(SKSE::SerializationInterface* a_skse)
	{
		if (!a_skse->OpenRecord(SerializationType, SerializationVersion))
		{
			REX::ERROR("Failed to open Ring Of Sacrifice record");
			return;
		}
		else
		{
			std::uint32_t last_bed_ID = ResEvent::SleepEvent::bed_ID;
			if (!a_skse->WriteRecordData(last_bed_ID))
			{
				REX::ERROR("Failed to save record data");
				return;
			}
			else {
				REX::INFO("saved last bed FormID: {}", last_bed_ID);
			}
		}
	}

	void OnLoad(SKSE::SerializationInterface* a_skse)
	{
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		a_skse->GetNextRecordInfo(type, version, length);

		if (type != SerializationType){
			return;
		}

		if (version != SerializationVersion){
			REX::ERROR("Unable to load data");
			return;
		}
		std::uint32_t last_bed_ID;
		if (!a_skse->ReadRecordData(last_bed_ID))
		{
			REX::ERROR("Failed to load record size");
			return;
		}
		else {
			ResEvent::SleepEvent::bed_ID = last_bed_ID;			
			//ResEvent::FindRefWithID(last_bed_ID);
			REX::INFO("Loaded last used bed FormID: {}", last_bed_ID);
		}
	}

	void OnRevert([[maybe_unused]] SKSE::SerializationInterface* a_skse)
	{
		ResEvent::SleepEvent::bed_ID = 0;
	}

}

