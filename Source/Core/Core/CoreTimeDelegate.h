#pragma once

template <typename... Types>
class CoreTimeDelegate
{
public:
    CoreTimeDelegate()
    {

    }

    CoreTimeDelegate(std::function<void(Types...)> func, Types... args, TIME_VALUE after = 0, TIME_VALUE tick = 0, int32_t maxCallCnt = 1) :
        func(func), args(args...), tickTime(tick), calledCnt(0), maxCallCnt(maxCallCnt)
    {
        this->activeTime = CORE_TIME_MANAGER.GetNowMilliSeconds() + after;
    }

    bool operator==(const CoreTimeDelegate& rhs) const
    {
        return (this->activeTime == rhs.activeTime);
    }

    bool operator!=(const CoreTimeDelegate& rhs) const
    {
        return !operator==(rhs);
    }

    bool operator<(const CoreTimeDelegate& rhs) const
    {
        return (this->activeTime < rhs.activeTime);
    }

    bool operator>(const CoreTimeDelegate& rhs) const
    {
        return (this->activeTime > rhs.activeTime);
    }

public:
    bool Run(void)
    {
        TIME_VALUE currTime = CORE_TIME_MANAGER.GetNowMilliSeconds();

        if (this->activeTime <= currTime)
        {
            if (this->tickTime)
                this->activeTime += this->tickTime;

            if (IS_NULL(this->func))
                return true;

            std::apply(this->func, this->args);
            return (++this->calledCnt == this->maxCallCnt);
        }

        return false;
    }

private:
    std::function<void(Types...)> func;
    std::tuple<Types...> args;

    CACHE_ALIGN int32_t calledCnt;
    CACHE_ALIGN TIME_VALUE activeTime;

    int32_t maxCallCnt;
    TIME_VALUE tickTime;
};