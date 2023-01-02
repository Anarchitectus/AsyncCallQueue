#ifndef ARC_ASYNC_INVOKABLE_H
#define ARC_ASYNC_INVOKABLE_H

#include <future>

namespace arc
{
struct AsyncInvokable
{

    AsyncInvokable(const AsyncInvokable &o) = delete;

    AsyncInvokable &operator=(const AsyncInvokable &other) = delete;

    AsyncInvokable(AsyncInvokable &&o) noexcept = default;

    AsyncInvokable &operator=(AsyncInvokable &&other) noexcept = default;

    template <typename TFunc, typename... TArgs,
              typename = std::enable_if_t<!std::is_member_function_pointer<TFunc>::value>>
    AsyncInvokable(std::future<typename std::invoke_result<TFunc, TArgs...>::type> &ret, TFunc &&func, TArgs &&...args)
        : _inner(std::make_unique<
                 innerAsyncInvokableGeneric<typename std::invoke_result<TFunc, TArgs...>::type, TFunc, int, TArgs...>>(
              ret, std::forward<TFunc>(func), std::forward<TArgs>(args)...)){};

    template <typename TMemberFunc, typename TObject, typename... TArgs,
              typename = std::enable_if_t<std::is_member_function_pointer<TMemberFunc>::value>>
    AsyncInvokable(std::future<typename std::invoke_result<TMemberFunc, TObject, TArgs...>::type> &ret,
                   TMemberFunc &&func, TObject &&inst, TArgs &&...args)
        : _inner(
              std::make_unique<innerAsyncInvokableGeneric<
                  typename std::invoke_result<TMemberFunc, TObject, TArgs...>::type, TMemberFunc, TObject, TArgs...>>(
                  0, ret, std::forward<TMemberFunc>(func), std::forward<TObject>(inst),
                  std::forward<TArgs>(args)...)){};

    AsyncInvokable() : _inner(nullptr){};

    bool invoke() const
    {
        if (_inner == nullptr)
            return false;
        return _inner->invoke();
    };

    struct innerAsyncInvokableBase
    {
        virtual ~innerAsyncInvokableBase() = default;
        virtual bool invoke() = 0;
    };

    class NullFunctionCallException : public std::exception
    {
      public:
        char *what()
        {
            return (char *)"function pointer is nullptr";
        }
    };

    template <typename TRet, typename TFunc, typename TObject, typename... TArgs>
    struct innerAsyncInvokableGeneric : innerAsyncInvokableBase
    {
        innerAsyncInvokableGeneric(int /*dummy*/, std::future<TRet> &ret, TFunc &&func, TObject &&inst, TArgs &&...args)
            : _func(std::forward<TFunc>(func)), _args({std::forward<TObject>(inst), std::forward<TArgs>(args)...})
        {
            ret = _retval.get_future();
        };

        innerAsyncInvokableGeneric(std::future<TRet> &ret, TFunc &&func, TArgs &&...args)
            : _func(std::forward<TFunc>(func)), _args(std::forward<TArgs>(args)...)
        {
            ret = _retval.get_future();
        };

        bool invoke() override
        {
            if (_invoked)
                return _invoked; // only one invokation

            if (_func == nullptr)
            {
                _invoked = true;
                _retval.set_exception(std::make_exception_ptr(NullFunctionCallException()));
                return _invoked;
            }

            try
            {
                if constexpr (std::is_same<void, TRet>::value)
                {
                    std::apply(std::move(_func), std::move(_args));
                    _retval.set_value();
                }
                else
                {
                    _retval.set_value(std::apply(std::move(_func), std::move(_args)));
                }
            }
            catch (...)
            {
                _retval.set_exception(std::current_exception());
            }

            _invoked = true;

            return true;
        }

        std::conditional_t<
            std::is_member_function_pointer<TFunc>::value,
            std::function<TRet(typename std::decay<TObject &>::type, typename std::decay<TArgs>::type...)>,
            std::function<TRet(typename std::decay<TArgs>::type...)>>
            _func{};

        std::conditional_t<std::is_member_function_pointer<TFunc>::value,
                           std::tuple<typename std::decay<TObject>::type, typename std::decay<TArgs>::type...>,
                           std::tuple<typename std::decay<TArgs>::type...>>
            _args{};


        std::promise<TRet> _retval{};
        bool _invoked{false};
    };

    std::unique_ptr<innerAsyncInvokableBase> _inner;
};
} // namespace arc
#endif // ARC_ASYNC_INVOKABLE_H
