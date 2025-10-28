#pragma once

namespace Serialisation
{
    static constexpr std::uint32_t SerializationVersion = 1;
    static constexpr std::uint32_t ID = 'SROS'; //Styyx Ring Of Sacrifice
    static constexpr std::uint32_t SerializationType = 'ROSD'; // Ring Of Sacrifice Data

    void OnSave(SKSE::SerializationInterface* a_skse);
    void OnLoad(SKSE::SerializationInterface* a_skse);
    void OnRevert([[maybe_unused]] SKSE::SerializationInterface* a_skse);
}