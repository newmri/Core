#pragma once

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

        Stat(const int16_t* stats)
        {
            memcpy_s(this->value, sizeof(this->value), stats, sizeof(this->value));
        }

        int16_t value[Define::StatType_END];
    };

    struct CharacterGear
    {
        CharacterGear()
        {
        }

        CharacterGear(const uint8_t* gears)
        {
            this->index = gears;
        }

        CoreArray<uint8_t, Define::GearType_END> index;
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
    const NativeInfo::Vec2Int UnPackVec2Int(const Info::Vec2Int& obj);

    Info::Stat PackStat(const NativeInfo::Stat& obj);
    const NativeInfo::Stat UnPackStat(const Info::Stat& obj);

    Info::CharacterGear PackCharacterGear(const NativeInfo::CharacterGear& obj);
    const NativeInfo::CharacterGear UnPackCharacterGear(const Info::CharacterGear& obj);
}
