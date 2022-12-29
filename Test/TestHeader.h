#ifndef TEST_HEADER_H
#define TEST_HEADER_H

#include "MoveOnly.hpp"
#include "Copyable.hpp"


std::string _message = "global";

void gbl_void_void() { std::cout << _message << " gbl_void_void" << std::endl; };
int gbl_int_int(int a) { std::cout << _message << " gbl_int_int" << std::endl; return a; };
int gbl_int_ull(unsigned long long a) { std::cout << _message << " gbl_int_ull" << std::endl; return a; };
int gbl_int_int_ref(int& a) { std::cout << _message << " gbl_int_int" << std::endl; a += 5; return a; };
void gbl_void_int(int a) { std::cout << _message << " gbl_void_int" << std::endl; };
void gbl_void_copyable(Copyable nc) { _message = _message; std::cout << _message << " gbl_void_copyable" << std::endl; };
void gbl_void_const_copyable(const Copyable nc) { _message = _message; std::cout << _message << " gbl_void_const_copyable" << std::endl; };
void gbl_void_copyable_ref(Copyable& nc) { _message = _message; std::cout << _message << " gbl_void_copyable_ref" << std::endl; };
void gbl_void_const_copyable_ref(const Copyable& nc) { _message = _message; std::cout << _message << " gbl_void_const_copyable_ref" << std::endl; };
void gbl_void_copyable_rvalue_ref(Copyable&& nc) { _message = _message; std::cout << _message << " gbl_void_copyable_rvalue_ref" << std::endl; };
void gbl_void_const_copyable_rvalue_ref(const Copyable&& nc) { _message = _message; std::cout << _message << " gbl_void_const_copyable_rvalue_ref" << std::endl; };
void gbl_void_MoveOnly(MoveOnly nc) { _message = _message; std::cout << _message << " gbl_void_MoveOnly" << std::endl; };

void gbl_void_int_throw_if_0(int i) {
    std::cout << _message << " gbl_void_int_throw_if_0\n";
	if (i == 0)
		throw std::invalid_argument("gbl_void_int_throw_if_0 arg is equal to 0\n");
};

void gbl_void_int_sleep_for(const std::chrono::duration<double, std::milli>& time)
{
    std::this_thread::sleep_for(time);
}

size_t gbl_size_t_size_t_sum(size_t& acc, size_t val)
{
    acc+= val;
    return acc;
}

long long gbl_lli_lli_ref_lli_sum(long long& acc, long long val)
{
    acc+= val;
    return acc;
}



size_t gbl_size_t_size_t_ret( size_t val)
{
return val;
}

#endif