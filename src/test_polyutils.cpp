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

const size_t BufferSize = 16384;
const size_t NUM_ROWS = 1024;
const size_t NUM_COLS = 32;

void test_tensorproduct () {
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

  std::vector<std::vector<AltBn128::FrElement>> vector1, vector2;
  
  for (int i = 0; i < SIZE; ++i) {
    vector1.push_back({sample1[i]});
  }
  vector2.push_back({sample2[0], sample2[1], sample2[2], sample2[3]});

  PolyUtils::Poly polyUtil;
  std::vector<std::vector<AltBn128::FrElement>> result = polyUtil.tensorProduct(vector1, vector2);

  for (int i = 0; i < SIZE; i++) {
    AltBn128::FrElement auxProduct;
    for (int j = 0; j < SIZE; j++) {
      AltBn128::Fr.toMontgomery(auxProduct, result[i][j]);
      printf("%s \n", AltBn128::Fr.toString(auxProduct).c_str());
    }
  }

  delete[] sample1;
  delete[] sample2;
}

int main(int argc, char **argv) {
    mpz_t altBbn128r;
    json jsonResult;

    mpz_init(altBbn128r);
    mpz_set_str(altBbn128r, "21888242871839275222246405745257275088548364400416034343698204186575808495617", 10);
    try {
      // AltBn128::FrElement sample1 = new AltBn128::FrElement[4];
      // AltBn128::FrElement sample2 = new AltBn128::FrElement[4];

      // AltBn128::Fr.fromString(sample1[0], "1626275109576878988287730541908027724405348106427831594181487487855202143055");
      // AltBn128::Fr.fromString(sample1[1], "18706364085805828895917702468512381358405767972162700276238017959231481018884");
      // AltBn128::Fr.fromString(sample1[2], "17245156998235704504461341147511350131061011207199931581281143511105381019978");
      // AltBn128::Fr.fromString(sample1[3], "3858908536032228066651712470282632925312300188207189106507111128103204506804");

      // AltBn128::Fr.fromString(sample2[0], "1")
      // AltBn128::Fr.fromString(sample2[1], "20187316456970436521602619671088988952475789765726813868033071292105413408473");
      // AltBn128::Fr.fromString(sample2[2], "9163953212624378696742080269971059027061360176019470242548968584908855004282");
      // AltBn128::Fr.fromString(sample2[3], "20922060990592511838374895951081914567856345629513259026540392951012456141360");

      // std::vector<AltBn128::FrElement> vector1;
      // std::vector<AltBn128::FrElement> vector2;

      // vector1.push_back(sample1);
      // vector2.push_back(sample2);

      test_tensorproduct ();
      

    } catch (std::exception* e) {
        mpz_clear(altBbn128r);
        std::cerr << e->what() << '\n';
        return EXIT_FAILURE;
    }
    mpz_clear(altBbn128r);
}
