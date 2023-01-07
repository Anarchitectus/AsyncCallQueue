#ifndef ANAR_ASYNC_CALL_QUEUE_H
#define ANAR_ASYNC_CALL_QUEUE_H

#include "AsyncInvokable.hpp"
#include "ConcurrentDeque.hpp"
#include <memory>

namespace anar
{

class AsyncCallQueue
{
  public:
    explicit AsyncCallQueue(size_t lim = std::numeric_limits<size_t>::max())
        : impl(std::make_unique<AsyncCallQueueImpl>(lim)){}

    ~AsyncCallQueue()
    {
        if (impl != nullptr)
        stop();
    }

    AsyncCallQueue(const AsyncCallQueue &o) = delete;

    AsyncCallQueue &operator=(const AsyncCallQueue &o) = delete;

    AsyncCallQueue(AsyncCallQueue &&other) noexcept = default;

    AsyncCallQueue &operator=(AsyncCallQueue &&other) noexcept = default;

    template <typename TFunc, typename... TArgs,
              typename = std::enable_if_t<!std::is_member_function_pointer<TFunc>::value>>
    [[nodiscard]] std::future<typename std::invoke_result<TFunc, TArgs...>::type> enqueue(TFunc &&func, TArgs &&...args)
    {
        return impl->enqueue(func, args...);
    };

    template <typename TMemberFunc, typename TObject, typename... TArgs,
              typename = std::enable_if_t<std::is_member_function_pointer<TMemberFunc>::value>>
    [[nodiscard]] std::future<typename std::invoke_result<TMemberFunc, TObject, TArgs...>::type> enqueue(
        TMemberFunc &&func, TObject &&inst, TArgs &&...args)
    {
        return impl->enqueue(func, inst, args...);
    };

    void sync()
    {
        impl->sync();
    }

    [[nodiscard]] size_t size() const
    {
        return impl->size();
    };

    [[nodiscard]] size_t maxSize() const
    {
        return impl->maxSize();
    };

    void run()
    {
        impl->run();
    }

    void stop()
    {
        impl->stop();
    }

  private:
    
      
class AsyncCallQueueImpl
    {
      public:
        explicit AsyncCallQueueImpl(size_t lim = std::numeric_limits<size_t>::max())
            : _concurrentDeque(lim), 
            //_exit_sig_future(_exit_sig.get_future()),
            _exit_flag(false)
        {};

        ~AsyncCallQueueImpl()
        {
            stop();
        }

        AsyncCallQueueImpl(const AsyncCallQueueImpl &o) = delete;

        AsyncCallQueueImpl &operator=(const AsyncCallQueueImpl &o) = delete;

        AsyncCallQueueImpl(AsyncCallQueueImpl &&other) noexcept = delete;

        AsyncCallQueueImpl &operator=(AsyncCallQueueImpl &&other) noexcept = delete;

        template <typename TFunc, typename... TArgs,
                  typename = std::enable_if_t<!std::is_member_function_pointer<TFunc>::value>>
        [[nodiscard]] std::future<typename std::invoke_result<TFunc, TArgs...>::type> enqueue(TFunc &&func,
                                                                                              TArgs &&...args)
        {
            std::future<typename std::invoke_result<TFunc, TArgs...>::type> fut;
            AsyncInvokable elem{fut, std::forward<TFunc>(func), std::forward<TArgs>(args)...};
            _concurrentDeque.push(std::move(elem));
            return fut;
        };

        template <typename TMemberFunc, typename TObject, typename... TArgs,
                  typename = std::enable_if_t<std::is_member_function_pointer<TMemberFunc>::value>>
        [[nodiscard]] std::future<typename std::invoke_result<TMemberFunc, TObject, TArgs...>::type> enqueue(
            TMemberFunc &&func, TObject &&inst, TArgs &&...args)
        {
            std::future<typename std::invoke_result<TMemberFunc, TObject, TArgs...>::type> fut;
            AsyncInvokable elem{fut, std::forward<TMemberFunc>(func), std::forward<TObject>(inst),
                                std::forward<TArgs>(args)...};
            _concurrentDeque.push(std::move(elem));
            return fut;
        };

        void sync()
        {
            try
            {
                enqueue([]() {}).get();
            }
            catch (...)
            {
            }
        }

        [[nodiscard]] size_t size() const
        {
            return _concurrentDeque.size();
        };

        [[nodiscard]] size_t maxSize() const
        {
            return _concurrentDeque.max_size();
        };

        void run()
        {
            if (!_runnerIsRunning)
            {
                auto lock{std::scoped_lock(_mutex)};
                if (!_runnerIsRunning)
                {
                    _runner = std::thread(&AsyncCallQueueImpl::work, this);
                    _runnerIsRunning = true;
                }
            }
        }

        void stop()
        {
            {
                auto lock{std::scoped_lock(_mutex)};
                if (!_runnerIsRunning)
                    return;

                sync();
                //_exit_sig.set_value();
                _exit_flag.store(true);
                // push another. thread may be waiting for pop, need to do another iteration
                std::ignore = enqueue([]() {}); 
                if (_runner.joinable())
                    _runner.join();

                _runnerIsRunning = false;
            }
        }

      private:
        void work()
        {
            //while (_exit_sig_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
            while (!_exit_flag.load())
            {
                _concurrentDeque.pop().invoke();
            }
        }

        mutable std::mutex _mutex;
        ConcurrentDeque<AsyncInvokable> _concurrentDeque;
        //std::promise<void> _exit_sig;
        //std::future<void> _exit_sig_future;
        std::atomic_bool _exit_flag;
        bool _runnerIsRunning{false};
        std::thread _runner;
    };
      
    std::unique_ptr<AsyncCallQueueImpl> impl;
};

} // namespace anar

#endif