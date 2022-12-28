﻿#ifndef ARC_ASYNC_INVOKABLE_H
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
        AsyncInvokable(std::future<typename std::invoke_result<TFunc, TArgs...>::type>& ret, TFunc&& func, TArgs&&... args)
               : _inner(std::make_unique<innerAsyncInvokableGeneric<typename std::invoke_result<TFunc, TArgs...>::type, TFunc,int, TArgs...>>(ret, std::forward <TFunc>(func), std::forward<TArgs>(args)...)) {};

        template <typename TMemberFunc, typename TObject, typename... TArgs, typename = std::enable_if_t<std::is_member_function_pointer<TMemberFunc>::value>>
        AsyncInvokable(std::future<typename std::invoke_result<TMemberFunc, TObject, TArgs...>::type>& ret, TMemberFunc&& func, TObject&& inst, TArgs&&... args):
                _inner(std::make_unique<innerAsyncInvokableGeneric<typename std::invoke_result<TMemberFunc, TObject, TArgs...>::type, TMemberFunc, TObject, TArgs...>>(0,ret, std::forward<TMemberFunc>(func), std::forward<TObject>(inst), std::forward<TArgs>(args)...)) {};

        AsyncInvokable() : _inner(nullptr) {};

        bool invoke() const { if (_inner == nullptr) return false; return _inner->invoke(); };

        void wait() const{ if (_inner == nullptr) return; _inner->wait(); };

        struct innerAsyncInvokableBase
        {
            virtual ~innerAsyncInvokableBase() = default;
            virtual bool invoke() = 0;
            virtual void wait() = 0;
        };

        class NullFunctionCallException : public std::exception
        {
            public:
            char * what () {
                char* msg = (char*)"function pointer is nullptr";
                return msg;
            }
        };

        template <typename TRet, typename TFunc, typename TObject, typename... TArgs>
        struct innerAsyncInvokableGeneric : innerAsyncInvokableBase {

           innerAsyncInvokableGeneric(int dummy,std::future<TRet>& ret, TFunc&& func, TObject&& inst, TArgs&&... args
            ) :
                    _func(std::forward<TFunc>(func)),
                    _args(std::forward_as_tuple(std::forward<TObject>(inst), std::forward<TArgs>(args)...))
            {
                ret = _retval.get_future();
            };

            innerAsyncInvokableGeneric(std::future<TRet>& ret, TFunc&& func, TArgs&&... args
            ) :
                    _func(std::forward<TFunc>(func)),
                    _args(std::forward_as_tuple(std::forward<TArgs>(args)...))
            {
                ret = _retval.get_future();
            };

            bool invoke() override
            {
                bool invoked = false;
                if (_func == nullptr) {
                    _retval.set_exception(std::make_exception_ptr(NullFunctionCallException()));
                    return invoked;
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

                    invoked = true;
                }
                catch (...)
                {
                    _retval.set_exception(std::current_exception());
                }

                _func = nullptr;

                return invoked;
            }

            void wait() override { _retval.get_future().get(); }

            std::conditional_t<std::is_member_function_pointer<TFunc>::value, std::function<TRet(TObject&, TArgs...) >,std::function<TRet(TArgs...)>> _func{};
            std::conditional_t<std::is_member_function_pointer<TFunc>::value,std::tuple<TObject, TArgs...>,std::tuple<TArgs...>> _args{};
            std::promise<TRet> _retval;
        };

        std::unique_ptr<innerAsyncInvokableBase> _inner;
    };
}
#endif //ARC_ASYNC_INVOKABLE_H