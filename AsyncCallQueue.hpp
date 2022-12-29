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
            _concurrentDeque(lim)
            //,_runner (&AsyncCallQueue::run, this)
        {
        };

        ~AsyncCallQueue() {
            end();
        }

        AsyncCallQueue(const AsyncCallQueue& o) = delete;

        AsyncCallQueue& operator=(const AsyncCallQueue& o) = delete;

        AsyncCallQueue(AsyncCallQueue&& o) noexcept = default;

        AsyncCallQueue& operator=(AsyncCallQueue&& o) noexcept = default;

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

        size_t size(){ return _concurrentDeque.size();};

        size_t maxSize(){ return _concurrentDeque.max_size();};

        void start()
        {
            if(!_runnerIsRunning) {
                _runner = std::thread(&AsyncCallQueue::run, this);
                _runnerIsRunning = true;
            }
        }


    public:


        void end()
        {
            if(!_runnerIsRunning)
                return ;

            sync();
            _exitRunner = true;
            if(!_exitedRunner)
                sync();

            if(_runner.joinable())
                _runner.join();
        }

        void run() {
            for (;;) {
                AsyncInvokable elem = _concurrentDeque.pop();

                elem.invoke(); //it is important to invoke, it is used as a sync element

                if (_exitRunner)
                {
                    _exitedRunner = true;
                    return;
                }
            }
        }

        volatile bool _runnerIsRunning{false};
        volatile bool _exitRunner{false};
        volatile bool _exitedRunner{false};
        ConcurrentDeque<AsyncInvokable> _concurrentDeque;
        std::thread _runner;
    };
}

#endif