
// #include <alt_bn128.hpp>
#include <tp.hpp>

void tensorProduct(
    typename AltBn128::Engine::FrElement *_array1, 
    typename AltBn128::Engine::FrElement *_array2,
    u_int32_t array1_size,
    u_int32_t array2_size
) {
    AltBn128::Engine E;
    // std::vector<std::vector<AltBn128::FrElement>> product(_array1.size(), std::vector<AltBn128::FrElement>(_array2[0].size()));
    // u_int32_t const array1_size = _array1.size();
    // u_int32_t const array2_size = _array2[0].size();
    auto product = new typename AltBn128::Engine::FrElement[array1_size];
    #pragma omp parallel for
    for (u_int64_t i = 0; i < array1_size; i++) {
        // for (size_t j = 0; j < array2_size; j++) {
            // E.fr.mul(product[i][j], _array2[0][j], _array1[i][0]);
            E.fr.mul(product[i], _array2, _array1[i]);
        // }
    }

    // return product;
}

int main() {
    u_int32_t const array1_size = 10;
    u_int32_t const array2_size = 10;
    auto array1 = new typename AltBn128::Engine::FrElement[array1_size];
    auto array2 = new typename AltBn128::Engine::FrElement[array2_size];

    // AltBn128::Engine::FrElement result = tensorProduct(array1, array2, array1_size, array2_size);
    tensorProduct(array1, array2, array1_size, array2_size);

    return 0;
}
