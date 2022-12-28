#ifndef ARC_ASYNC_CALL_QUEUE_H
#define ARC_ASYNC_CALL_QUEUE_H

#include "AsyncInvokable.hpp"
#include "ConcurrentDeque.hpp"


namespace arc {
    class AsyncCallQueue {
    public:
        explicit AsyncCallQueue(size_t lim = std::numeric_limits<size_t>::max()) :
                _exitRunner(false),
                _concurrentDeque(lim),
                _runner(&AsyncCallQueue::run, this) {};

        ~AsyncCallQueue() {
            sync();
            _exitRunner = true;
            sync();
            _runner.join();
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

        size_t size(){ return _concurrentDeque.size();};

        size_t maxSize(){ return _concurrentDeque.max_size();};

    private:

        void run() {
            for (;;) {
                AsyncInvokable elem{_concurrentDeque.pop()};
                elem.invoke();
                if (_exitRunner)
                {
                    return;
                }
            }
        }

        bool _exitRunner;
        ConcurrentDeque<AsyncInvokable> _concurrentDeque;
        std::thread _runner;
    };
}

#endif