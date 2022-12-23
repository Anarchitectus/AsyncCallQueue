#ifndef ARC_ASYNC_INVOKABLE_H
#define ARC_ASYNC_INVOKABLE_H

#include <functional>
#include <future>

namespace arc
{
    struct AsyncInvokable {

        AsyncInvokable(const AsyncInvokable& o) = delete;

        AsyncInvokable& operator=(const AsyncInvokable& other) = delete;

        AsyncInvokable(AsyncInvokable&& o) = default;

        AsyncInvokable& operator=(AsyncInvokable&& other) = default;

        template <typename TFunc, typename... TArgs, typename = std::enable_if_t<!std::is_member_function_pointer<TFunc>::value>>
        AsyncInvokable(std::future<typename std::invoke_result<TFunc, TArgs...>::type>& ret, TFunc&& func, TArgs&&... args) :
            _inner(std::make_unique<innerAsyncInvokable<typename std::invoke_result<TFunc, TArgs...>::type, TFunc, TArgs...>>(ret, std::forward <TFunc>(func), std::forward<TArgs>(args)...)) {};

        template <typename TFunc, typename TInst, typename... TArgs, typename = std::enable_if_t<std::is_member_function_pointer<TFunc>::value>>
        AsyncInvokable(std::future<typename std::invoke_result<TFunc, TInst, TArgs...>::type>& ret, TFunc&& func, TInst&& inst, TArgs&&... args) :
            _inner(std::make_unique<innerAsyncInvokableMember<typename std::invoke_result<TFunc, TInst, TArgs...>::type, TFunc, TInst, TArgs...>>(ret, std::forward<TFunc>(func), std::forward<TInst>(inst), std::forward<TArgs>(args)...)) {};

        AsyncInvokable() : _inner(nullptr) {};

        bool invoke() const { if (_inner == nullptr) return false; return _inner->invoke(); };

        void wait() const{ if (_inner == nullptr) return; _inner->wait(); };

        struct innerAsyncInvokableBase
        {
            virtual ~innerAsyncInvokableBase() = default;
            virtual bool invoke() = 0;
            virtual void wait() = 0;
        };

        template <typename TRet, typename TFunc, typename... TArgs>
        struct innerAsyncInvokable : innerAsyncInvokableBase {

            template <typename = std::enable_if_t<!std::is_member_function_pointer<TFunc>::value>>
            innerAsyncInvokable(std::future<typename std::invoke_result<TFunc, TArgs...>::type>& ret, TFunc&& func, TArgs&&... args) :
                _func(std::forward<TFunc>(func)),
                _args(std::forward_as_tuple(std::forward<TArgs>(args)...))
            {
                ret = _retval.get_future();
            };

            bool invoke() override
            {
                if (_func == nullptr) return false;

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

                _func = nullptr; //we can only invoke once

                return true;
            }

            void wait() override { _retval.get_future().get(); }

            std::function<TRet(TArgs...)> _func;
            std::tuple<TArgs...> _args;
            std::promise<TRet> _retval;
        };

        template <typename TRet, typename TFunc, typename TInst, typename... TArgs>
        struct innerAsyncInvokableMember : innerAsyncInvokableBase {

            innerAsyncInvokableMember(std::future<typename std::invoke_result<TFunc, TInst, TArgs...>::type>& ret, TFunc&& func, TInst&& inst, TArgs&&... args) :
                _func(std::forward<TFunc>(func)),
                _args(std::forward_as_tuple(std::forward<TInst>(inst), std::forward<TArgs>(args)...))
            {
                ret = _retval.get_future();
            };

            bool invoke() override
            {
                if (_func == nullptr) return false;
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

                _func = nullptr;

                return true;
            }

            void wait() override { _retval.get_future().get(); }

            std::function<TRet(TInst&, TArgs...) > _func;
            std::tuple<TInst, TArgs...> _args;
            std::promise<TRet> _retval;
        };

        std::unique_ptr<innerAsyncInvokableBase> _inner;
    };
}
#endif //ARC_ASYNC_INVOKABLE_H
