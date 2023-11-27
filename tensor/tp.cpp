
// #include <alt_bn128.hpp>
#include "tp.hpp"

void tensorProduct(
    typename AltBn128::Engine::FrElement *_array1, 
    typename AltBn128::Engine::FrElement *_array2,
    u_int32_t array1_size,
    u_int32_t array2_size
) {
    AltBn128::Engine E;
    typename AltBn128::Engine::FrElement coef;
    // E.fr.copy(coef, E.fr.zero());

    typename AltBn128::Engine::FrElement product;
    #pragma omp parallel for
    for (u_int64_t i = 0; i < array1_size; i++) {
        // for (size_t j = 0; j < array2_size; j++) {
            // E.fr.mul(product[i][j], _array2[0][j], _array1[i][0]);
            E.fr.mul(product, _array1[i], _array2);
        // }
    }

    // return product;
}

int main() {
    u_int32_t const array1_size = 10;
    u_int32_t const array2_size = 10;
    AltBn128::FrElement *array1 = new typename AltBn128::Engine::FrElement[array1_size];
    AltBn128::FrElement *array2 = new typename AltBn128::Engine::FrElement[array2_size];

    // AltBn128::Engine::FrElement result = tensorProduct(array1, array2, array1_size, array2_size);
    tensorProduct(array1, array2, array1_size, array2_size);

    return 0;
}
