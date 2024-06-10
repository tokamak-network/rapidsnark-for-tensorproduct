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
#include "poly_utils.hpp"

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

using json = nlohmann::json;
using std::ofstream;
using namespace PolyUtils;

PolyUtils::Poly<AltBn128::Engine> polyUtil;
const size_t BufferSize = 16384;
const size_t NUM_ROWS = 1024;
const size_t NUM_COLS = 32;

void test_tensorproduct(typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector1, typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector2) {
    auto result = polyUtil.tensorProduct(vector1, vector2);

    printf("tensor product test result check start \n\n");
    for (int i = 0; i < 4; i++) {
        AltBn128::FrElement auxProduct;
        for (int j = 0; j < 4; j++) {
            AltBn128::Fr.toMontgomery(auxProduct, result[i][j]);
            printf("%s \n", AltBn128::Fr.toString(auxProduct).c_str());
        }
    }
    printf("\ntensor product test result check end \n\n");
    printf("----------------------------------------------------- \n\n");
}

void test_addpoly(typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector1, typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector2) {
    auto result = polyUtil.addPoly(vector1, vector2);

    printf("add poly test result start \n\n");
    for (int i = 0; i < 4; i++) {
        AltBn128::FrElement auxProduct;
        for (int j = 0; j < 4; j++) {
            AltBn128::Fr.toMontgomery(auxProduct, result[i][j]);
            printf("%s \n", AltBn128::Fr.toString(auxProduct).c_str());
        }
    }
    printf("\nadd poly test result end \n\n");
    printf("----------------------------------------------------- \n\n");
}

void test_mulpoly(typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector1, typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector2) {
    auto result = polyUtil.mulPoly(vector1, vector2);

    printf("mul poly test result start \n\n");
    for (int i = 0; i < 4; i++) {
        AltBn128::FrElement auxProduct;
        for (int j = 0; j < 4; j++) {
            AltBn128::Fr.toMontgomery(auxProduct, result[i][j]);
            printf("%s \n", AltBn128::Fr.toString(auxProduct).c_str());
        }
    }
    printf("\nmul poly test result end \n\n");
    printf("----------------------------------------------------- \n\n");
}

void test_subpoly(typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector1, typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector2) {
    auto result = polyUtil.subPoly(vector1, vector2);

    printf("sub poly test result start \n\n");
    for (int i = 0; i < 4; i++) {
        AltBn128::FrElement auxProduct;
        for (int j = 0; j < 4; j++) {
            AltBn128::Fr.toMontgomery(auxProduct, result[i][j]);
            printf("%s \n", AltBn128::Fr.toString(auxProduct).c_str());
        }
    }
    printf("\nsub poly test result end \n\n");
    printf("----------------------------------------------------- \n\n");
}

void test_fftMulPoly(typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector1, typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector2, FFT<AltBn128::Engine::Fr>* fft) {
    auto result = polyUtil.fftMulPoly(vector1, vector2, fft);

    printf("fftMul poly test result start \n\n");
    for (int i = 0; i < 4; i++) {
        AltBn128::FrElement auxProduct;
        for (int j = 0; j < 4; j++) {
            AltBn128::Fr.toMontgomery(auxProduct, result[i][j]);
            printf("%s \n", AltBn128::Fr.toString(auxProduct).c_str());
        }
    }
    printf("\nfftMul poly test result end \n\n");
    printf("----------------------------------------------------- \n\n");
}

void test_scalepoly(typename PolyUtils::Poly<AltBn128::Engine>::FrElementMatrix vector1, typename AltBn128::Engine::FrElement scaler) {
    auto result = polyUtil.scalePoly(vector1, scaler);

    printf("scale poly test result start \n\n");
    for (int i = 0; i < 4; i++) {
        AltBn128::FrElement auxProduct;
        for (int j = 0; j < 4; j++) {
            AltBn128::Fr.toMontgomery(auxProduct, result[i][j]);
            printf("%s \n", AltBn128::Fr.toString(auxProduct).c_str());
        }
    }
    printf("\nscale poly test result end \n\n");
    printf("----------------------------------------------------- \n\n");
}

int main(int argc, char **argv) {
    mpz_t altBbn128r;
    json jsonResult;

    mpz_init(altBbn128r);
    mpz_set_str(altBbn128r, "21888242871839275222246405745257275088548364400416034343698204186575808495617", 10);
    try {
        auto fft = new FFT<AltBn128::Engine::Fr>(1024*2);
        Poly<AltBn128::Engine> polyUtil;
        typedef Poly<AltBn128::Engine>::FrElementMatrix FrElementMatrix;
        int SIZE = 4;
        AltBn128::FrElement *sample1 = new AltBn128::FrElement[SIZE];
        AltBn128::FrElement *sample2 = new AltBn128::FrElement[SIZE];

        AltBn128::Fr.fromString(sample1[0], "1626275109576878988287730541908027724405348106427831594181487487855202143055");
        AltBn128::Fr.fromString(sample1[1], "18706364085805828895917702468512381358405767972162700276238017959231481018884");
        AltBn128::Fr.fromString(sample1[2], "17245156998235704504461341147511350131061011207199931581281143511105381019978");
        AltBn128::Fr.fromString(sample1[3], "3858908536032228066651712470282632925312300188207189106507111128103204506804");

        AltBn128::Fr.fromString(sample2[0], "1");
        AltBn128::Fr.fromString(sample2[1], "20187316456970436521602619671088988952475789765726813868033071292105413408473");
        AltBn128::Fr.fromString(sample2[2], "9163953212624378696742080269971059027061360176019470242548968584908855004282");
        AltBn128::Fr.fromString(sample2[3], "20922060990592511838374895951081914567856345629513259026540392951012456141360");

        for (int i = 0; i < SIZE; ++i) {
            AltBn128::Fr.fromMontgomery(sample1[i], sample1[i]);
            AltBn128::Fr.fromMontgomery(sample2[i], sample2[i]);
        }
        FrElementMatrix vector1, vector2, vector1_1;

        for (int i = 0; i < SIZE; ++i) {
            vector1.push_back({sample1[i]});
        }
        vector2.push_back({sample2[0], sample2[1], sample2[2], sample2[3]});
        vector1_1.push_back({sample1[0], sample1[1], sample1[2], sample1[3]});

        // test_tensorproduct(vector1, vector2);
        // test_addpoly(vector1, vector2);
        // test_subpoly(vector1_1, vector2);
        // test_mulpoly(vector1_1, vector2);
        test_fftMulPoly(vector1_1, vector2, fft);
        test_scalepoly(vector1_1, sample2[1]);

        delete[] sample1;
        delete[] sample2;
        delete fft;

    } catch (std::exception* e) {
        mpz_clear(altBbn128r);
        std::cerr << e->what() << '\n';
        return EXIT_FAILURE;
    }
    mpz_clear(altBbn128r);
}
