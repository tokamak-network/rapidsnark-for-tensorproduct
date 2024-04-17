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

using json = nlohmann::json;
using std::ofstream;

const size_t BufferSize = 16384;
const size_t NUM_ROWS = 1024;
const size_t NUM_COLS = 32;

int main(int argc, char **argv) {
    mpz_t altBbn128r;
    json jsonResult;

    mpz_init(altBbn128r);
    mpz_set_str(altBbn128r, "21888242871839275222246405745257275088548364400416034343698204186575808495617", 10);
    try {
        std::string zkeyFilename = argv[1];
        std::string zkey1Filename = argv[2];
        std::string outputFilename = argv[3];

        auto zkey = BinFileUtils::openExisting(zkeyFilename, "zkey", 1);
        auto zkey1 = BinFileUtils::openExisting(zkey1Filename, "zkey", 1);

        AltBn128::FrElement *scaled = (AltBn128::FrElement *)zkey->getSectionData(2);
        AltBn128::FrElement *fYK = (AltBn128::FrElement *)zkey1->getSectionData(2);
        AltBn128::FrElement products;

        clock_t start = clock();
        // omp_set_dynamic(0);   
        // omp_set_num_threads(4);
        #pragma omp parallel for
        for (u_int64_t i=0; i < NUM_ROWS; i++) {
            json x;
            AltBn128::FrElement auxProduct;
            for (u_int64_t j=0; j < NUM_COLS; j++ ){
                AltBn128::Fr.mul(
                    products,
                    scaled[i],
                    fYK[j]
                );
                AltBn128::Fr.toMontgomery(auxProduct, products);
                x.push_back(AltBn128::Fr.toString(auxProduct));
            }
            jsonResult.push_back(x);
        }
        clock_t end = clock();
        std::cout <<"time duration for whole: "<< double(end - start) / CLOCKS_PER_SEC <<"\n";

        ofstream outputFile(outputFilename);
        outputFile << jsonResult.dump(4);
        outputFile.close();

    } catch (std::exception* e) {
        mpz_clear(altBbn128r);
        std::cerr << e->what() << '\n';
        return EXIT_FAILURE;
    }
    mpz_clear(altBbn128r);
}