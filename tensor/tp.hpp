// #ifndef FUNCTIONS_H
// #define FUNCTIONS_H
#include <iostream>
#include <string>
#include <fstream>
#include <alt_bn128.hpp>
using namespace std;

class functions
{
  public:
  void tensorProduct(
    typename AltBn128::Engine::FrElement& _array1, 
    typename AltBn128::Engine::FrElement& _array2,
    u_int32_t array1_size,
    u_int32_t array2_size
  );

};
