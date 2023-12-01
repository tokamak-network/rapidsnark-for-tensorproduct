#ifndef FUNCTIONS_H
#define FUNCTIONS_H
// #include <iostream>
// #include <string>
// #include <fstream>
// #include "alt_bn128.hpp"
using namespace std;
// extern "C" 

// namespace functions
// {
  
void tensorProduct(
  int arr1[1024][1][32], 
  int arr2[1][32][32],
  int arr1_size,
  int arr2_size
);

// };
// #include "tp.cpp"

#endif
