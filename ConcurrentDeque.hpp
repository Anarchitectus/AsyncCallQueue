#ifndef ANAR_CONCURRENT_DEQUEUE_HPP
#define ANAR_CONCURRENT_DEQUEUE_HPP

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

namespace anar
{

template <typename T> class ConcurrentDeque
{
  public:
    explicit ConcurrentDeque(size_t maxSize = std::numeric_limits<size_t>::max()) : _maxSize(maxSize){};

    ConcurrentDeque(const ConcurrentDeque &other)
    {
        auto lock{std::scoped_lock(other._mutex)};
        _deque = other._deque;
    }

    ConcurrentDeque(ConcurrentDeque &&other) noexcept
    {
        auto lock{std::scoped_lock(other._mutex)};
        _deque = std::exchange(other._deque, {});
    }

    ConcurrentDeque &operator=(const ConcurrentDeque &other)
    {
        {
            auto lock{std::scoped_lock(_mutex, other._mutex)};
            _deque = other._deque;
        }
        _conditionEmpty.notify_all();
        return *this;
    }

    ConcurrentDeque &operator=(ConcurrentDeque &&other) noexcept
    {
        {
            auto lock{std::scoped_lock(_mutex, other._mutex)};
            _deque = std::exchange(other._deque, {});
        }
        _conditionEmpty.notify_all();
        return *this;
    }

    ~ConcurrentDeque() = default;

    [[nodiscard]] auto empty() const
    {
        auto lock{std::scoped_lock(_mutex)};
        return _deque.empty();
    }

    [[nodiscard]] size_t size() const
    {
        auto lock{std::scoped_lock(_mutex)};
        return _deque.size();
    }

    [[nodiscard]] size_t max_size() const
    {
        auto lock{std::scoped_lock(_mutex)};
        return _deque.max_size();
    }

    void push(const T &value)
    {
        auto lock{std::unique_lock(_mutex)};
        while (_maxSize != std::numeric_limits<size_t>::max() && _deque.size() == _maxSize)
        {
            _conditionFull.wait(lock);
        }

        _deque.push_back(value);
        lock.unlock();
        _conditionEmpty.notify_one();
    }

    void push(T &&value)
    {
        auto lock{std::unique_lock(_mutex)};
        while (_maxSize != std::numeric_limits<size_t>::max() && _deque.size() == _maxSize)
        {
            _conditionFull.wait(lock);
        }

        _deque.push_back(std::move(value));
        lock.unlock();
        _conditionEmpty.notify_one();
    }

    template <class... Args> void emplace(Args &&...args)
    {
        T new_item{std::forward<Args>(args)...};
        push(std::move(new_item));
    }

    T pop()
    {
        auto lock{std::unique_lock(_mutex)};
        while (_deque.empty())
        {
            _conditionEmpty.wait(lock);
        }
        T rc{std::move(_deque.front())};
        _deque.pop_front();
        lock.unlock();
        _conditionFull.notify_one();
        return rc;
    }

    std::optional<T> try_pop()
    {
        auto lock{std::unique_lock(_mutex)};
        if (_deque.empty())
        {
            lock.unlock();
            return std::nullopt;
        }
        T rc{std::move(_deque.front())};
        _deque.pop_front();
        lock.unlock();
        _conditionFull.notify_one();
        return rc;
    }

  private:
    std::deque<T> _deque{};
    std::condition_variable _conditionEmpty{};
    std::condition_variable _conditionFull{};
    mutable std::mutex _mutex{};
    size_t _maxSize = std::numeric_limits<size_t>::max();
};
} // namespace anar

#endif