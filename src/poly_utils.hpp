#ifndef POLY_UTILS_H
#define POLY_UTILS_H
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace PolyUtils {

  class Poly {

  public:
    std::vector<std::vector<AltBn128::FrElement>> tensorProduct(std::vector<std::vector<AltBn128::FrElement>>& array1, std::vector<std::vector<AltBn128::FrElement>>& array2);

  };

}

#endif // POLY_UTILS_H
