#include <vector>
#include <omp.h>
#include <stdio.h>
#include <memory>
#include "groth16.hpp"
// using namespace CPlusPlusLogging;

namespace TensorProduct {
template <typename Engine>
std::unique_ptr<TensorProduct<Engine>> tensorProduct(void *_array1, void *_array2) {
    // if (_array1.size() == 1 && _array1[0].size() == 1) {
    //     if (E.fr == _array1[0][0]) {
    //         return {{E.fr}};
    //     }
    // }
    // if (_array2.size() == 1 && _array2[0].size() == 1) {
    //     if (E.fr == _array2[0][0]) {
    //         return {{E.fr}};
    //     }
    // }
    typename Engine::FrElement product;
    product(_array1.size(), std::vector<T>(_array2[0].size()));

    #pragma omp parallel for
    for (size_t i = 0; i < _array1.size(); i++) {
        for (size_t j = 0; j < _array2[0].size(); j++) {
            product[i][j] = E.fr.mul(_array2[0][j], _array1[i][0]);
        }
    }

    return product;
}
} // namespace
