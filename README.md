# AsyncCallQueue
Similar to std::async functionality. AsyncCallQueue is a container class that store functions its arguments, and process calls asynchronously though a thread loop.
AsyncCallQueue guarantee that calls are issued in order and within the same thread.
Implementation is header only, it relies on std and c++17.

- support global functions
- member functions
- lambdas

    
```
#include "AsyncCallQueue.hpp"

using namespace anar;

class testClass
{
public :
int print(int arg) const {std::cout<<msg<<"\n";return arg;}
private :
std::string msg{"hello member function"};
};

int print(int arg)
{
std::cout<<"hello global function\n";
return arg;
}

int main()
{
int cap = 3;
testClass tc;

    AsyncCallQueue q(20); // max 20 calls waiting
    q.start(); //start processing
    auto retMemberFunc = q.enqueue(&testClass::print,std::cref(tc),1);
    auto retGlobalFunc = q.enqueue(print,2);
    auto retLambda = q.enqueue([cap](int arg) -> int{std::cout<<"hello lambda func\n";return arg + cap;},4);
    int val = retLambda.get(); //will sync previous call and retrieve result, id 3 + 4
    q.sync(); //flush queue
    return 0;
}
```