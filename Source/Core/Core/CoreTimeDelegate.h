#pragma once

static const int INSTANT_TIME = -1;

template <typename... Types>
class CoreTimeInfo
{
public:
    CoreTimeInfo()
    {

    }

    CoreTimeInfo(std::function<void(Types...)> func, Types... args, TIME_VALUE after = INSTANT_TIME, int maxCallCnt = 1) :
                 func(func), args(args...), calledCnt(0), maxCallCnt(maxCallCnt)
    {
        if (IS_NOT_SAME(INSTANT_TIME, after))
            this->activeTime = CORE_TIME_MANAGER.GetNowMilliSeconds() + after;
    }

    bool operator==(const CoreTimeInfo& rhs) const
    {
        return (this->activeTime == rhs.activeTime);
    }

    bool operator!=(const CoreTimeInfo& rhs) const
    {
        return !operator==(rhs);
    }

    bool operator<(const CoreTimeInfo& rhs) const
    {
        return (this->activeTime < rhs.activeTime);
    }

    bool operator>(const CoreTimeInfo& rhs) const
    {
        return (this->activeTime > rhs.activeTime);
    }

public:
    bool Run(void)
    {
        if (IS_SAME(INSTANT_TIME, this->activeTime))
        {
            std::apply(this->func, this->args);
            return (++this->calledCnt == this->maxCallCnt);
        }
        else
        {
            if (this->activeTime <= CORE_TIME_MANAGER.GetNowMilliSeconds())
            {
                std::apply(this->func, this->args);
                return true;
            }
        }

        return false;
    }

private:
    std::function<void(Types...)> func;
    std::tuple<Types...> args;

    int calledCnt;
    int maxCallCnt;
    TIME_VALUE activeTime;
};

template <typename... Types>
class CoreTimeDelegate
{
    CoreTimeDelegate& operator+=(CoreTimeInfo<Types...> info)
    {
        this->functions.push(info);
        return *this;
    }

    void clear()
    {
        this->functions.clear();
    }

private:
    CorePriorityQueue<CoreTimeInfo<Types...>> functions;
};