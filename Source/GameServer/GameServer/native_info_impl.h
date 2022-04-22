#pragma once

#include "define_protocol_generated.h"

namespace NativeInfo
{
    struct Vec2Int
    {
        Vec2Int()
        {        
        };

        Vec2Int(const int32_t x, const int32_t y) : x(x), y(y)
        {
        }

        static Vec2Int Up(void)
        {
            return Vec2Int(0, 1);
        }

        static Vec2Int Down(void)
        {
            return Vec2Int(0, -1);
        }

        static Vec2Int Left(void)
        {
            return Vec2Int(-1, 0);
        }

        static Vec2Int Right(void)
        {
            return Vec2Int(1, 0);
        }

        void Normalize(void)
        {
            int32_t magnitude = GetMagnitude();
            x /= magnitude;
            y /= magnitude;
        }

        int32_t GetMagnitude(void)
        {
            return static_cast<int>(sqrtf(static_cast<float>(GetSqrMagnitude())));
        }

        int32_t GetSqrMagnitude(void)
        {
            return (x * x + y * y);
        }

        bool operator==(const Vec2Int& rhs) const
        {
            return (x == rhs.x && y == rhs.y);
        }

        Vec2Int operator+(const Vec2Int& rhs) const
        {
            return Vec2Int(x + rhs.x, y + rhs.y);
        }

        Vec2Int& operator+=(const Vec2Int& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Vec2Int operator-(const Vec2Int& rhs) const
        {
            return Vec2Int(x - rhs.x, y - rhs.y);
        }

        Vec2Int& operator-=(const Vec2Int& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        Define::Dir GetDirection(const Vec2Int& destPos)
        {
            NativeInfo::Vec2Int dir = destPos - *this;
            dir.Normalize();

            if (dir.x > 0)
                return Define::Dir_RIGHT;
            else
                return Define::Dir_LEFT;
        }

        Vec2Int GetFrontPos(const Define::Dir& dir, const uint8_t& range, Vec2Int& increasePos) const
        {
            Vec2Int destPos = *this;

			switch (dir)
			{
            case Define::Dir_UP:
                increasePos = Up();
                destPos.y += range;
                break;
            case Define::Dir_DOWN:
                increasePos = Down();
                destPos.y -= range;
                break;
			case Define::Dir_LEFT:
                increasePos = Left();
				destPos.x -= range;
				break;
			case Define::Dir_RIGHT:
                increasePos = Right();
				destPos.x += range;
                break;
			}

            return destPos;
        }

        Vec2Int GetFrontPos(const Define::Dir& dir)
        {
            Vec2Int destPos = *this;

            switch (dir)
            {
            case Define::Dir_UP:
                destPos += Up();
                break;
            case Define::Dir_DOWN:
                destPos += Down();
                break;
            case Define::Dir_LEFT:
                destPos += Left();
                break;
            case Define::Dir_RIGHT:
                destPos += Right();
                break;
            }

            return destPos;
        }

        Vec2Int GetFrontPos(const Define::Dir& dir, const uint8_t& range)
        {
            Vec2Int destPos = *this;

            switch (dir)
            {
            case Define::Dir_UP:
                destPos.y += range;
                break;
            case Define::Dir_DOWN:
                destPos.y -= range;
                break;
            case Define::Dir_LEFT:
                destPos.x -= range;
                break;
            case Define::Dir_RIGHT:
                destPos.x += range;
                break;
            }

            return destPos;
        }
        int32_t x = 0;
        int32_t y = 0;
    };

    struct PositionInfo
    {
        PositionInfo()
        {
        };

        PositionInfo(const int32_t mapID, const NativeInfo::Vec2Int pos, const Define::ObjectState state, const Define::Dir moveDir) :
            mapID(mapID), pos(pos), state(state), moveDir(moveDir)
        {

        }

        int32_t mapID = 0;
        NativeInfo::Vec2Int pos;
        Define::ObjectState state = Define::ObjectState::ObjectState_IDLE;
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

    struct Speed
    {
        Speed()
        {
        }

        Speed(const flatbuffers::Array<float_t, Define::SpeedType_END>* speeds)
        {
            std::copy(speeds->begin(), speeds->end(), this->value.begin());
        }

        std::array<float_t, Define::SpeedType_END> value = {};
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
    struct Speed;
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

    Info::Speed PackSpeed(const NativeInfo::Speed& obj);
    NativeInfo::Speed UnPackSpeed(const Info::Speed& obj);

    Info::CharacterGear PackCharacterGear(const NativeInfo::CharacterGear& obj);
    NativeInfo::CharacterGear UnPackCharacterGear(const Info::CharacterGear& obj);

    Info::Money PackMoney(const NativeInfo::Money& obj);
    NativeInfo::Money UnPackMoney(const Info::Money& obj);
}
