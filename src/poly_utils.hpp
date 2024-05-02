#ifndef POLY_UTILS_H
#define POLY_UTILS_H
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace PolyUtils {

  class Poly {

  public:
    void tensorProduct(AltBn128::FrElement *array1, AltBn128::FrElement *array2);

  }

}

#endif // POLY_UTILS_H
