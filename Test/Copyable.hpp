#ifndef TEST_COPYABLE_HPP
#define TEST_COPYABLE_HPP

#include <iostream>
#include <string>

class Copyable
{
  public:
    Copyable()= default;
    explicit Copyable(std::string message) : _message(std::move(message)){};
    Copyable(const Copyable &o) = default;
    Copyable &operator=(const Copyable &o) = default;
    Copyable(Copyable &&o) = default;
    Copyable &operator=(Copyable &&o) = default;
    std::string _message = "Copyable";

    int static staticMember(int a)
    {
        std::cout << " static Copyable call " << std::to_string(a) << std::endl;
        return a;
    };
    void mem_void_void()
    {
        std::cout << _message << " mem_void_void" << std::endl;
    };
    void mem_void_int_ref(int &a)
    {
        std::cout << _message << " mem_void_int_ref" << std::endl;
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

    void mem_void_copyable_const(Copyable nc) const
    {
        std::cout << _message << " mem_void_copyable_const" << std::endl;
    };
    void mem_void_const_copyable_const(const Copyable nc) const
    {
        std::cout << _message << " mem_void_const_copyable_const" << std::endl;
    };
    void mem_void_copyable(Copyable nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_copyable" << std::endl;
    };
    void mem_void_const_copyable(const Copyable nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_const_copyable" << std::endl;
    };

    void mem_void_copyable_ref_const(Copyable &nc) const
    {
        std::cout << _message << " mem_void_copyable_ref_const" << std::endl;
    };
    void mem_void_const_copyable_ref_const(const Copyable &nc) const
    {
        std::cout << _message << " mem_void_const_copyable_ref_const" << std::endl;
    };
    void mem_void_copyable_ref(Copyable &nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_copyable_ref" << std::endl;
    };
    void mem_void_const_copyable_ref(const Copyable &nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_const_copyable_ref" << std::endl;
    };

    void mem_void_copyable_rvalue_ref_const(Copyable &&nc) const
    {
        std::cout << _message << " mem_void_copyable_rvalue_ref_const" << std::endl;
    };
    void mem_void_const_copyable_rvalue_ref_const(const Copyable &&nc) const
    {
        std::cout << _message << " mem_void_const_copyable_rvalue_ref_const" << std::endl;
    };
    void mem_void_copyable_rvalue_ref(Copyable &&nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_copyable_rvalue_ref" << std::endl;
    };
    void mem_void_const_copyable_rvalue_ref(const Copyable &&nc)
    {
        _message = nc._message;
        std::cout << _message << " mem_void_const_copyable_rvalue_ref" << std::endl;
    };

    Copyable mem_copyable_void()
    {
        std::cout << _message << " mem_copyable_void" << std::endl;
        return Copyable{};
    };
    Copyable mem_copyable_void_const() const
    {
        std::cout << _message << " mem_copyable_void_const" << std::endl;
        return Copyable{};
    };

    // MoveOnly mem_MoveOnly_void() { std::cout << _message << " mem_copyable_void" << std::endl; return MoveOnly{}; };
};

#endif