#ifndef TEST_MOVE_ONLY_HPP
#define TEST_MOVE_ONLY_HPP
#include <iostream>
#include "Copyable.hpp"

class MoveOnly
{
  public:
    MoveOnly(){};
    MoveOnly(std::string message) : _message(std::move(message)){};
    MoveOnly(const MoveOnly &o) = delete;
    MoveOnly &operator=(const MoveOnly &o) = delete;
    MoveOnly(MoveOnly &&o) = default;
    MoveOnly &operator=(MoveOnly &&o) = default;

    int static staticMember(int a)
    {
        std::cout << " static MoveOnly call " << std::to_string(a) << std::endl;
        return a;
    };
    void mem_void_void()
    {
        std::cout << _message << " mem_void_void" << std::endl;
    };
    int mem_int_int(int a)
    {
        std::cout << _message << " mem_int_int" << std::endl;
        return a;
    };
    void mem_void_int(int a)
    {
        std::cout << _message << " mem_void_int" << std::endl;
    };
    void mem_void_int_const(int a) const
    {
        std::cout << _message << " mem_void_int_const" << std::endl;
    };

    void mem_void_Copyable_const(Copyable nc) const
    {
        std::cout << _message << " mem_void_Copyable_const" << std::endl;
    };
    void mem_void_const_Copyable_const(const Copyable nc) const
    {
        std::cout << _message << " mem_void_const_Copyable_const" << std::endl;
    };
    void mem_void_Copyable(Copyable nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_Copyable" << std::endl;
    };
    void mem_void_const_Copyable(const Copyable nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_const_Copyable" << std::endl;
    };

    void mem_void_Copyable_ref_const(Copyable &nc) const
    {
        std::cout << _message << " mem_void_Copyable_ref_const" << std::endl;
    };
    void mem_void_const_Copyable_ref_const(const Copyable &nc) const
    {
        std::cout << _message << " mem_void_const_Copyable_ref_const" << std::endl;
    };
    void mem_void_Copyable_ref(Copyable &nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_Copyable_ref" << std::endl;
    };
    void mem_void_const_Copyable_ref(const Copyable &nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_const_Copyable_ref" << std::endl;
    };

    void mem_void_Copyable_rvalue_ref_const(Copyable &&nc) const
    {
        std::cout << _message << " mem_void_Copyable_rvalue_ref_const" << std::endl;
    };
    void mem_void_const_Copyable_rvalue_ref_const(const Copyable &&nc) const
    {
        std::cout << _message << " mem_void_const_Copyable_rvalue_ref_const" << std::endl;
    };
    void mem_void_Copyable_rvalue_ref(Copyable &&nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_Copyable_rvalue_ref" << std::endl;
    };
    void mem_void_const_Copyable_rvalue_ref(const Copyable &&nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_const_Copyable_rvalue_ref" << std::endl;
    };

    Copyable mem_Copyable_void()
    {
        std::cout << _message << " mem_Copyable_void" << std::endl;
        return Copyable{};
    };
    Copyable mem_Copyable_void_const() const
    {
        std::cout << _message << " mem_Copyable_void_const" << std::endl;
        return Copyable{};
    };
    MoveOnly mem_nonCopyable_void()
    {
        std::cout << _message << " mem_nonCopyable_void" << std::endl;
        return MoveOnly{};
    };
    MoveOnly mem_nonCopyable_void_const() const
    {
        std::cout << _message << " mem_nonCopyable_void" << std::endl;
        return MoveOnly{};
    };

    std::string _message = "non Copyable";
};

#endif