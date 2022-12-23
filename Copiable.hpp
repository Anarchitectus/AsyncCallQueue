#ifndef TEST_COPIABLE_HPP
#define TEST_COPIABLE_HPP

#include "NotCopiable.hpp"
#include <iostream>
#include <string>

class NotCopiable;

class Copiable
{
public:
    Copiable() {};
    Copiable(std::string message) :_message(std::move(message)) {};
    Copiable(const Copiable& o) = default;
    Copiable& operator=(const Copiable& o) = default;
    Copiable(Copiable&& o) = default;
    Copiable& operator=(Copiable&& o) = default;
    std::string _message = "Copiable";

    int static staticMember(int a) { std::cout << " static Copiable call " << std::to_string(a) << std::endl; return a; };
    void mem_void_void() { std::cout << _message << " mem_void_void" << std::endl; };
    int mem_int_int(int a) { std::cout << _message << " mem_int_int" << std::endl; return a; };
    void mem_void_int(int a) { std::cout << _message << " mem_void_int" << std::endl; };
    void mem_void_int_const(int a) const { std::cout << _message << " mem_void_int_const" << std::endl; };

    void mem_void_copiable_const(Copiable nc) const { std::cout << _message << " mem_void_copiable_const" << std::endl; };
    void mem_void_const_copiable_const(const Copiable nc) const { std::cout << _message << " mem_void_const_copiable_const" << std::endl; };
    void mem_void_copiable(Copiable nc) { _message = _message; std::cout << _message << " mem_void_copiable" << std::endl; };
    void mem_void_const_copiable(const Copiable nc) { _message = _message; std::cout << _message << " mem_void_const_copiable" << std::endl; };

    void mem_void_copiable_ref_const(Copiable& nc) const { std::cout << _message << " mem_void_copiable_ref_const" << std::endl; };
    void mem_void_const_copiable_ref_const(const Copiable& nc) const { std::cout << _message << " mem_void_const_copiable_ref_const" << std::endl; };
    void mem_void_copiable_ref(Copiable& nc) { _message = _message; std::cout << _message << " mem_void_copiable_ref" << std::endl; };
    void mem_void_const_copiable_ref(const Copiable& nc) { _message = _message; std::cout << _message << " mem_void_const_copiable_ref" << std::endl; };

    void mem_void_copiable_rvalue_ref_const(Copiable&& nc) const { std::cout << _message << " mem_void_copiable_rvalue_ref_const" << std::endl; };
    void mem_void_const_copiable_rvalue_ref_const(const Copiable&& nc) const { std::cout << _message << " mem_void_const_copiable_rvalue_ref_const" << std::endl; };
    void mem_void_copiable_rvalue_ref(Copiable&& nc) { _message = _message; std::cout << _message << " mem_void_copiable_rvalue_ref" << std::endl; };
    void mem_void_const_copiable_rvalue_ref(const Copiable&& nc) { _message = _message; std::cout << _message << " mem_void_const_copiable_rvalue_ref" << std::endl; };

    Copiable mem_copiable_void() { std::cout << _message << " mem_copiable_void" << std::endl; return Copiable{}; };
    Copiable mem_copiable_void_const() const { std::cout << _message << " mem_copiable_void_const" << std::endl; return Copiable{}; };
};


#endif