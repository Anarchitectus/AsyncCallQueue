#include "../AsyncCallQueue.hpp"
#include "TestHeader.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>
#include <vector>
using namespace arc;

int main()
{
#ifdef NDEBUG

#endif

    int iarg = 1;
    int iarg2 = 1;
    int& iarg2_ref = iarg2;
    MoveOnly nc, nc2;
    Copyable c, c2, c3;
    std::future<void> ret_void;
    std::future<int> ret_int;
    std::future<Copyable> ret_copyable;
    std::future<MoveOnly> ret_moveOnly;

    //AsyncInvokable f_gbl_void_void{ ret_void,gbl_void_void }; f_gbl_void_void.invoke();
    //member func
//    AsyncInvokable mem_void_void{ ret_void, &Copyable::mem_void_void,(c) }; mem_void_void.invoke();
//    AsyncInvokable mem_int_int_rvalue{ ret_int, &Copyable::mem_int_int,(c),1 }; mem_int_int_rvalue.invoke();
//    AsyncInvokable mem_int_int_lvalue{ ret_int, &Copyable::mem_int_int,(c),iarg }; mem_int_int_lvalue.invoke();
//    AsyncInvokable mem_void_int{ ret_void, &Copyable::mem_void_int,(c),iarg }; mem_void_int.invoke();
//    AsyncInvokable mem_void_int_const{ ret_void, &Copyable::mem_void_int_const,(c),iarg }; mem_void_int_const.invoke();
//
//    AsyncInvokable mem_void_Copyable_const{ ret_void, &Copyable::mem_void_copyable_const,(c),c }; mem_void_Copyable_const.invoke();
//    AsyncInvokable mem_void_const_Copyable_const{ ret_void, &Copyable::mem_void_const_copyable_const,(c),c }; mem_void_const_Copyable_const.invoke();
//    AsyncInvokable mem_void_copyable{ ret_void, &Copyable::mem_void_copyable,(c),c }; mem_void_copyable.invoke();
//    AsyncInvokable mem_void_const_copyable{ ret_void, &Copyable::mem_void_const_copyable,(c),c }; mem_void_const_copyable.invoke();
//
//    AsyncInvokable mem_void_copyable_ref_const{ ret_void, &Copyable::mem_void_copyable_ref_const,(c),c }; mem_void_copyable_ref_const.invoke();
//    AsyncInvokable mem_void_const_copyable_ref_const{ ret_void, &Copyable::mem_void_const_copyable_ref_const,(c),c }; mem_void_const_copyable_ref_const.invoke();
//    AsyncInvokable mem_void_copyable_ref{ ret_void, &Copyable::mem_void_copyable_ref,(c),c }; mem_void_copyable_ref.invoke();
//    AsyncInvokable mem_void_const_copyable_ref{ ret_void, &Copyable::mem_void_const_copyable_ref,(c),c }; mem_void_const_copyable_ref.invoke();
//
//    AsyncInvokable mem_void_copyable_rvalue_ref_const{ ret_void, &Copyable::mem_void_copyable_rvalue_ref_const,(c),std::move(c2) }; mem_void_copyable_rvalue_ref_const.invoke();
//    AsyncInvokable mem_void_const_copyable_rvalue_ref_const{ ret_void, &Copyable::mem_void_const_copyable_rvalue_ref_const,(c),Copyable{} }; mem_void_const_copyable_rvalue_ref_const.invoke();
//    AsyncInvokable mem_void_copyable_rvalue_ref{ ret_void, &Copyable::mem_void_copyable_rvalue_ref,(c),Copyable{} }; mem_void_copyable_rvalue_ref.invoke();
//    AsyncInvokable mem_void_const_copyable_rvalue_ref{ ret_void, &Copyable::mem_void_const_copyable_rvalue_ref,(c),Copyable{} }; mem_void_const_copyable_rvalue_ref.invoke();
//
//    AsyncInvokable mem_copyable_void{ ret_copyable, &Copyable::mem_copyable_void,(c) }; mem_copyable_void.invoke();
//    AsyncInvokable mem_copyable_void_const{ ret_copyable, &Copyable::mem_copyable_void_const,(c) }; mem_copyable_void_const.invoke();
//
//    //member func
//    AsyncInvokable nonCopyable_mem_void_void{ ret_void, &MoveOnly::mem_void_void,(nc) }; nonCopyable_mem_void_void.invoke();
//    AsyncInvokable nonCopyable_mem_int_int_rvalue{ ret_int, &MoveOnly::mem_int_int,(nc),1 }; nonCopyable_mem_int_int_rvalue.invoke();
//    AsyncInvokable nonCopyable_mem_int_int_lvalue{ ret_int, &MoveOnly::mem_int_int,(nc),iarg }; nonCopyable_mem_int_int_lvalue.invoke();
//    AsyncInvokable nonCopyable_mem_void_int{ ret_void, &MoveOnly::mem_void_int,(nc),iarg }; nonCopyable_mem_void_int.invoke();
//    AsyncInvokable nonCopyable_mem_void_int_const{ ret_void, &MoveOnly::mem_void_int_const,(nc),iarg }; nonCopyable_mem_void_int_const.invoke();
//
//    AsyncInvokable nonCopyable_mem_void_Copyable_const{ ret_void, &MoveOnly::mem_void_Copyable_const,(nc),c }; nonCopyable_mem_void_Copyable_const.invoke();
//    AsyncInvokable nonCopyable_mem_void_const_Copyable_const{ ret_void, &MoveOnly::mem_void_const_Copyable_const,(nc),c }; nonCopyable_mem_void_const_Copyable_const.invoke();
//    AsyncInvokable nonCopyable_mem_void_Copyable{ ret_void, &MoveOnly::mem_void_Copyable,(nc),c }; nonCopyable_mem_void_Copyable.invoke();
//    AsyncInvokable nonCopyable_mem_void_const_Copyable{ ret_void, &MoveOnly::mem_void_const_Copyable,(nc),c }; nonCopyable_mem_void_const_Copyable.invoke();
//
//    AsyncInvokable nonCopyable_mem_void_Copyable_ref_const{ ret_void, &MoveOnly::mem_void_Copyable_ref_const,(nc),c }; nonCopyable_mem_void_Copyable_ref_const.invoke();
//    AsyncInvokable nonCopyable_mem_void_const_Copyable_ref_const{ ret_void, &MoveOnly::mem_void_const_Copyable_ref_const,(nc),c }; nonCopyable_mem_void_const_Copyable_ref_const.invoke();
//    AsyncInvokable nonCopyable_mem_void_Copyable_ref{ ret_void, &MoveOnly::mem_void_Copyable_ref,(nc),c }; nonCopyable_mem_void_Copyable_ref.invoke();
//    AsyncInvokable nonCopyable_mem_void_const_Copyable_ref{ ret_void, &MoveOnly::mem_void_const_Copyable_ref,(nc),c }; nonCopyable_mem_void_const_Copyable_ref.invoke();
//
//    AsyncInvokable nonCopyable_mem_void_Copyable_rvalue_ref_const{ ret_void, &MoveOnly::mem_void_Copyable_rvalue_ref_const,(nc),std::move(c3) }; nonCopyable_mem_void_Copyable_rvalue_ref_const.invoke();
//    AsyncInvokable nonCopyable_mem_void_const_Copyable_rvalue_ref_const{ ret_void, &MoveOnly::mem_void_const_Copyable_rvalue_ref_const,(nc),Copyable{} }; nonCopyable_mem_void_const_Copyable_rvalue_ref_const.invoke();
//    AsyncInvokable nonCopyable_mem_void_Copyable_rvalue_ref{ ret_void, &MoveOnly::mem_void_Copyable_rvalue_ref,(nc),Copyable{} }; nonCopyable_mem_void_Copyable_rvalue_ref.invoke();
//    AsyncInvokable nonCopyable_mem_void_const_Copyable_rvalue_ref{ ret_void, &MoveOnly::mem_void_const_Copyable_rvalue_ref,(nc),Copyable{} }; nonCopyable_mem_void_const_Copyable_rvalue_ref.invoke();
//
//    AsyncInvokable nonCopyable_mem_Copyable_void{ ret_copyable, &MoveOnly::mem_Copyable_void,(nc) }; nonCopyable_mem_Copyable_void.invoke();
//    AsyncInvokable nonCopyable_mem_Copyable_void_const{ ret_copyable, &MoveOnly::mem_Copyable_void_const,(nc) }; nonCopyable_mem_Copyable_void_const.invoke();
//
//    AsyncInvokable f_gbl_void_void{ ret_void,gbl_void_void }; f_gbl_void_void.invoke();
//    AsyncInvokable f_gbl_void_int{ ret_void,gbl_void_int,iarg }; f_gbl_void_int.invoke();
//    AsyncInvokable f_gbl_void_int_ref{ ret_int,gbl_int_int_ref,iarg2_ref }; f_gbl_void_int_ref.invoke();
//    AsyncInvokable f_gbl_int_int{ ret_int,gbl_int_int,iarg }; f_gbl_int_int.invoke();
//    AsyncInvokable f_gbl_void_MoveOnly{ ret_void,gbl_void_MoveOnly,std::move(nc2) };
//    f_gbl_void_MoveOnly.invoke();
//    f_gbl_void_MoveOnly.invoke(); //double call check
//
//    AsyncInvokable f_gbl_void_int_throw{ ret_void, gbl_void_int_throw_if_0,0 };
//    try { f_gbl_void_int_throw.invoke(); }
//    catch (std::invalid_argument& e) { std::cout << e.what(); }
//    int cap = 3;
//    int addlambda = 5;
//    AsyncInvokable  lambda_int_int{ ret_int ,[cap](int a) -> int {std::cout << "lambda_int_int " << std::to_string(a + cap);  return a+cap;},addlambda };
//    lambda_int_int.invoke();
//    assert(ret_int.get() == addlambda + cap);
//    AsyncInvokable f_gbl_int_int{ ret_int,gbl_int_int,iarg };
//    f_gbl_int_int.invoke();
//    f_gbl_int_int.invoke();



//    AsyncCallQueue queue(20);
//    //ret_copyable = queue.enqueue(&MoveOnly::mem_Copyable_void, (nc));
//    ret_int = queue.enqueue(gbl_int_int,1);
//    ret_int = queue.enqueue(gbl_int_int,1);
//    ret_void = queue.enqueue(gbl_void_int_throw_if_0, 1);
//    ret_void = queue.enqueue(gbl_void_int_throw_if_0, 0);
//    //ret_int = queue.enqueue(gbl_int_int_ref, iarg2_ref);
//    // retiarg2 = ret_int.get();
//    //assert(retiarg2 == iarg2_ref);
//    ret_int.get();
//    //ret_int.get();
//    try
//    {
//        ret_void.get();
//    }
//    catch (std::invalid_argument& e)
//    {
//        std::cout << e.what();
//    }
//
//    //ret_copyable.get();


    /////////////////////////////////////////////////////////////TIME TEST/////////////////////////////////////////////////////

    //std::srand(unsigned(std::time(nullptr)));

    size_t queue_count = 10;
    size_t max_size = 1000;
    size_t max_time = 10;

    std::vector<AsyncCallQueue> queues(queue_count);
    std::vector<std::vector<size_t>> queue_delays(queue_count);
    std::vector<size_t> queue_sum_delays(queue_count);

    std::generate(queue_delays.begin(),queue_delays.end(),[max_size](){return std::vector<size_t>(max_size);});

    int counter = 0;
    for(auto& delays : queue_delays) {
        std::generate(delays.begin(), delays.end(),
                      [max_time]() { return static_cast<double>(std::rand()) / RAND_MAX * max_time; });
        queue_sum_delays[counter] = std::accumulate(delays.begin(),delays.end(),0);
        counter++;
    }

    auto max_delay = *max_element(std::begin(queue_sum_delays), std::end(queue_sum_delays));

    //The actual calls
    auto start = std::chrono::high_resolution_clock::now();
    counter = 0;
    for(auto& queue: queues)
    {
        for(auto delay : queue_delays[counter])
            queue.enqueue(gbl_void_int_sleep_for,std::chrono::duration<double, std::milli>(delay));

        counter++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    for(auto& queue: queues)
        queue.sync();
    auto endSync = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsedAsync = end-start;
    std::cout << "Waited Async " << elapsedAsync.count() << " ms\n";
    std::chrono::duration<double, std::milli> elapsedSync = endSync-start;
    std::cout << "Waited Sync " << elapsedSync.count() << " ms\n";
    std::cout << "Expected " << max_delay << " ms\n";
    return 0;
}
