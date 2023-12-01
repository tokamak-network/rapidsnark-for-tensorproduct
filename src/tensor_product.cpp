#include <vector>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <nlohmann/json.hpp>

#include <alt_bn128.hpp>
#include "binfile_utils.hpp"
#include "zkey_utils.hpp"
#include "wtns_utils.hpp"

#include "groth16.hpp"

using json = nlohmann::json;

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

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
        typename Engine::FrElement product;
        // void *_array1 = argv[1];
        // void *_array2 = argv[2];
        int array1_size = atoi(argv[4]);
        int array2_size = atoi(argv[5]);

        std::string zkeyFilename = argv[6];
        std::string wtnsFilename = argv[7];
        std::string proofFilename = argv[8];
        std::string publicFilename = argv[9];

        auto zkey = BinFileUtils::openExisting(zkeyFilename, "zkey", 1);
        auto zkeyHeader = ZKeyUtils::loadHeader(zkey.get());

        auto prover = Groth16::makeProver<AltBn128::Engine>(
            zkeyHeader->nVars,
            zkeyHeader->nPublic,
            zkeyHeader->domainSize,
            zkeyHeader->nCoefs,
            zkeyHeader->vk_alpha1,
            zkeyHeader->vk_beta1,
            zkeyHeader->vk_beta2,
            zkeyHeader->vk_delta1,
            zkeyHeader->vk_delta2,
            zkey->getSectionData(4),    // Coefs
            zkey->getSectionData(5),    // pointsA
            zkey->getSectionData(6),    // pointsB1
            zkey->getSectionData(7),    // pointsB2
            zkey->getSectionData(8),    // pointsC
            zkey->getSectionData(9)     // pointsH1
        );

        AltBn128::FrElement *_product = (AltBn128::FrElement *)argv[1];
        AltBn128::FrElement *_array1 = (AltBn128::FrElement *)argv[2];
        AltBn128::FrElement *_array2 = (AltBn128::FrElement *)argv[3];
        
        auto result = prover->tensorProduct(_product, _array1, _array2, array1_size, array2_size);
        return 1;
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
}


// std::vector<std::vector<alt_bn128_Fr>> tensorProduct(const std::vector<std::vector<alt_bn128_Fr>>& _array1, const std::vector<std::vector<alt_bn128_Fr>>& _array2) {

//     std::vector<std::vector<alt_bn128_Fr>> product(_array1.size(), std::vector<alt_bn128_Fr>(_array2[0].size()));
//     #pragma omp parallel for
//     for (size_t i = 0; i < _array1.size(); i++) {
//         for (size_t j = 0; j < _array2[0].size(); j++) {
//             product[i][j] = alt_bn128_Fr::mul(_array2[0][j], _array1[i][0]);
//         }
//     }

//     return product;
// }

// int main() {
//     // Example usage
//     std::vector<std::vector<alt_bn128_Fr>> array1 = {{alt_bn128_Fr::random_element()}, {alt_bn128_Fr::random_element()}};
//     std::vector<std::vector<alt_bn128_Fr>> array2 = {{alt_bn128_Fr::random_element(), alt_bn128_Fr::random_element()}};

//     std::vector<std::vector<alt_bn128_Fr>> result = tensorProduct(array1, array2);

//     return 0;
// }