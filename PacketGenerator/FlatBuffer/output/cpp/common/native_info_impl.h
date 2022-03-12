#pragma once

#include "define_protocol_generated.h"

namespace NativeInfo
{
    struct Vec2Int
    {
        Vec2Int()
        {        
        };

        Vec2Int(int x, int y) : x(x), y(y)
        {
        }

        int x = 0;
        int y = 0;
    };

    struct PositionInfo
    {
        PositionInfo()
        {
        };

        PositionInfo(NativeInfo::Vec2Int pos, Define::CreatureState state, Define::Dir moveDir) :
            pos(pos), state(state), moveDir(moveDir)
        {

        }

        NativeInfo::Vec2Int pos;
        Define::CreatureState state = Define::CreatureState::CreatureState_IDLE;
        Define::Dir moveDir = Define::Dir_RIGHT;
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

        std::array<int32_t, Define::StatType_END> value = {};
    };

    struct Ability
    {
        Ability()
        {
        }

        Ability(const flatbuffers::Array<int32_t, Define::AbilityType_END>* abilites)
        {
            std::copy(abilites->begin(), abilites->end(), this->value.begin());
        }

        std::array<int32_t, Define::AbilityType_END> value = {};
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

        std::array<uint8_t, Define::GearType_END> index = {};
    };

    struct Money
    {
        Money()
        {
        }

        Money(const flatbuffers::Array<int32_t, Define::MoneyType_END>* values)
        {
            std::copy(values->begin(), values->end(), this->value.begin());
        }

        std::array<int32_t, Define::MoneyType_END> value = {};
    };
}  

namespace Info
{
    struct Vec2Int;
    struct PositionInfo;
    struct Stat;
    struct Ability;
    struct CharacterGear;
    struct Money;
} 

namespace flatbuffers
{
    Info::Vec2Int PackVec2Int(const NativeInfo::Vec2Int& obj);
    NativeInfo::Vec2Int UnPackVec2Int(const Info::Vec2Int& obj);

    Info::PositionInfo PackPositionInfo(const NativeInfo::PositionInfo& obj);
    NativeInfo::PositionInfo UnPackPositionInfo(const Info::PositionInfo& obj);

    Info::Stat PackStat(const NativeInfo::Stat& obj);
    NativeInfo::Stat UnPackStat(const Info::Stat& obj);

    Info::Ability PackAbility(const NativeInfo::Ability& obj);
    NativeInfo::Ability UnPackAbility(const Info::Ability& obj);

    Info::CharacterGear PackCharacterGear(const NativeInfo::CharacterGear& obj);
    NativeInfo::CharacterGear UnPackCharacterGear(const Info::CharacterGear& obj);

    Info::Money PackMoney(const NativeInfo::Money& obj);
    NativeInfo::Money UnPackMoney(const Info::Money& obj);
}
