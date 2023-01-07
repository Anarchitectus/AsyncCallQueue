#include <iostream>
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

    return 0;
}