#ifndef TEST_HEADER_H
#define TEST_HEADER_H

#include "NotCopiable.hpp"
#include "Copiable.hpp"


std::string _message = "global";

void gbl_void_void() { std::cout << _message << " gbl_void_void" << std::endl; };
int gbl_int_int(int a) { std::cout << _message << " gbl_int_int" << std::endl; return a; };
void gbl_void_int(int a) { std::cout << _message << " gbl_void_int" << std::endl; };
void gbl_void_copiable(Copiable nc) { _message = _message; std::cout << _message << " gbl_void_copiable" << std::endl; };
void gbl_void_const_copiable(const Copiable nc) { _message = _message; std::cout << _message << " gbl_void_const_copiable" << std::endl; };
void gbl_void_copiable_ref(Copiable& nc) { _message = _message; std::cout << _message << " gbl_void_copiable_ref" << std::endl; };
void gbl_void_const_copiable_ref(const Copiable& nc) { _message = _message; std::cout << _message << " gbl_void_const_copiable_ref" << std::endl; };
void gbl_void_copiable_rvalue_ref(Copiable&& nc) { _message = _message; std::cout << _message << " gbl_void_copiable_rvalue_ref" << std::endl; };
void gbl_void_const_copiable_rvalue_ref(const Copiable&& nc) { _message = _message; std::cout << _message << " gbl_void_const_copiable_rvalue_ref" << std::endl; };
void gbl_void_notCopiable(NotCopiable nc) { _message = _message; std::cout << _message << " gbl_void_copiable" << std::endl; };


#endif