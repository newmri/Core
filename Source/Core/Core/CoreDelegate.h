#pragma once

// EX)
// CoreDelegate<const int32_t attacker, int32_t victim> delegate;
// --------------------
// function and lambda
// [Add] delegate += Attack;
// [Call] delegate(1, 2);  
// [Remove] delegate -= Attack;
// --------------------

// --------------------
// member function
// [Add] delegate += std::bind(&Obj::Attack, &obj, std::placeholders::_2);
// [Call] delegate(1, 2);
// [Remove] delegate -= std::bind(&Obj::Attack, &obj, std::placeholders::_2);
// --------------------

template <typename... Types>
class CoreDelegate
{
public:
    typedef typename CoreList<std::function<void(Types...)>>::iterator iterator;

    void operator()(const Types... args)
    {
        for (auto& func : functions)
        {
            func(args...);
        }
    }

    CoreDelegate& operator+=(std::function<void(Types...)> const& func)
    {
        this->functions.push_back(func);
        return *this;
    }

    CoreDelegate& operator-=(std::function<void(Types...)> const& func)
    {
        void (* const* func_ptr)(Types...) = func.template target<void(*)(Types...)>();
        const std::size_t func_hash = func.target_type().hash_code();

        if (IS_NULL(func_ptr))
        {
            for (auto iter = this->functions.begin(); iter != this->functions.end(); ++iter)
            {
                if (func_hash == (*iter).target_type().hash_code())
                {
                    this->functions.erase(iter);
                    return *this;
                }
            }
        }
        else
        {
            for (auto iter = this->functions.begin(); iter != this->functions.end(); ++iter)
            {
                void (* const* delegate_ptr)(Types...) = (*iter).template target<void(*)(Types...)>();
                if (IS_NOT_NULL(delegate_ptr) && *func_ptr == *delegate_ptr)
                {
                    this->functions.erase(iter);
                    return *this;
                }
            }
        }

        return *this;
    }

    iterator begin() noexcept
    {
        return this->functions.begin();
    }

    iterator end() noexcept
    {
        return this->functions.end();
    }

    void clear()
    {
        this->functions.clear();
    }

private:
    CoreList<std::function<void(Types...)>> functions;
};