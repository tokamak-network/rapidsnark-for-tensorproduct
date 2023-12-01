#ifndef TENSOR_HPP
#define TENSOR_HPP

#pragma once
#include <stdio.h>
#include <sodium.h>
// #include "groth16.hpp"
// #include "alt_bn128.hpp"

#include <string>
// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

#include "binfile_utils.hpp"
// #include "fft.hpp"

#define DLLEXPORT extern "C" __declspec(DLLEXPORT)

namespace TensorProduct {
  template <typename Engine>
  class Multiflier {
    Engine &E;
  public:
    typename Engine::Array1 &_array1;
    typename Engine::Array2 &_array2;
    std::unique_ptr<TensorProduct<Engine>> tensorProduct(void *_array1, void *_arry2);
    DLLEXPORT;
  };

};

#include "tensor_product.cpp"

#endif