#include "AsyncInvokable.hpp"
#include "AsyncCallQueue.hpp"
#include "TestHeader.h"

using namespace arc;

int main()
{
    int iarg = 1;
    NotCopiable nc,nc2;
    Copiable c,c2,c3;
    std::future<void> ret_void;
    std::future<int> ret_int;
    std::future<Copiable> ret_copiable;
    std::future<NotCopiable> ret_nonCopiable;

    //member func
    AsyncInvokable mem_void_void{ ret_void, &Copiable::mem_void_void,(c) }; mem_void_void.invoke();
    AsyncInvokable mem_int_int_rvalue{ ret_int, &Copiable::mem_int_int,(c),1 }; mem_int_int_rvalue.invoke();
    AsyncInvokable mem_int_int_lvalue{ ret_int, &Copiable::mem_int_int,(c),iarg }; mem_int_int_lvalue.invoke();
    AsyncInvokable mem_void_int{ ret_void, &Copiable::mem_void_int,(c),iarg }; mem_void_int.invoke();
    AsyncInvokable mem_void_int_const{ ret_void, &Copiable::mem_void_int_const,(c),iarg }; mem_void_int_const.invoke();

    AsyncInvokable mem_void_copiable_const{ ret_void, &Copiable::mem_void_copiable_const,(c),c }; mem_void_copiable_const.invoke();
    AsyncInvokable mem_void_const_copiable_const{ ret_void, &Copiable::mem_void_const_copiable_const,(c),c }; mem_void_const_copiable_const.invoke();
    AsyncInvokable mem_void_copiable{ ret_void, &Copiable::mem_void_copiable,(c),c }; mem_void_copiable.invoke();
    AsyncInvokable mem_void_const_copiable{ ret_void, &Copiable::mem_void_const_copiable,(c),c }; mem_void_const_copiable.invoke();

    AsyncInvokable mem_void_copiable_ref_const{ ret_void, &Copiable::mem_void_copiable_ref_const,(c),c }; mem_void_copiable_ref_const.invoke();
    AsyncInvokable mem_void_const_copiable_ref_const{ ret_void, &Copiable::mem_void_const_copiable_ref_const,(c),c }; mem_void_const_copiable_ref_const.invoke();
    AsyncInvokable mem_void_copiable_ref{ ret_void, &Copiable::mem_void_copiable_ref,(c),c }; mem_void_copiable_ref.invoke();
    AsyncInvokable mem_void_const_copiable_ref{ ret_void, &Copiable::mem_void_const_copiable_ref,(c),c }; mem_void_const_copiable_ref.invoke();

    AsyncInvokable mem_void_copiable_rvalue_ref_const{ ret_void, &Copiable::mem_void_copiable_rvalue_ref_const,(c),std::move(c2)}; mem_void_copiable_rvalue_ref_const.invoke();
    AsyncInvokable mem_void_const_copiable_rvalue_ref_const{ ret_void, &Copiable::mem_void_const_copiable_rvalue_ref_const,(c),Copiable{} }; mem_void_const_copiable_rvalue_ref_const.invoke();
    AsyncInvokable mem_void_copiable_rvalue_ref{ ret_void, &Copiable::mem_void_copiable_rvalue_ref,(c),Copiable{} }; mem_void_copiable_rvalue_ref.invoke();
    AsyncInvokable mem_void_const_copiable_rvalue_ref{ ret_void, &Copiable::mem_void_const_copiable_rvalue_ref,(c),Copiable{} }; mem_void_const_copiable_rvalue_ref.invoke();

    AsyncInvokable mem_copiable_void{ ret_copiable, &Copiable::mem_copiable_void,(c) }; mem_copiable_void.invoke();
    AsyncInvokable mem_copiable_void_const{ ret_copiable, &Copiable::mem_copiable_void_const,(c)}; mem_copiable_void_const.invoke();

    //member func
    AsyncInvokable nonCopiable_mem_void_void{ ret_void, &NotCopiable::mem_void_void,(nc) }; nonCopiable_mem_void_void.invoke();
    AsyncInvokable nonCopiable_mem_int_int_rvalue{ ret_int, &NotCopiable::mem_int_int,(nc),1 }; nonCopiable_mem_int_int_rvalue.invoke();
    AsyncInvokable nonCopiable_mem_int_int_lvalue{ ret_int, &NotCopiable::mem_int_int,(nc),iarg }; nonCopiable_mem_int_int_lvalue.invoke();
    AsyncInvokable nonCopiable_mem_void_int{ ret_void, &NotCopiable::mem_void_int,(nc),iarg }; nonCopiable_mem_void_int.invoke();
    AsyncInvokable nonCopiable_mem_void_int_const{ ret_void, &NotCopiable::mem_void_int_const,(nc),iarg }; nonCopiable_mem_void_int_const.invoke();

    AsyncInvokable nonCopiable_mem_void_copiable_const{ ret_void, &NotCopiable::mem_void_copiable_const,(nc),c }; nonCopiable_mem_void_copiable_const.invoke();
    AsyncInvokable nonCopiable_mem_void_const_copiable_const{ ret_void, &NotCopiable::mem_void_const_copiable_const,(nc),c }; nonCopiable_mem_void_const_copiable_const.invoke();
    AsyncInvokable nonCopiable_mem_void_copiable{ ret_void, &NotCopiable::mem_void_copiable,(nc),c }; nonCopiable_mem_void_copiable.invoke();
    AsyncInvokable nonCopiable_mem_void_const_copiable{ ret_void, &NotCopiable::mem_void_const_copiable,(nc),c }; nonCopiable_mem_void_const_copiable.invoke();

    AsyncInvokable nonCopiable_mem_void_copiable_ref_const{ ret_void, &NotCopiable::mem_void_copiable_ref_const,(nc),c }; nonCopiable_mem_void_copiable_ref_const.invoke();
    AsyncInvokable nonCopiable_mem_void_const_copiable_ref_const{ ret_void, &NotCopiable::mem_void_const_copiable_ref_const,(nc),c }; nonCopiable_mem_void_const_copiable_ref_const.invoke();
    AsyncInvokable nonCopiable_mem_void_copiable_ref{ ret_void, &NotCopiable::mem_void_copiable_ref,(nc),c }; nonCopiable_mem_void_copiable_ref.invoke();
    AsyncInvokable nonCopiable_mem_void_const_copiable_ref{ ret_void, &NotCopiable::mem_void_const_copiable_ref,(nc),c }; nonCopiable_mem_void_const_copiable_ref.invoke();

    AsyncInvokable nonCopiable_mem_void_copiable_rvalue_ref_const{ ret_void, &NotCopiable::mem_void_copiable_rvalue_ref_const,(nc),std::move(c3) }; nonCopiable_mem_void_copiable_rvalue_ref_const.invoke();
    AsyncInvokable nonCopiable_mem_void_const_copiable_rvalue_ref_const{ ret_void, &NotCopiable::mem_void_const_copiable_rvalue_ref_const,(nc),Copiable{} }; nonCopiable_mem_void_const_copiable_rvalue_ref_const.invoke();
    AsyncInvokable nonCopiable_mem_void_copiable_rvalue_ref{ ret_void, &NotCopiable::mem_void_copiable_rvalue_ref,(nc),Copiable{} }; nonCopiable_mem_void_copiable_rvalue_ref.invoke();
    AsyncInvokable nonCopiable_mem_void_const_copiable_rvalue_ref{ ret_void, &NotCopiable::mem_void_const_copiable_rvalue_ref,(nc),Copiable{} }; nonCopiable_mem_void_const_copiable_rvalue_ref.invoke();

    AsyncInvokable nonCopiable_mem_copiable_void{ ret_copiable, &NotCopiable::mem_copiable_void,(nc) }; nonCopiable_mem_copiable_void.invoke();
    AsyncInvokable nonCopiable_mem_copiable_void_const{ ret_copiable, &NotCopiable::mem_copiable_void_const,(nc) }; nonCopiable_mem_copiable_void_const.invoke();

    AsyncInvokable f_gbl_void_void{ ret_void,gbl_void_void }; f_gbl_void_void.invoke();
    AsyncInvokable f_gbl_void_int{ ret_void,gbl_void_int,iarg}; f_gbl_void_int.invoke();
    AsyncInvokable f_gbl_int_int{ ret_int,gbl_int_int,iarg }; f_gbl_int_int.invoke();
    AsyncInvokable f_gbl_void_notCopiable{ ret_void,gbl_void_notCopiable,std::move(nc2) }; 
    f_gbl_void_notCopiable.invoke();  
    f_gbl_void_notCopiable.invoke(); //double call check


    AsyncCallQueue<AsyncInvokable> queue(20);
    ret_copiable = queue.enqueue(&NotCopiable::mem_copiable_void, (nc));
    ret_void = queue.enqueue(gbl_void_void);
    ret_void.get();
    Copiable ccccc = ret_copiable.get();
    
    return 0;
}
