#pragma once

// EX)
// CoreDelegate<const int attacker, const int victim> delegate;
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
        functions.push_back(func);
        return *this;
    }

    CoreDelegate& operator-=(std::function<void(Types...)> const& func)
    {
        void (* const* func_ptr)(Types...) = func.template target<void(*)(Types...)>();
        const std::size_t func_hash = func.target_type().hash_code();

        if (nullptr == func_ptr)
        {
            for (auto iter = functions.begin(); iter != functions.end(); ++iter)
            {
                if (func_hash == (*iter).target_type().hash_code())
                {
                    functions.erase(iter);
                    return *this;
                }
            }
        }
        else
        {
            for (auto iter = functions.begin(); iter != functions.end(); ++iter)
            {
                void (* const* delegate_ptr)(Types...) = (*iter).template target<void(*)(Types...)>();
                if (nullptr != delegate_ptr && *func_ptr == *delegate_ptr)
                {
                    functions.erase(iter);
                    return *this;
                }
            }
        }

        return *this;
    }

    iterator begin() noexcept
    {
        return functions.begin();
    }

    iterator end() noexcept
    {
        return functions.end();
    }

    void clear()
    {
        functions.clear();
    }

private:
    CoreList<std::function<void(Types...)>> functions;
};