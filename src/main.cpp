#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <gmp.h>
#include <memory>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <alt_bn128.hpp>
#include <time.h>
#include "binfile_utils.hpp"
#include "zkey_utils.hpp"
#include "wtns_utils.hpp"
#include "fileloader.hpp"
// #include "tp.h"

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


const size_t BufferSize = 16384;

int main(int argc, char **argv) {
    clock_t start = clock();
    mpz_t altBbn128r;

    mpz_init(altBbn128r);
    mpz_set_str(altBbn128r, "21888242871839275222246405745257275088548364400416034343698204186575808495617", 10);
    try {
        std::string zkeyFilename = argv[1];
        std::string zkey1Filename = argv[2];

        char proofBuffer[BufferSize];
        auto zkey = BinFileUtils::openExisting(zkeyFilename, "zkey", 1);

        auto zkey1 = BinFileUtils::openExisting(zkey1Filename, "zkey", 1);

        FrElement *fYK = (FrElement *)zkey->getSectionData(2);
        FrElement *scaled = (FrElement *)zkey1->getSectionData(2);
        FrElement product;

        #pragma omp parallel for
        for (u_int64_t i=0; i<1024; i++) {
            for (u_int64_t j=0; j<32; j++ ){
                Fr_mul(
                    &product,
                    &fYK[j],
                    &scaled[i]
                );
            }
        }
        clock_t end = clock();
        std::cout <<"time duration for whole: "<< double(end - start) / CLOCKS_PER_SEC <<"\n";

    } catch (std::exception* e) {
        mpz_clear(altBbn128r);
        std::cerr << e->what() << '\n';
        return EXIT_FAILURE;
    }
    mpz_clear(altBbn128r);
}