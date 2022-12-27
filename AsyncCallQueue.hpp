#ifndef ARC_ASYNC_CALL_QUEUE_H
#define ARC_ASYNC_CALL_QUEUE_H

#include "AsyncInvokable.hpp"
#include "ConcurrentDeque.hpp"
#include <future>

namespace arc {

    template<typename T = AsyncInvokable>
    class AsyncCallQueue {
    public:
        AsyncCallQueue(size_t lim) :
                _exitRunner(false),
                _concurrentDeque(lim),
                _runner(&AsyncCallQueue::run, this) {};

        ~AsyncCallQueue() {
            _exitRunner = true;
            void (*terminateToken)() = nullptr;
            auto ret = enqueue(terminateToken);

            _runner.join();
        }

        template<typename TFunc, typename... TArgs, typename = std::enable_if_t<!std::is_member_function_pointer<TFunc>::value>>
        [[nodiscard]] std::future<typename std::invoke_result<TFunc, TArgs...>::type>
        enqueue(TFunc &&func, TArgs &&... args) {
            std::future<typename std::invoke_result<TFunc, TArgs...>::type> fut;
            T elem{fut, std::forward<TFunc>(func), std::forward<TArgs>(args)...};
            _concurrentDeque.push(std::move(elem));
            return fut;
        };

        template<typename TFunc, typename Tinst, typename... TArgs, typename = std::enable_if_t<std::is_member_function_pointer<TFunc>::value>>
        [[nodiscard]] std::future<typename std::invoke_result<TFunc, Tinst, TArgs...>::type>
        enqueue(TFunc &&func, Tinst &&inst, TArgs &&... args) {
            std::future<typename std::invoke_result<TFunc, Tinst, TArgs...>::type> fut;
            T elem{fut, std::forward<TFunc>(func), std::forward<Tinst>(inst), std::forward<TArgs>(args)...};
            _concurrentDeque.push(std::move(elem));
            return fut;
        };

        void sync() {
            if (_concurrentDeque.size() == 0) return;
            void (*terminateToken)() = nullptr;
            this->enqueue(terminateToken).get();
        }

    private:

        void run() {
            for (;;) {
                T elem{_concurrentDeque.pop()};
                bool isInvoked = elem.invoke();
                if (_exitRunner && !isInvoked) return;
            }
        }

        bool _exitRunner;
        ConcurrentDeque <T> _concurrentDeque;
        std::thread _runner;
    };
}

#endif