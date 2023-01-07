# AsyncCallQueue
Similar to std::async functionality. AsyncCallQueue is a container that stores function calls, call arguments, and invoke them asynchronously through a thread loop.
AsyncCallQueue guarantee that calls are issued in order and within the same thread. 
It is possible to specify a max size that will make enqueue call blocking when max size is reached.
Implementation is header only (no dep), it relies on std and c++ 17.

handle different type of functions :

- global functions
- member functions
- lambdas
- function that throws



**Example** `main.cpp`

```
#include "AsyncCallQueue.hpp"

using namespace anar;

class testClass
{
  public:
    int print(int arg, int arg2, int arg3) const
    {
        std::cout << msg << "\n";
        return arg + arg2 + arg3;
    }

  private:
    std::string msg{"hello member function"};
};

int print(int arg)
{
    std::cout << "hello global function\n";
    return arg;
}

void printThrowIf0(int arg)
{
    std::cout << "hello global might throw\n";
    if (arg == 0)
        throw std::invalid_argument("arg is equal to 0\n");
};

int main()
{
    int cap = 3;
    testClass tc;

    // max 20 calls can be queued, after 20 enqueue is a blocking call
    AsyncCallQueue q(20);

    // start running the invoker thread
    q.start();
    // first param is the member function, second the const ref object, third the function arguments
    auto retMemberFunc = q.enqueue(&testClass::print, std::cref(tc), 1, 2, 3);
    assert(retMemberFunc.get() == 1 + 2 + 3);
    auto retGlobalFunc = q.enqueue(print, 2);
    assert(retGlobalFunc.get() == 2);
    auto retLambda = q.enqueue(
        [cap](int arg) -> int {
            std::cout << "hello lambda func\n";
            return arg + cap;
        },
        4);
    assert(retLambda.get() == 3 + 4);
    auto retThrow = q.enqueue(printThrowIf0, 0);
    // intercept exception with future::get()
    try
    {
        retThrow.get();
    }
    catch (std::invalid_argument &e)
    {
        std::cout << e.what() << "\n";
    }

    // blocking call to flush all the remaining calls
    // useless in this case retThrow.get() sync on the last queued call
    //~AsyncCallQueue dtor also sync()
    q.sync();

    return 0;
}
```