#pragma once

namespace NativeInfo
{
    struct Vec2Int
    {
        Vec2Int()
        {
            x = 0;
            y = 0;
        };

        Vec2Int(int x, int y) : x(x), y(y)
        {
        }

        int x;
        int y;
    };
}  

namespace Info
{
    struct Vec2Int;
} 

namespace flatbuffers
{
    Info::Vec2Int PackVec2Int(const NativeInfo::Vec2Int& obj);
    const NativeInfo::Vec2Int UnPackVec2Int(const Info::Vec2Int& obj);
}
