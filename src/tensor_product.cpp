#include <vector>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
// #include <nlohmann/json.hpp>

#include <alt_bn128.hpp>
// #include "binfile_utils.hpp"
// #include "zkey_utils.hpp"
// #include "wtns_utils.hpp"

#include "groth16.hpp"

// using json = nlohmann::json;

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

namespace TensorProduct {
template <typename Engine>
int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Invalid number of parameters:\n";
        std::cerr << "Usage: <array1> <arry1_size> <array2> <array2_size>\n";
        return -1;
    }

    mpz_t altBbn128r;

    mpz_init(altBbn128r);
    mpz_set_str(altBbn128r, "21888242871839275222246405745257275088548364400416034343698204186575808495617", 10);
    // if (argv[1].size() == 1 && argv[1][0].size() == 1) {
    //     if (E.fr == argv[1][0][0]) {
    //         return {{E.fr}};
    //     }
    // }
    // if (argv[2].size() == 1 && argv[2][0].size() == 1) {
    //     if (E.fr == argv[2][0][0]) {
    //         return {{E.fr}};
    //     }
    // }
    
    try {
        typename Engine engine = new Engine()
        typename Engine::FrElement product;
        void *_array1 = argv[1];
        void *_array2 = argv[2];
        int array1_size = argv[3];
        int array2_size = argv[4];
        
        auto result = Groth16::tensorProduct<AltBn128::Engine>(_array1, _array2, array1_size, array2_size);
        return result;
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
}
} // namespace
