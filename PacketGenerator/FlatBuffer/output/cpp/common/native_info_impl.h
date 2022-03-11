#pragma once

#include "define_protocol_generated.h"

namespace NativeInfo
{
    struct Vec2Int
    {
        Vec2Int()
        {
            memset(this, 0, sizeof(this));
        };

        Vec2Int(int x, int y) : x(x), y(y)
        {
        }

        int x;
        int y;
    };

    struct Stat
    {
        Stat()
        {
        }

        Stat(const flatbuffers::Array<int32_t, Define::StatType_END>* stats)
        {
            std::copy(stats->begin(), stats->end(), this->value.begin());
        }

        std::array<int32_t, Define::StatType_END> value;
    };

    struct CharacterGear
    {
        CharacterGear()
        {
        }

        CharacterGear(const flatbuffers::Array<uint8_t, Define::GearType_END>* index)
        {
            std::copy(index->begin(), index->end(), this->index.begin());
        }

        std::array<uint8_t, Define::GearType_END> index;
    };
}  

namespace Info
{
    struct Vec2Int;
    struct Stat;
    struct CharacterGear;
} 

namespace flatbuffers
{
    Info::Vec2Int PackVec2Int(const NativeInfo::Vec2Int& obj);
    NativeInfo::Vec2Int UnPackVec2Int(const Info::Vec2Int& obj);

    Info::Stat PackStat(const NativeInfo::Stat& obj);
    NativeInfo::Stat UnPackStat(const Info::Stat& obj);

    Info::CharacterGear PackCharacterGear(const NativeInfo::CharacterGear& obj);
    NativeInfo::CharacterGear UnPackCharacterGear(const Info::CharacterGear& obj);
}
