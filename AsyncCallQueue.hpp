#ifndef ARC_ASYNC_CALL_QUEUE_H
#define ARC_ASYNC_CALL_QUEUE_H

#include "AsyncInvokable.hpp"
#include "ConcurrentDeque.hpp"
#include <iostream>
#include <thread>

namespace arc {
    class AsyncCallQueue {
    public:
        explicit AsyncCallQueue(size_t lim = std::numeric_limits<size_t>::max()) :
            _concurrentDeque(lim),
            _exit_sig_future(_exit_sig.get_future())
        {};

        ~AsyncCallQueue() {
            terminate();
        }

       AsyncCallQueue(const AsyncCallQueue& o) = delete;

       AsyncCallQueue& operator=(const AsyncCallQueue& o) = delete;

        AsyncCallQueue(AsyncCallQueue&& other) noexcept
        {
            auto lock{ std::scoped_lock(other._mutex) };
            _concurrentDeque = std::exchange(other._concurrentDeque, ConcurrentDeque<AsyncInvokable>{});
            _exit_sig_future = std::exchange(other._exit_sig_future, std::future<void>{});
            _exit_sig = std::exchange(other._exit_sig, std::promise<void>{});
        }

        AsyncCallQueue& operator=(AsyncCallQueue&& other) noexcept
        {
            {
                auto lock{ std::scoped_lock(_mutex, other._mutex) };
                _concurrentDeque = std::exchange(other._concurrentDeque, ConcurrentDeque<AsyncInvokable>{});
                _exit_sig_future = std::exchange(other._exit_sig_future, std::future<void>{});
                _exit_sig = std::exchange(other._exit_sig, std::promise<void>{});
            }

            return *this;
        }

        template<typename TFunc, typename... TArgs, typename = std::enable_if_t<!std::is_member_function_pointer<TFunc>::value>>
        [[nodiscard]] std::future<typename std::invoke_result<TFunc, TArgs...>::type>
        enqueue(TFunc &&func, TArgs &&... args) {
            std::future<typename std::invoke_result<TFunc, TArgs...>::type> fut;
            AsyncInvokable elem{fut, std::forward<TFunc>(func), std::forward<TArgs>(args)...};
            _concurrentDeque.push(std::move(elem));
            return fut;
        };

        template<typename TMemberFunc, typename TObject, typename... TArgs, typename = std::enable_if_t<std::is_member_function_pointer<TMemberFunc>::value>>
        [[nodiscard]] std::future<typename std::invoke_result<TMemberFunc, TObject, TArgs...>::type>
        enqueue(TMemberFunc &&func, TObject &&inst, TArgs &&... args) {
            std::future<typename std::invoke_result<TMemberFunc, TObject, TArgs...>::type> fut;
            AsyncInvokable elem{fut, std::forward<TMemberFunc>(func), std::forward<TObject>(inst), std::forward<TArgs>(args)...};
            _concurrentDeque.push(std::move(elem));
            return fut;
        };

        void sync() {
            void (*syncTok)(){nullptr};
            auto retSyncTok = enqueue(syncTok);

            try{
                retSyncTok.get();
            }catch(...){}
        }

        [[nodiscard]] size_t size() const { return _concurrentDeque.size();};

        [[nodiscard]] size_t maxSize() const{ return _concurrentDeque.max_size();};

        void run()
        {
            if (!_runnerIsRunning)
            {
                auto lock{ std::scoped_lock(_mutex) };
                if (!_runnerIsRunning) {
                    _runner = std::thread(&AsyncCallQueue::work, this);
                    _runnerIsRunning = true;
                }
            }
        }


    private:


        void terminate()
        {
            {
                auto lock{std::scoped_lock(_mutex)};
                if (!_runnerIsRunning)
                    return;
            }

            sync();
            _exit_sig.set_value();
            std::ignore = enqueue([](){}); //push another thread may be waiting, need to do another iteration
            if(_runner.joinable())
                _runner.join();
        }

        void work() {
            while ( _exit_sig_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
            {
                _concurrentDeque.pop().invoke();
            }
        }

        mutable std::mutex _mutex;
        ConcurrentDeque<AsyncInvokable> _concurrentDeque;
        std::promise<void> _exit_sig;
        std::future<void> _exit_sig_future;
        bool _runnerIsRunning{false};
        std::thread _runner;
    };
}

#endif